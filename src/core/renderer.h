#ifndef RT_RENDERER_H
#define RT_RENDERER_H

#include <string>

#include "core/camera.h"
#include "objects/object3d.h"

namespace RT {

class Renderer {
public:
    Renderer(int sub_pixel, int sub_sample, float gamma);

    void Render(const Object3D &obj, const Camera &camera, const std::string &output_file);

private:
    Vector3f trace(const Ray &ray, const Object3D &obj, int depth);
    int sub_pixel, sub_sample;
    float gamma;
};

} // namespace RT

#endif // RT_RENDERER_H
