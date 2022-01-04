#ifndef RT_PHOTON_MAPPING_H
#define RT_PHOTON_MAPPING_H

#include <string>
#include <vector>
#include <memory>

#include <Vector3f.h>

#include "core/light.h"
#include "core/hit.h"
#include "objects/object3d.h"
#include "utils/ball_finder.hpp"
#include "utils/scene_parser.h"

namespace RT {

class Ray;

class Camera;

struct VisiblePoint {
    Vector3f center;
    float radius;

    Vector3f photon_flux;
    Vector3f forward_flux;
    Vector3f attenuation;

    int num_photons = 0;
};

struct Photon {
    Vector3f center;
};

class PhotonMappingRender {
public:
    PhotonMappingRender(
            float gamma,
            float alpha,
            const SceneParser &scene_parser
    );

    void Render(const std::string &output_file);

private:
    void trace_visible_point(VisiblePoint &vp, const Ray &ray);
    void trace_photon(Photon &photon, const Ray &ray);
    void update_nearby_vp(const Photon &photon, const Vector3f &attenuation);

    int width, height;
    const Object3D *obj;
    const Camera *camera;
    const Vector3f &bg_color;
    const std::vector<std::unique_ptr<Light>> &lights;
    float gamma;
    float alpha;

    std::vector<VisiblePoint> visible_point_map;

    BallFinder<VisiblePoint> ball_finder;
};

} // namespace RT
#endif // RT_PHOTON_MAPPING_H
