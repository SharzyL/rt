#include "path_tracing.h"
#include "utils/image.h"
#include "utils/math_util.h"
#include "utils/debug.h"
#include "utils/prog_bar.hpp"

#include "core/hit.h"
#include "core/material.h"
#include "core/ray.h"

namespace RT {

PathTracingRender::PathTracingRender(int sub_pixel, int sub_sample, float gamma) : sub_pixel(sub_pixel), sub_sample(sub_sample), gamma(gamma) {}

void PathTracingRender::Render(const Object3D &obj, const Camera &camera, const std::string &output_file) {
    Image img(camera.getWidth(), camera.getHeight());

    int ok_pixels = 0;
    ProgressBar bar("Path tracing", camera.getWidth() * camera.getHeight());

#pragma omp parallel for collapse(2) schedule(dynamic, 4) shared(camera, img, obj, ok_pixels, bar, rng) default(none)
    for (int y = 0; y < camera.getHeight(); y++) {
        for (int x = 0; x < camera.getWidth(); x++) {
            Vector3f pixel_color;
            for (int sx = 0; sx < sub_pixel; sx++) {
                for (int sy = 0; sy < sub_pixel; sy++) {
                    float sub_x = (float) x + (float) sx / (float) sub_pixel;
                    float sub_y = (float) y + (float) sy / (float) sub_pixel;
                    for (int s = 0; s < sub_sample; s++) {
                        float disturb_x = (1 + rng.RandTentFloat()) / (float) sub_pixel / 2;
                        float disturb_y = (1 + rng.RandTentFloat()) / (float) sub_pixel / 2;
                        Ray r = camera.generateRay(Vector2f(sub_x + disturb_x, sub_y + disturb_y));
                        Vector3f sample_color = trace(r, obj, 0);
                        pixel_color += sample_color;
//                        LOG(ERROR) << fmt::format("cast ray ({}, {}) ({} -> {}) = {}", x, y, r.GetOrigin(),
//                                                 r.GetDirection(), sample_color);
                    }
                }
            }
            pixel_color = gamma_correct(pixel_color / (float) sub_pixel / (float) sub_pixel / (float) sub_sample, gamma);
            img.SetPixel(x, y, pixel_color);
            bar.Step();
        }
    }

    img.SaveImage(output_file.c_str());
}

Vector3f PathTracingRender::trace(const Ray &ray, const Object3D &obj, int depth) {
    Hit hit;
    bool is_hit = obj.Intersect(ray, hit, 0);
    if (!is_hit) {
        return Vector3f::ZERO;
    }
    const Material *mat = hit.GetMaterial();

    if (depth >= 5) {
        return mat->emissionColor;
    }

    const Vector3f &hit_ambient = hit.GetAmbient();

    Vector3f hit_point = hit.GetPos();

    Vector3f sample_dir = mat->Sample(ray, hit);
    Ray sample_ray = Ray(hit_point + 0.0001 * sample_dir, sample_dir);
    float brdf = mat->BRDF(ray, sample_ray, hit);

    float cos_theta = std::abs(Vector3f::dot(sample_dir, hit.GetNormal()));

    Vector3f sample_ray_color = trace(sample_ray, obj, depth + 1);

    return mat->emissionColor + hit_ambient * sample_ray_color * brdf;
}

} // namespace RT
