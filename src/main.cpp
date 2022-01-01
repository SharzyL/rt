#include <iostream>
#include <random>

#include "camera.h"
#include "debug.h"
#include "image.h"
#include "light.h"
#include "util.h"

#include "objects/group.h"

using namespace std;

Vector3f trace(const Ray &ray, const Object3D &obj, int depth) {
    Hit hit;
    bool is_hit = obj.intersect(ray, hit, 0);
    if (!is_hit) return Vector3f::ZERO;
    LOG(INFO) << fmt::format("hit ({}): {}", ray.pointAtParameter(hit.getT()), hit.getMaterial()->GetName());
    const Material *mat = hit.getMaterial();

    if (depth >= 5 && rand_float() > 1.f - (float) depth * 0.1f) {
        return mat->Ambient();
    }

    Vector3f hit_point = ray.pointAtParameter(hit.getT());

    Ray sample_ray = Ray(hit_point, mat->Sample(ray, hit));
    Vector3f bdrf = mat->BDRF(sample_ray, ray, hit);
    Vector3f sample_ray_color = trace(sample_ray, obj, depth + 1);
    Vector3f ambient = mat->Ambient();

    return ambient + bdrf * sample_ray_color;
}

int main(int argc, char *argv[]) {
    google::InitGoogleLogging(argv[0]);

    PerspectiveCamera camera = PerspectiveCamera(Vector3f(0, 1.3, 3), Vector3f(0, -0.05, -1), Vector3f(0, 1, 0), 1024, 768,
                                                 to_radian(60.f));
    Group g("testcases/CornellBox-Original.obj", "testcases");

    Image img(camera.getWidth(), camera.getHeight());
    const int w = camera.getWidth(), h = camera.getHeight();

#pragma omp parallel for schedule(dynamic, 1) default(none) shared(h, w, camera, img, g)
    for (int y = 0; y < h; y++) {
        LOG(ERROR) << fmt::format("start tracing y = {} / {}", y, h);
        for (int x = 0; x < w; x++) {
            Ray r = camera.generateRay(Vector2f((float) x, (float) y));
            Vector3f color = trace(r, g, 0);
            LOG(INFO) << fmt::format("cast ray ({}, {}) ({} -> {}) = {}", x, y, r.getOrigin(), r.getDirection(), color);
            img.SetPixel(x, y, color);
        }
    }

    img.SaveBMP("/tmp/output.bmp");
    return 0;
}
