#include "core/ray.h"
#include "core/hit.h"
#include "core/texture.h"
#include "core/material.h"

#include "./plane.h"

namespace RT {

Plane::Plane(
        const Vector3f &normal,
        float d,
        const Material *material,
        const Texture *texture,
        float texture_scale,
        const Vector2f &texture_translate,
        const Vector3f &texture_up
) : normal(normal.normalized()),
    d(d),
    SimpleObject3D(material, texture),
    texture_scale(texture_scale),
    texture_translate(texture_translate) {
    this->texture_up = (texture_up - this->normal * Vector3f::dot(this->normal, texture_up)).normalized();
    this->texture_right = Vector3f::cross(texture_up, normal);
}

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
        auto x = (Vector3f::dot(hit_point, texture_right) + texture_translate.x()) / texture_scale;
        auto y = (Vector3f::dot(hit_point, texture_up) + texture_translate.y()) / texture_scale;
        auto u = x - std::floor(x);
        auto v = y - std::floor(y);
        return texture->At(u / 2, v / 2);
    } else {
        return material->ambientColor;
    }
}

} // namespace RT
