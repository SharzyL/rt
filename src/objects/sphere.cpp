#include <cmath>

#include "./sphere.h"

namespace RT {

Sphere::Sphere(const Vector3f &center, float radius, const Material *material)
    : SimpleObject3D(material), center(center), radius(radius) {}

Sphere::~Sphere() = default;

bool Sphere::Intersect(const Ray &r, Hit &h, float tmin) const {
    const Vector3f &rd = r.GetDirection();
    auto origin_to_center = center - r.GetOrigin();
    float tp = Vector3f::dot(origin_to_center, rd);
    float dist_to_center = origin_to_center.length();
    float dist = std::sqrt(dist_to_center * dist_to_center - tp * tp);

    if (dist > radius) {
        return false;
    } else {
        float t_prime = std::sqrt(radius * radius - dist * dist);
        float t = dist_to_center > radius ? tp - t_prime : tp + t_prime;
        if (t >= tmin && t < h.GetT()) {
            Vector3f intersection = r.PointAtParameter(t);
            Vector3f normal_at_intersection = (intersection - center).normalized();
            h.set(t, material, normal_at_intersection);
            return true;
        } else {
            return false;
        }
    }
}

} // namespace RT
