#include <limits>

#include "./hit.h"
#include "core/material.h"

#include "debug.h"

namespace RT {

Hit::Hit() {
    t = std::numeric_limits<decltype(t)>::max();
    material = nullptr;
}

Hit::Hit(float _t, const Material *m, const Vector3f &n) {
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

void Hit::Set(float _t, const Material *m, const Vector3f &n) {
    t = _t;
    material = m;
    normal = n;
}

const Vector3f &Hit::GetAmbient() const {
    if (ambient.has_value()) {
        return ambient.value();
    } else {
        return material->ambientColor;
    }
}

void Hit::SetTextureColor(const Vector3f &color) {
    ambient = color;
}

void Hit::SetTextureCalculator(const SimpleObject3D *object) {
    texture_calculator = object;
}

inline std::ostream &operator<<(std::ostream &os, const Hit &h) {
    os << "Hit <" << h.GetT() << ", " << h.GetNormal() << ">";
    return os;
}

} // namespace RT
