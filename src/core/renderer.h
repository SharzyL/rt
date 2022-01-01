#ifndef RT_RENDERER_H
#define RT_RENDERER_H

#include "core/camera.h"
#include "image.h"
#include "objects/object3d.h"

class Renderer {
public:
    Renderer(int sub_pixel, int sub_sample);

    void Render(const Object3D &obj, const Camera &camera, const std::string &output_file);

private:
    Vector3f trace(const Ray &ray, const Object3D &obj, int depth);
    int sub_pixel, sub_sample;
};

#endif // RT_RENDERER_H
