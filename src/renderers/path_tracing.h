#ifndef RT_PATH_TRACING_H
#define RT_PATH_TRACING_H

#include <string>

#include "core/camera.h"
#include "utils/scene_parser.h"
#include "objects/object3d.h"

namespace RT {

class RNG;

class PathTracingRender {
public:
    PathTracingRender(int sub_pixel, int sub_sample, const SceneParser &parser);

    void Render(const Object3D &obj, const Camera &camera, const std::string &output_file);

private:
    Vector3f trace(const Ray &ray, const Object3D &obj, int depth, RNG &rng);
    int sub_pixel, sub_sample;
    float gamma;
    Vector3f bg_color;
};

} // namespace RT

#endif // RT_PATH_TRACING_H
