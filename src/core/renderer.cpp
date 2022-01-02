#include "renderer.h"
#include "image.h"
#include "util.h"

namespace RT {

Renderer::Renderer(int sub_pixel, int sub_sample, float gamma) : sub_pixel(sub_pixel), sub_sample(sub_sample), gamma(gamma) {}

void Renderer::Render(const Object3D &obj, const Camera &camera, const std::string &output_file) {
    Image img(camera.getWidth(), camera.getHeight());

    int ok_pixels = 0;
    int total_pixels = camera.getWidth() * camera.getHeight();
    const int progress_per_pixel = std::max(total_pixels / 2000, 1);

#pragma omp parallel for collapse(2) schedule(dynamic, 4) shared(camera, img, obj, ok_pixels, progress_per_pixel, total_pixels) default(none)
    for (int y = 0; y < camera.getHeight(); y++) {
        for (int x = 0; x < camera.getWidth(); x++) {
            Vector3f pixel_color;
            for (int sx = 0; sx < sub_pixel; sx++) {
                for (int sy = 0; sy < sub_pixel; sy++) {
                    float sub_x = (float) x + (float) sx / (float) sub_pixel;
                    float sub_y = (float) y + (float) sy / (float) sub_pixel;
                    for (int s = 0; s < sub_sample; s++) {
                        float disturb_x = (1 + rand_float_tent()) / (float) sub_pixel / 2;
                        float disturb_y = (1 + rand_float_tent()) / (float) sub_pixel / 2;
                        Ray r = camera.generateRay(Vector2f(sub_x + disturb_x, sub_y + disturb_y));
                        Vector3f sample_color = trace(r, obj, 0);
                        pixel_color += clamp1(sample_color);
//                        LOG(ERROR) << fmt::format("cast ray ({}, {}) ({} -> {}) = {}", x, y, r.GetOrigin(),
//                                                 r.GetDirection(), sample_color);
                    }
                }
            }
            pixel_color = gamma_correct(pixel_color / (float) sub_pixel / (float) sub_pixel / (float) sub_sample, gamma);
            img.SetPixel(x, y, pixel_color);

            // report progress
# pragma omp critical
            ok_pixels += 1;
            if (ok_pixels % progress_per_pixel == 0) {
                float complete_rate = (float) ok_pixels * 100.f / (float) total_pixels;
                LOG(ERROR) << fmt::format("complete {:.2f}% ({}/{} pixels)", complete_rate, ok_pixels, total_pixels);
            }
        }
    }

    img.SaveBMP(output_file.c_str());
}

Vector3f Renderer::trace(const Ray &ray, const Object3D &obj, int depth) {
    Hit hit;
    bool is_hit = obj.Intersect(ray, hit, 0.00001);
    if (!is_hit) {
        return Vector3f::ZERO;
    }
//    LOG(INFO) << fmt::format("hit ({}): {}", ray.PointAtParameter(hit.GetT()), hit.GetMaterial()->GetName());
    const Material *mat = hit.GetMaterial();

    if (depth >= 5) {
        return mat->Ambient();
    }

    Vector3f hit_point = ray.PointAtParameter(hit.GetT());

    Ray sample_ray = Ray(hit_point, mat->Sample(ray, hit));
    //    Vector3f bdrf = mat->BRDF(sample_ray, ray, hit);
    Vector3f sample_ray_color = trace(sample_ray, obj, depth + 1);
    Vector3f ambient = mat->Ambient();

    return mat->Emission() + mat->Ambient() * sample_ray_color;
}

} // namespace RT
