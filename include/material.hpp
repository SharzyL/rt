#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include <vecmath.h>

#include "hit.hpp"
#include "ray.hpp"
#include <iostream>

static float clamp(float x) {
    return x >= 0 ? x : 0;
}

class Material {
public:
    explicit Material(const Vector3f &d_color, const Vector3f &s_color = Vector3f::ZERO, float s = 0)
        : diffuseColor(d_color), specularColor(s_color), shininess(s) {}

    virtual ~Material() = default;

    virtual Vector3f getDiffuseColor() const { return diffuseColor; }

    Vector3f Shade(const Ray &ray, const Hit &hit, const Vector3f &dirToLight, const Vector3f &lightColor) {
        Vector3f shaded = Vector3f::ZERO;
        const Vector3f &normal = hit.getNormal();
        const Vector3f &toRay = -ray.getDirection().normalized();
        Vector3f reflection = 2 * Vector3f::dot(normal, dirToLight) * normal - dirToLight;
        Vector3f diffuse = diffuseColor * clamp(Vector3f::dot(dirToLight, normal));
        Vector3f specular = shaded += specularColor * std::pow(clamp(Vector3f::dot(toRay, reflection)), shininess);
//        LOG(INFO) << fmt::format("diffuse {}, specular {}", diffuse, specular);
        LOG(INFO) << fmt::format("li {}, normal {}, toRay {}, reflection {}", dirToLight, normal, toRay, reflection);
        return (diffuse + specular) * lightColor;
    }

protected:
    Vector3f diffuseColor;
    Vector3f specularColor;
    float shininess;
};

#endif // MATERIAL_H
