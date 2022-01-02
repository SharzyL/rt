#include <limits>

#include "./hit.h"

namespace RT {

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

[[nodiscard]] float Hit::GetT() const { return t; }

[[nodiscard]] const Material *Hit::GetMaterial() const { return material; }

[[nodiscard]] const Vector3f &Hit::GetNormal() const { return normal; }

void Hit::set(float _t, const Material *m, const Vector3f &n) {
    t = _t;
    material = m;
    normal = n;
}

inline std::ostream &operator<<(std::ostream &os, const Hit &h) {
    os << "Hit <" << h.GetT() << ", " << h.GetNormal() << ">";
    return os;
}

} // namespace RT
