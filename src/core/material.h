#ifndef RT_MATERIAL_H
#define RT_MATERIAL_H

#include <Vector3f.h>

#include "utils/math_util.h"
#include "core/hit.h"
#include "core/ray.h"

namespace tinyobj {
class material_t;
}

namespace RT {

class Hit;

class Material {
public:
    enum class IlluminationModel {
        diffuse = 1,     // random reflection
        blinn = 2,       // random reflection (to be improved)
        reflective = 3,  // total reflective
        transparent = 4, // refractive
        fresnelReflection,
        transparentNoReflection,
        transparentReflection,
        reflectionNoRayTrace,
        transparentNoRayTrace,
        castShadows
    };

    explicit Material(IlluminationModel model) : illumination_model(model){};

    explicit Material(const tinyobj::material_t &mat);

    virtual ~Material() = default;

    // energy conservation: \int_{x on sphere} PDF(x) * BRDF(x) d x = 1
    [[nodiscard]] Vector3f Sample(const Ray &ray_in, const Hit &hit, RNG &rng) const;
    [[nodiscard]] float BRDF(const Ray &ray_in, const Ray &ray_out, const Hit &hit) const;
    [[nodiscard]] bool IsDiffuse() const;

    IlluminationModel illumination_model;

    Vector3f ambientColor;  // Ka
    Vector3f diffuseColor;  // Kd
    Vector3f specularColor; // Ks
    Vector3f emissionColor; // Ke
    float shininess = 1;    // Ns
    float refraction = 1;   // Ni
    std::string name;
};

} // namespace RT
#endif // RT_MATERIAL_H
