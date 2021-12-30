#include <cmath>

#include "./sphere.h"

Sphere::Sphere(const Vector3f &center, float radius, Material *material)
    : Object3D(material), center(center), radius(radius) {}

Sphere::~Sphere() = default;

bool Sphere::intersect(const Ray &r, Hit &h, float tmin) {
    const Vector3f &rd = r.getDirection();
    auto origin_to_center = center - r.getOrigin();
    float tp = Vector3f::dot(origin_to_center, rd);
    float dist_to_center = origin_to_center.length();
    float dist = std::sqrt(dist_to_center * dist_to_center - tp * tp);

    if (dist > radius) {
        return false;
    } else {
        float t_prime = sqrt(radius * radius - dist * dist);
        float t = dist_to_center > radius ? tp - t_prime : tp + t_prime;
        if (t >= tmin && t < h.getT()) {
            Vector3f intersection = r.pointAtParameter(t);
            Vector3f normal_at_intersection = (intersection - center).normalized();
            h.set(t, material, normal_at_intersection);
            return true;
        } else {
            return false;
        }
    }
}
