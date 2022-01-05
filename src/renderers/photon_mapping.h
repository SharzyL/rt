#ifndef RT_PHOTON_MAPPING_H
#define RT_PHOTON_MAPPING_H

#include <memory>
#include <string>
#include <vector>

#include <Vector3f.h>

#include "core/hit.h"
#include "core/light.h"
#include "objects/object3d.h"
#include "utils/ball_finder.hpp"
#include "utils/scene_parser.h"

namespace RT {

class RNG;
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

class PhotonMappingRender {
public:
    PhotonMappingRender(float alpha, float init_radius, int num_rounds, int photons_per_round,
                        int vp_per_pixel, const SceneParser &scene_parser);

    void Render(const std::string &output_file);

private:
    void trace_visible_point(VisiblePoint &vp, const Ray &ray, RNG &rng);
    void trace_photon(const ColoredRay &ray, RNG &rng);
    void update_nearby_vp(const Vector3f &pos, const Vector3f &attenuation);

    int width, height;
    const Object3D *obj;
    const Camera *camera;
    const Vector3f &bg_color;
    const std::vector<std::unique_ptr<Light>> &lights;
    float gamma;

    float alpha;
    float init_radius;
    int num_rounds;
    int photons_per_round;
    int vp_per_pixel;

    std::vector<VisiblePoint> visible_point_map;

    BallFinder<VisiblePoint> ball_finder;
};

} // namespace RT
#endif // RT_PHOTON_MAPPING_H
