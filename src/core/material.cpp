#include <tiny_obj_loader.h>

#include "utils/math_util.h"

#include "core/ray.h"
#include "core/hit.h"

#include "./material.h"

namespace RT {

float schlick(float cos, float refr_idx) {  // return rate of reflection
    float r0 = fsquare((1.f - refr_idx) / (1.f + refr_idx));
    return r0 + (1 - r0) * std::pow(1.f - cos, 5.f);
}

Material::Material(const tinyobj::material_t &mat) {
    illumination_model = static_cast<IlluminationModel>(mat.illum);

    ambientColor = mat.ambient;
    diffuseColor = mat.diffuse;
    specularColor = mat.specular;
    emissionColor = mat.emission;

    shininess = mat.shininess;
    refraction = mat.ior;

    name = mat.name;
}

float Material::BRDF(const Ray &ray_in, const Ray &ray_out, const Hit &hit) const {
    return 1.f;
}

Vector3f Material::Sample(const Ray &ray_in, const Hit &hit) const {
    const Vector3f &norm = hit.GetNormal();
    const Vector3f &dir = ray_in.GetDirection();
    float cos_ray_in = Vector3f::dot(norm, dir);
    bool norm_in_diff_side = cos_ray_in > 0;
    Vector3f ray_side_norm = norm_in_diff_side ? -norm : norm;

    switch (illumination_model) {
        case IlluminationModel::diffuse: {  // 1
            return ray_side_norm + rng.RandNormalizedVector();
        }
        case IlluminationModel::blinn: {  // 2
            return dir - 2 * norm * Vector3f::dot(norm, dir) + rng.RandNormalizedVector() * std::min(1.f, 1 / shininess);
        }
        case IlluminationModel::reflective: {  // 3
            return dir - 2 * norm * Vector3f::dot(norm, dir);
        }
        case IlluminationModel::transparent: {  // 4
            float cos_ray_in_abs = std::abs(cos_ray_in);
            float refr_idx = norm_in_diff_side ? refraction : 1.f / refraction;

            float cos2t = 1.f - fsquare(refr_idx) * (1.f - fsquare(cos_ray_in));
            if (cos2t < 0 || rng.RandUniformFloat() < schlick(cos_ray_in_abs, refr_idx)) {
                Vector3f refl_dir = dir - 2 * norm * Vector3f::dot(norm, dir);
                return refl_dir;
            } else {
                Vector3f refr_dir = refr_idx * dir - ray_side_norm * (-refr_idx * cos_ray_in_abs + std::sqrt(cos2t));
                return refr_dir;
            }
        }
        default:
            throw std::runtime_error("illum type not implemented");
    }
}

bool Material::IsDiffuse() const {
    return illumination_model == IlluminationModel::diffuse || illumination_model == IlluminationModel::blinn;
}

} // namespace RT
