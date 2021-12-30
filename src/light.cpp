#include "./light.h"

DirectionalLight::DirectionalLight(const Vector3f &d, const Vector3f &c) {
    direction = d.normalized();
    color = c;
}

void DirectionalLight::getIllumination(const Vector3f &p, Vector3f &dir, Vector3f &col) const {
    // the direction to the light is the opposite of the
    // direction of the directional light source
    dir = -direction;
    col = color;
}

PointLight::PointLight(const Vector3f &p, const Vector3f &c) {
    position = p;
    color = c;
}

void PointLight::getIllumination(const Vector3f &p, Vector3f &dir, Vector3f &col) const {
    // the direction to the light is the opposite of the
    // direction of the directional light source
    dir = (position - p).normalized();
    col = color;
}
