#include "renderer.h"
#include "util.h"

Renderer::Renderer(int sub_pixel, int sub_sample, float gamma) : sub_pixel(sub_pixel), sub_sample(sub_sample), gamma(gamma) {}

void Renderer::Render(const Object3D &obj, const Camera &camera, const std::string &output_file) {
    Image img(camera.getWidth(), camera.getHeight());

    int ok_pixels = 0;
    int total_pixels = camera.getWidth() * camera.getHeight();
    const int progress_per_pixel = std::max(total_pixels / 2000, 1);

#pragma omp parallel for collapse(1) schedule(dynamic, 4) shared(camera, img, obj, ok_pixels, progress_per_pixel, total_pixels) default(none)
    for (int y = 0; y < camera.getHeight(); y++) {
        for (int x = 0; x < camera.getWidth(); x++) {
            Vector3f color;
            for (int sx = 0; sx < sub_pixel; sx++) {
                for (int sy = 0; sy < sub_pixel; sy++) {
                    float sub_x = (float) x + (float) sx / (float) sub_pixel;
                    float sub_y = (float) y + (float) sy / (float) sub_pixel;
                    for (int s = 0; s < sub_sample; s++) {
                        float disturb_x = (1 + rand_float_tent()) / (float) sub_pixel / 2;
                        float disturb_y = (1 + rand_float_tent()) / (float) sub_pixel / 2;
                        Ray r = camera.generateRay(Vector2f(sub_x + disturb_x, sub_y + disturb_y));
                        color += clamp1(trace(r, obj, 0));
//                        LOG(INFO) << fmt::format("cast ray ({}, {}) ({} -> {}) = {}", x, y, r.getOrigin(),
//                                                 r.getDirection(), color);
                    }
                }
            }
            color = gamma_correct(color / (float) sub_pixel / (float) sub_pixel / (float) sub_sample, gamma);
            img.SetPixel(x, y, color);

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
    bool is_hit = obj.intersect(ray, hit, 0.00001);
    if (!is_hit) {
        return Vector3f::ZERO;
    }
//    LOG(INFO) << fmt::format("hit ({}): {}", ray.pointAtParameter(hit.getT()), hit.getMaterial()->GetName());
    const Material *mat = hit.getMaterial();

    if (depth >= 5) {
        return mat->Ambient();
    }

    Vector3f hit_point = ray.pointAtParameter(hit.getT());

    Ray sample_ray = Ray(hit_point, mat->Sample(ray, hit));
    //    Vector3f bdrf = mat->BRDF(sample_ray, ray, hit);
    Vector3f sample_ray_color = trace(sample_ray, obj, depth + 1);
    Vector3f ambient = mat->Ambient();

    return mat->Emission() + mat->Ambient() * sample_ray_color;
}
