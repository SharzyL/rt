#include "utils/math_util.h"

#include "./light.h"

namespace RT {

ColoredRay PointLight::EmitRay(RNG &rng) const {
    Vector3f dir = rng.RandNormalizedVector();
    return {center, dir, color};
}

PointLight::PointLight(const Vector3f &center, const Vector3f &color): center(center), color(color) {}

} // namespace RT