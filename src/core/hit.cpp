#include <limits>

#include "./hit.h"

Hit::Hit() {
    material = nullptr;
    t = std::numeric_limits<decltype(t)>::max();
}

Hit::Hit(float _t, Material *m, const Vector3f &n) {
    t = _t;
    material = m;
    normal = n;
}

Hit::Hit(const Hit &h) {
    t = h.t;
    material = h.material;
    normal = h.normal;
}

[[nodiscard]] float Hit::getT() const { return t; }

[[nodiscard]] const Material *Hit::getMaterial() const { return material; }

[[nodiscard]] const Vector3f &Hit::getNormal() const { return normal; }

void Hit::set(float _t, const Material *m, const Vector3f &n) {
    t = _t;
    material = m;
    normal = n;
}

inline std::ostream &operator<<(std::ostream &os, const Hit &h) {
    os << "Hit <" << h.getT() << ", " << h.getNormal() << ">";
    return os;
}
