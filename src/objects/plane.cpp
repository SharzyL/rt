#include "core/ray.h"
#include "core/hit.h"
#include "core/texture.h"
#include "core/material.h"

#include "./plane.h"

namespace RT {

Plane::Plane(const Vector3f &normal, float d, const Material *material, const Texture *texture)
        : normal(normal.normalized()), d(d), SimpleObject3D(material, texture) {}

bool Plane::Intersect(const Ray &r, Hit &h, float tmin) const {
    const Vector3f &dir = r.GetDirection();
    float t = (d - Vector3f::dot(r.GetOrigin(), normal)) / Vector3f::dot(dir, normal);
    if (t > tmin && t < h.GetT()) {
        h.Set(t, material, normal, r.PointAtParameter(t), this);
        return true;
    } else {
        return false;
    }
}

Vector3f Plane::AmbientColorAtHit(const Hit &hit) const {
    if (texture != nullptr) {
        const auto &hit_point = hit.GetPos();
        // TODO: improve locating method
        float scale = 2;
        auto x = hit_point.x() / scale;
        auto y = hit_point.y() / scale;
        auto u = x - std::floor(x);
        auto v = y - std::floor(y);
        return texture->At(u / 2, v / 2);
    } else {
        return material->ambientColor;
    }
}

} // namespace RT
