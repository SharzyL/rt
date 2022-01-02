#include "ray.h"

namespace RT {

Ray::Ray(const Vector3f &orig, const Vector3f &dir) {
    origin = orig;
    direction = dir.normalized();
}

Ray::Ray(const Ray &r) {
    origin = r.origin;
    direction = r.direction;
}

[[nodiscard]] const Vector3f &Ray::GetOrigin() const { return origin; }

[[nodiscard]] const Vector3f &Ray::GetDirection() const { return direction; }

[[nodiscard]] Vector3f Ray::PointAtParameter(float t) const { return origin + direction * t; }

void Ray::set(const Vector3f &orig, const Vector3f &dir) {
    this->origin = orig;
    this->direction = dir;
}

inline std::ostream &operator<<(std::ostream &os, const Ray &r) {
    os << "Ray <" << r.GetOrigin() << ", " << r.GetDirection() << ">";
    return os;
}

} // namespace RT