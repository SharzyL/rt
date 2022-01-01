#include "renderer.h"
#include "util.h"

Renderer::Renderer(int sub_pixel, int sub_sample) : sub_pixel(sub_pixel), sub_sample(sub_sample) {}

void Renderer::Render(const Object3D &obj, const Camera &camera, const std::string &output_file) {
    Image img(camera.getWidth(), camera.getHeight());

#pragma omp parallel for collapse(1) schedule(dynamic, 2) shared(camera, img, obj) default(none)
    for (int y = 0; y < camera.getHeight(); y++) {
        LOG(ERROR) << fmt::format("start tracing y = {} / {}", y + 1, camera.getHeight());
        for (int x = 0; x < camera.getWidth(); x++) {
            Vector3f color;
            for (int sx = 0; sx < sub_pixel; sx++) {
                for (int sy = 0; sy < sub_pixel; sy++) {
                    float sub_x = (float)x + (float)sx / (float)sub_pixel;
                    float sub_y = (float)y + (float)sy / (float)sub_pixel;
                    for (int s = 0; s < sub_sample; s++) {
                        Ray r = camera.generateRay(Vector2f(sub_x, sub_y));
                        color += clamp1(trace(r, obj, 0));
                        LOG(INFO) << fmt::format("cast ray ({}, {}) ({} -> {}) = {}", x, y, r.getOrigin(),
                                                 r.getDirection(), color);
                    }
                }
            }
            color = color / (float)sub_pixel / (float)sub_pixel / (float)sub_sample;
            img.SetPixel(x, y, color);
        }
    }

    img.SaveBMP(output_file.c_str());
}

Vector3f Renderer::trace(const Ray &ray, const Object3D &obj, int depth) {
    Hit hit;
    bool is_hit = obj.intersect(ray, hit, 0);
    if (!is_hit) {
        return Vector3f::ZERO;
    }
//    LOG(INFO) << fmt::format("hit ({}): {}", ray.pointAtParameter(hit.getT()), hit.getMaterial()->GetName());
    const Material *mat = hit.getMaterial();

    if (depth >= 3 && rand_float() > 1 - 0.2 * depth) {
        return mat->Ambient();
    }

    Vector3f hit_point = ray.pointAtParameter(hit.getT());

    Vector3f refl_dir = mat->Sample(ray, hit);
    Ray sample_ray = Ray(hit_point + 0.001 * refl_dir, refl_dir);
    //    Vector3f bdrf = mat->BDRF(sample_ray, ray, hit);
    Vector3f sample_ray_color = trace(sample_ray, obj, depth + 1);
    Vector3f ambient = mat->Ambient();

    return mat->Emission() + mat->Ambient() * sample_ray_color;
}
