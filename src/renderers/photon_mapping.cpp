#include "utils/prog_bar.hpp"
#include "utils/image.h"
#include "utils/math_util.h"
#include "utils/debug.h"

#include "core/camera.h"
#include "core/ray.h"
#include "core/material.h"

#include "./photon_mapping.h"

namespace RT {

PhotonMappingRender::PhotonMappingRender(
        float alpha,
        float init_radius,
        int num_rounds,
        int photons_per_round,
        int vp_per_pixel,
        const SceneParser &scene_parser
        ) :
        gamma(scene_parser.gamma),
        obj(scene_parser.scene.get()),
        camera(scene_parser.camera.get()),
        lights(scene_parser.lights),
        bg_color(scene_parser.bg_color),
        alpha(alpha),
        init_radius(init_radius),
        num_rounds(num_rounds),
        photons_per_round(photons_per_round),
        vp_per_pixel(vp_per_pixel),
        ball_finder(init_radius * 2)
        {
    width = camera->getWidth();
    height = camera->getHeight();
}

void PhotonMappingRender::Render(const std::string &output_file) {
    visible_point_map.resize(width * height);
    img_data.resize(width * height);
    size_t photons_per_light = photons_per_round / lights.size();
    size_t true_photons_per_round = photons_per_light * lights.size();

    for (int r = 0; r < num_rounds; r++) {
        ProgressBar bar_forward(fmt::format("Forward round {}", r + 1), width * height);
#pragma omp parallel for schedule(dynamic, 4) collapse(2) default(none) shared(bar_forward)
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                RNG per_thread_rng;
                Ray ray = camera->generateRay({
                        (float) x + 0.5f + 0.5f * per_thread_rng.RandTentFloat(),
                        (float) y + 0.5f + 0.5f * per_thread_rng.RandTentFloat()
                }, per_thread_rng);
                // modifies vp, ball_finder
                auto &vp = visible_point_map[y * width + x] = VisiblePoint();
                trace_visible_point(vp, ray, per_thread_rng, 0);
                if (vp.radius > 0) {
#pragma omp critical
                    ball_finder.AddBall(&vp);
                }
                bar_forward.Step();
            }
        }

        ProgressBar bar_back(fmt::format("Back round {}", r + 1), true_photons_per_round);
#pragma omp parallel for schedule(dynamic, 20) collapse(2) default(none) shared(bar_back, photons_per_light)
        for (int l = 0; l < lights.size(); l++) {
            for (int p = 0; p < photons_per_light; p++) {
                RNG per_thread_rng;
                auto ray = lights[l]->EmitRay(per_thread_rng);
                // modifies some vp
                trace_photon(ray, per_thread_rng, 0);
                bar_back.Step();
            }
        }
        ball_finder.Reset();

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                VisiblePoint &vp = visible_point_map[y * width + x];
                Vector3f photon_color = vp.photon_flux / ((float) M_PI * fsquare(vp.radius) * (float) photons_per_round);
                Vector3f color = photon_color + vp.forward_flux;
                img_data[y * width + x] += color;
            }
        }
    }

    Image img(width, height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Vector3f color = img_data[y * width + x] / (float) num_rounds;
            img.SetPixel(x, y, gamma_correct(color, gamma));
        }
    }
    img.SaveImage(output_file.c_str());
}

void PhotonMappingRender::trace_visible_point(VisiblePoint &vp, const Ray &ray, RNG &rng, int depth) {
    if (depth > 10) return;

    Hit hit;
    bool is_hit = obj->Intersect(ray, hit, 0.0001);
    if (!is_hit) {
        vp.forward_flux = vp.attenuation * bg_color;
        return;
    }
    vp.attenuation = vp.attenuation * hit.GetAmbient();
    const Material *mat = hit.GetMaterial();

    if (mat->IsDiffuse()) {
        vp.forward_flux = vp.attenuation * mat->emissionColor;
        vp.center = hit.GetPos();
        vp.radius = init_radius;
        return;
    } else {
        auto ray_out_dir = mat->Sample(ray, hit, rng);
        Ray out_ray(hit.GetPos(), ray_out_dir);
        trace_visible_point(vp, out_ray, rng, depth + 1);
    }
}

void PhotonMappingRender::trace_photon(const ColoredRay &ray, RNG &rng, int depth) {
    if (depth > 20) return;

    Hit hit;
    bool is_hit = obj->Intersect(ray, hit, 0.0001);
    if (!is_hit) return;

    const Material *mat = hit.GetMaterial();
    Vector3f hit_ambient = hit.GetAmbient();

    if (mat->IsDiffuse()) {
        update_nearby_vp(hit.GetPos(), ray.GetColor());
        if (depth > 5 && rng.RandUniformFloat() < hit_ambient.max_component()) {
            return;
        }
    }
    auto ray_out_dir = mat->Sample(ray, hit, rng);
    ColoredRay out_ray(hit.GetPos(), ray_out_dir, hit_ambient * ray.GetColor());
    trace_photon(out_ray, rng, depth + 1);
}

void PhotonMappingRender::update_nearby_vp(const Vector3f &pos, const Vector3f &attenuation) {
    ball_finder.FindAndOperateBalls(pos, [&, this](VisiblePoint *vp) {
        std::unique_lock<std::mutex> lk(*vp->mtx);
        float radius_factor = ((float) vp->num_photons * alpha + alpha) / ((float) vp->num_photons * alpha + 1);
        vp->num_photons++;
        vp->photon_flux = (vp->photon_flux + vp->attenuation * attenuation / M_PI) * radius_factor;
        vp->radius *= std::sqrt(radius_factor);
    });
}

} // namespace RT
