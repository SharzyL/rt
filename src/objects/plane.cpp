#include "core/ray.h"
#include "core/hit.h"
#include "core/texture.h"
#include "core/material.h"

#include "./plane.h"
#include "utils/debug.h"

namespace RT {

Plane::Plane(
        const Vector3f &normal,
        float d,
        const Material *material,
        const Texture *texture,
        const Texture *normal_texture,
        float texture_scale,
        const Vector2f &texture_translate,
        const Vector3f &texture_up
) : normal(normal.normalized()),
    d(d),
    SimpleObject3D(material, texture),
    normal_texture(normal_texture),
    texture_scale(texture_scale),
    texture_translate(texture_translate) {
    this->texture_up = (texture_up - this->normal * Vector3f::dot(this->normal, texture_up)).normalized();
    this->texture_right = Vector3f::cross(texture_up, normal);
}

bool Plane::Intersect(const Ray &r, Hit &hit, float tmin) const {
    const Vector3f &dir = r.GetDirection();
    float t = (d - Vector3f::dot(r.GetOrigin(), normal)) / Vector3f::dot(dir, normal);

    if (t > tmin && t < hit.GetT()) {
        auto color = material->ambientColor;
        auto true_normal = normal;
        auto hit_point = r.PointAtParameter(t);

        // handling texture
        if (texture != nullptr || normal_texture != nullptr) {
            auto x = (Vector3f::dot(hit_point, texture_right) + texture_translate.x()) / texture_scale;
            auto y = (Vector3f::dot(hit_point, texture_up) + texture_translate.y()) / texture_scale;
            auto u = x - std::floor(x);
            auto v = y - std::floor(y);
            if (texture != nullptr) {
                color = texture->At(u, v);
            }
            if (normal_texture != nullptr) {
                auto texture_n = normal_texture->At(u, v);
                true_normal = texture_n.x() * texture_up + texture_n.y() * texture_right + texture_n.z() * normal;
//                fmt::print("true_normal: {}\n", true_normal);
            }
        }
        hit.Set(t, material, true_normal, hit_point, color, this);
        return true;
    } else {
        return false;
    }
}

} // namespace RT
