#include "utils/prog_bar.hpp"
#include "utils/image.h"
#include "utils/math_util.h"

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
        vp_per_pixel(vp_per_pixel)
        {
    width = camera->getWidth();
    height = camera->getHeight();
}

void PhotonMappingRender::Render(const std::string &output_file) {
    visible_point_map.resize(width * height);
    for (int r = 0; r < num_rounds; r++) {
        ProgressBar bar_forward(fmt::format("Forward round {}", r + 1), width * height);
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                const Ray &ray = camera->generateRay(Vector2f((float) x + 0.5f, (float) y + 0.5f));
                trace_visible_point(visible_point_map[y * width + x], ray);
                bar_forward.Step();
            }
        }

        ProgressBar bar_back(fmt::format("Back round {}", r + 1), lights.size() * photons_per_round);
        for (const auto &light: lights) {
            for (int p = 0; p < photons_per_round; p++) {
                auto ray = light->EmitRay();
                trace_photon(ray);
                bar_back.Step();
            }
        }
    }

    Image img(camera->getWidth(), camera->getHeight());
    for (int y = 0; y < camera->getHeight(); y++) {
        for (int x = 0; x < camera->getWidth(); x++) {
            VisiblePoint &vp = visible_point_map[y * width + x];
            Vector3f photon_color = vp.photon_flux / ((float) M_PI * fsquare(vp.radius));
            Vector3f forward_color = vp.forward_flux;
            Vector3f color = photon_color / (float) photons_per_round + forward_color;
            color = color / (float) num_rounds;
            img.SetPixel(x, y, gamma_correct(color, gamma));
        }
    }
    img.SaveImage(output_file.c_str());
}

void PhotonMappingRender::trace_visible_point(VisiblePoint &vp, const Ray &ray) {
    Ray tracing_ray(ray);
    vp.attenuation = Vector3f(1, 1, 1);
    int depth = 0;
    while (true) {
        depth++;
        if (depth > 5) return;

        Hit hit;
        bool is_hit = obj->Intersect(tracing_ray, hit, 0);
        if (!is_hit) {
            vp.forward_flux = vp.attenuation * bg_color;  // hit not in scene, no pos
            return;
        }
        const Material *mat = hit.GetMaterial();

        vp.attenuation = vp.attenuation * hit.GetAmbient();
        if (mat->IsDiffuse()) {
            vp.center = hit.GetPos();
            vp.forward_flux = vp.attenuation * mat->emissionColor;
            vp.radius = init_radius;
            ball_finder.AddBall(&vp);
            return;
        } else {
            auto ray_out_dir = mat->Sample(tracing_ray, hit);
            tracing_ray.set(hit.GetPos(), ray_out_dir);
        }
    }
}

void PhotonMappingRender::trace_photon(const ColoredRay &ray) {
    Ray tracing_ray = ray;
    Vector3f attenuation = ray.GetColor();
    int depth = 0;
    while (true) {
        depth ++;
        if (depth > 5) return;

        Hit hit;
        bool is_hit = obj->Intersect(tracing_ray, hit, 0);
        if (!is_hit) return;

        const Material *mat = hit.GetMaterial();

        if (mat->IsDiffuse()) {
            attenuation = attenuation * hit.GetAmbient();
            update_nearby_vp(hit.GetPos(), attenuation);
        } else {
            auto ray_out_dir = mat->Sample(tracing_ray, hit);
            tracing_ray.set(hit.GetPos(), ray_out_dir);
        }
    }
}

void PhotonMappingRender::update_nearby_vp(const Vector3f &pos, const Vector3f &attenuation) {
    ball_finder.FindAndResizeBalls(pos, [&, this](VisiblePoint *vp) -> float {
        float radius_factor = ((float) vp->num_photons * alpha + alpha) / ((float) vp->num_photons + 1);
        vp->num_photons++;
        vp->photon_flux = (vp->photon_flux + vp->attenuation * attenuation) * radius_factor;
        return vp->radius * radius_factor;
    });
}

} // namespace RT
