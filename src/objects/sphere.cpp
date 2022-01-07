#include <cmath>

#include "core/ray.h"
#include "core/hit.h"
#include "core/texture.h"
#include "core/material.h"

#include "./sphere.h"

namespace RT {

Sphere::Sphere(const Vector3f &center, float radius, const Material *material, const Texture *texture)
    : SimpleObject3D(material, texture), center(center), radius(radius) {
    box.AddVertex(Vector3f(center.x() + radius, center.y() + radius, center.z() + radius));
    box.AddVertex(Vector3f(center.x() - radius, center.y() - radius, center.z() - radius));
}

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
            h.Set(t, material, normal_at_intersection, intersection, this);
            return true;
        } else {
            return false;
        }
    }
}

Vector3f Sphere::AmbientColorAtHit(const Hit &hit) const {
    if (texture != nullptr) {
        const auto &hit_point = hit.GetPos();
        Vector3f center_to_intersection = (hit_point - center).normalized();
        float u = std::atan2(center_to_intersection.z(), center_to_intersection.x()) / (float) M_PI / 2.f + 0.5f;
        float v = std::asin(center_to_intersection.y()) / (float) M_PI + 0.5f;
        return texture->At(u, v);
    } else {
        return material->ambientColor;
    }
}

} // namespace RT
