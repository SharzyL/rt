#include "utils/math_util.h"

#include "./light.h"

namespace RT {

ColoredRay PointLight::EmitRay(RNG &rng) const {
    Vector3f dir = rng.RandNormalizedVector();
    return {center, dir, color, 0};
}

PointLight::PointLight(const Vector3f &center, const Vector3f &color): center(center), color(color) {}

SphereLight::SphereLight(const Vector3f &center, float radius, const Vector3f &color): center(center), radius(radius), color(color) {}

ColoredRay SphereLight::EmitRay(RNG &rng) const {
    Vector3f dir = rng.RandNormalizedVector();
    Vector3f orig = center + radius * rng.RandNormalizedVector();
    return {orig, dir, color, 0};
}

} // namespace RT