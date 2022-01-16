#include "ray.h"

namespace RT {

Ray::Ray(const Vector3f &orig, const Vector3f &dir, float time): origin(orig), direction(dir.normalized()), time(time) {};

[[nodiscard]] const Vector3f &Ray::GetOrigin() const { return origin; }

[[nodiscard]] const Vector3f &Ray::GetDirection() const { return direction; }

[[nodiscard]] Vector3f Ray::PointAtParameter(float t) const { return origin + direction * t; }

const float &Ray::GetTime() const {
    return time;
}

inline std::ostream &operator<<(std::ostream &os, const Ray &r) {
    os << "Ray <" << r.GetOrigin() << ", " << r.GetDirection() << ">";
    return os;
}

} // namespace RT