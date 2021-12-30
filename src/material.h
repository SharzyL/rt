#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include <iostream>

#include "vecmath.h"

#include "debug.h"
#include "hit.h"
#include "ray.h"

static float clamp(float x) { return x >= 0 ? x : 0; }

class Material {
public:
    explicit Material(const Vector3f &d_color, const Vector3f &s_color = Vector3f::ZERO, float s = 0);

    virtual ~Material() = default;

    [[nodiscard]] virtual Vector3f getDiffuseColor() const;

    Vector3f Shade(const Ray &ray, const Hit &hit, const Vector3f &dirToLight, const Vector3f &lightColor);

protected:
    Vector3f diffuseColor;
    Vector3f specularColor;
    float shininess;
};

#endif // MATERIAL_H
