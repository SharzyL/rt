#ifndef MATERIAL_H
#define MATERIAL_H

#include <tiny_obj_loader.h>

#include "vecmath.h"

#include "debug.h"
#include "hit.h"
#include "ray.h"

namespace RT {

class Material {
public:
    enum class IlluminationModel {
        diffuse = 1,           // random reflection
        blinn = 2,             // random reflection (to be improved)
        reflective = 3,        // total reflective
        transparent = 4,       // refractive
        fresnelReflection,
        transparentNoReflection,
        transparentReflection,
        reflectionNoRayTrace,
        transparentNoRayTrace,
        castShadows
    };

    explicit Material(const tinyobj::material_t &mat);

    virtual ~Material() = default;

    [[nodiscard]] Vector3f Sample(const Ray &ray_in, const Hit &hit) const;

    [[nodiscard]] Vector3f BRDF(const Ray &ray_in, const Ray &ray_out, const Hit &hit) const;

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

} // namespace RT
#endif // MATERIAL_H
