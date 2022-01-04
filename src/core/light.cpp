#include "light.h"

namespace RT {

Ray PointLight::EmitRay() const {
    // TODO: emit ray for it
}

PointLight::PointLight(const Vector3f &center, const Vector3f &color): center(center), color(color) {}

} // namespace RT