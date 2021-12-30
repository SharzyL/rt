#include "./plane.h"

Plane::Plane(const Vector3f &normal, float d, Material *m) : normal(normal), d(d), Object3D(m) {}

Plane::~Plane() = default;

bool Plane::intersect(const Ray &r, Hit &h, float tmin) {
    const Vector3f &rd = r.getDirection();
    float t = (d + Vector3f::dot(r.getOrigin(), normal)) / Vector3f::dot(rd, normal);
    if (t > tmin && t < h.getT()) {
        LOG(INFO) << fmt::format("hit plane at {}", r.pointAtParameter(t));
        h.set(t, material, normal);
        return true;
    } else {
        return false;
    }
}
