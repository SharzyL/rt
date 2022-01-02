#include "./plane.h"

namespace RT {

Plane::Plane(const Vector3f &normal, float d, const Material *m) : normal(normal), d(d), Object3D(m) {}

Plane::~Plane() = default;

bool Plane::Intersect(const Ray &r, Hit &h, float tmin) const {
    const Vector3f &rd = r.GetDirection();
    float t = (d + Vector3f::dot(r.GetOrigin(), normal)) / Vector3f::dot(rd, normal);
    if (t > tmin && t < h.GetT()) {
        LOG(INFO) << fmt::format("hit plane at {}", r.PointAtParameter(t));
        h.set(t, material, normal);
        return true;
    } else {
        return false;
    }
}

} // namespace RT
