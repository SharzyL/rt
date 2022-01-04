#ifndef RT_PHOTON_MAPPING_H
#define RT_PHOTON_MAPPING_H

#include <string>
#include <vector>

#include <Vector3f.h>

#include "core/light.h"
#include "objects/object3d.h"

namespace RT {

class Ray;

class Camera;

class PhotonMappingRender {
public:
    PhotonMappingRender(int sub_pixel, int sub_sample, float gamma);

    void Render(const Object3D &obj, const Camera &camera, const std::vector<Light> &lights,
                const std::string &output_file);

private:
    int sub_pixel, sub_sample;
    float gamma;
};

} // namespace RT
#endif // RT_PHOTON_MAPPING_H
