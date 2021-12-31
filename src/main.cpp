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
    if (depth >= 5 && rand_float() < 1.f - (float) depth * 0.1f) {
        return Vector3f::ZERO;
    }

    Hit hit;
    bool is_hit = obj.intersect(ray, hit, 0);
    if (!is_hit) return Vector3f::ZERO;
    const Material *mat = hit.getMaterial();
    Vector3f hit_point = ray.pointAtParameter(hit.getT());

    Ray sample_ray = Ray(hit_point, mat->Sample(ray, hit));
    Vector3f bdrf = mat->BDRF(sample_ray, ray, hit);
    Vector3f sample_ray_color = trace(sample_ray, obj, depth + 1);
    Vector3f ambient = mat->Ambient();

    return ambient + bdrf * sample_ray_color;
}

int main(int argc, char *argv[]) {
    google::InitGoogleLogging(argv[0]);

    PerspectiveCamera camera = PerspectiveCamera(Vector3f(0, 0, 0), Vector3f(1, 0, 0), Vector3f(0, 1, 0), 1024, 768,
                                                 40.f);
    Group g("testcases/CornellBox-Original.obj", "testcases");
    Image img(camera.getWidth(), camera.getHeight());
    int w = camera.getWidth(), h = camera.getHeight();
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            Ray r = camera.generateRay(Vector2f((float) x, (float) y));
            Vector3f color = trace(r, g, 0);
            LOG(INFO) << fmt::format("cast ray ({}, {}) = {}", x, y, color);
            img.SetPixel(x, y, color);
        }
    }
    img.SaveBMP("/tmp/output.bmp");
    return 0;
}
