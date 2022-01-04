#include <limits>

#include "./hit.h"
#include "objects/object3d.h"
#include "core/material.h"

namespace RT {

Hit::Hit() {
    t = std::numeric_limits<decltype(t)>::max();
    material = nullptr;
}

Hit::Hit(const Hit &h) {
    t = h.t;
    material = h.material;
    normal = h.normal;
}

[[nodiscard]] float Hit::GetT() const { return t; }

[[nodiscard]] const Material *Hit::GetMaterial() const { return material; }

[[nodiscard]] const Vector3f &Hit::GetNormal() const { return normal; }

void Hit::Set(float _t, const Material *m, const Vector3f &n, const Vector3f &hit_point, const SimpleObject3D *object) {
    t = _t;
    material = m;
    normal = n;
    texture_calculator = object;
    this->pos = hit_point;
}

Vector3f Hit::GetAmbient() const {
    if (texture_calculator) {
        return texture_calculator->AmbientColorAtHit(*this);
    } else {
        return material->ambientColor;
    }
}

const Vector3f &Hit::GetPos() const {
    return pos;
}

inline std::ostream &operator<<(std::ostream &os, const Hit &h) {
    os << "Hit <" << h.GetT() << ", " << h.GetNormal() << ">";
    return os;
}

} // namespace RT
