#include "ray.h"

Ray::Ray(const Vector3f &orig, const Vector3f &dir) {
    origin = orig;
    direction = dir.normalized();
}

Ray::Ray(const Ray &r) {
    origin = r.origin;
    direction = r.direction;
}

[[nodiscard]] const Vector3f &Ray::getOrigin() const { return origin; }

[[nodiscard]] const Vector3f &Ray::getDirection() const { return direction; }

[[nodiscard]] Vector3f Ray::pointAtParameter(float t) const { return origin + direction * t; }

inline std::ostream &operator<<(std::ostream &os, const Ray &r) {
    os << "Ray <" << r.getOrigin() << ", " << r.getDirection() << ">";
    return os;
}
