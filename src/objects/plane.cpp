#include "./plane.h"

namespace RT {

Plane::Plane(const Vector3f &normal, float d, const Material *material, const Texture *texture)
        : normal(normal.normalized()), d(d), SimpleObject3D(material, texture) {}

bool Plane::Intersect(const Ray &r, Hit &h, float tmin) const {
    const Vector3f &dir = r.GetDirection();
    float t = (d - Vector3f::dot(r.GetOrigin(), normal)) / Vector3f::dot(dir, normal);
    if (t > tmin && t < h.GetT()) {
        h.Set(t, material, normal, this);
        return true;
    } else {
        return false;
    }
}

Vector3f Plane::AmbientColorAtHit(const Ray &r, const Hit &hit) const {
    if (texture != nullptr) {
        auto hit_point = r.PointAtParameter(hit.GetT());
        // TODO: improve locating method
        auto u = hit_point.x() - std::floor(hit_point.x());
        auto v = hit_point.y() - std::floor(hit_point.y());
        return texture->At(u, v);
    } else {
        return material->ambientColor;
    }
}

} // namespace RT
