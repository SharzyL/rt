#ifndef RT_PHOTON_MAPPING_H
#define RT_PHOTON_MAPPING_H

#include <memory>
#include <string>
#include <vector>
#include <mutex>

#include <Vector3f.h>

#include "core/hit.h"
#include "core/light.h"
#include "objects/object3d.h"
#include "utils/ball_finder.hpp"
#include "utils/scene_parser.h"

namespace RT {

class Ray;
class Camera;

struct VisiblePoint {
    VisiblePoint(): mtx(new std::mutex) {};
    Vector3f center;

    // determined in forward process
    Vector3f forward_flux;
    Vector3f attenuation;

    // determined in backward process
    Vector3f photon_flux;
    int num_photons = 0;
    float radius = -1;

    std::unique_ptr<std::mutex> mtx;
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

    std::vector<Vector3f> img_data;
    std::vector<VisiblePoint> visible_point_map;

    BallFinder<VisiblePoint> ball_finder;
};

} // namespace RT
#endif // RT_PHOTON_MAPPING_H
