#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include <iostream>
#include <optional>

#include "tiny_obj_loader.h"

#include "vecmath.h"

#include "debug.h"
#include "hit.h"
#include "ray.h"

class Material {
public:
    enum class IlluminationModel {
        constant = 0,      // Kd color
        diffuse,           // Lambertian shading
        blinn,             // actually Blinn-Phong diffuse and specular combined
        reflective,        // Blinn-Phong plus reflection
        transparent,       // Blinn-Phong plus transparency
        fresnelReflection, // Blinn-Phong plus Fresnel reflection
        transparentNoReflection,
        transparentReflection,
        reflectionNoRayTrace,
        transparentNoRayTrace,
        castShadows
    };

    explicit Material(const tinyobj::material_t &mat);

    virtual ~Material() = default;

    [[nodiscard]] Vector3f Sample(const Ray &ray_in, const Hit &hit) const;
    [[nodiscard]] std::optional<Vector3f> SampleRefraction(const Ray &ray_in, const Hit &hit) const;

    [[nodiscard]] bool HasRefraction() const;
    [[nodiscard]] Vector3f BDRF(const Ray &ray_in, const Ray &ray_out, const Hit &hit) const;

    [[nodiscard]] Vector3f Ambient() const;
    [[nodiscard]] Vector3f Emission() const;

    [[nodiscard]] const std::string &GetName() const;

protected:
    IlluminationModel illumination_model;

    Vector3f ambientColor;  // Ka
    Vector3f diffuseColor;  // Kd
    Vector3f specularColor; // Ks
    Vector3f emissionColor; // Ke
    float shininess;        // Ns
    float refraction;       // Ni
    std::string name;
};

#endif // MATERIAL_H
