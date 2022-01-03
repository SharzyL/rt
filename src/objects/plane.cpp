#include "./plane.h"

namespace RT {

Plane::Plane(const Vector3f &normal, float d, const Material *material, const Texture *texture)
        : normal(normal.normalized()), d(d), SimpleObject3D(material, texture) {}

Plane::~Plane() = default;

bool Plane::Intersect(const Ray &r, Hit &h, float tmin) const {
    const Vector3f &dir = r.GetDirection();
    float t = (d - Vector3f::dot(r.GetOrigin(), normal)) / Vector3f::dot(dir, normal);
    if (t > tmin && t < h.GetT()) {
        h.Set(t, material, normal);
        if (texture != nullptr) {
            auto hit_point = r.PointAtParameter(t);
            // TODO: improve locating method
            auto u = hit_point.x() - std::floor(hit_point.x());
            auto v = hit_point.y() - std::floor(hit_point.y());
            h.SetTextureColor(texture->At(u, v));
        }
        return true;
    } else {
        return false;
    }
}

} // namespace RT
