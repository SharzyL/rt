#include <cassert>

#include "./material.h"
#include "util.h"

namespace RT {

Vector3f random_normalized_vector() {
    return Vector3f{rng.RandNormalFloat(), rng.RandNormalFloat(), rng.RandNormalFloat()}.normalized();
}

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

Vector3f Material::BRDF(const Ray &ray_in, const Ray &ray_out, const Hit &hit) const {
    assert(illumination_model == IlluminationModel::blinn);

    const Vector3f &norm = hit.GetNormal();
    const Vector3f &ray_out_dir = -ray_out.GetDirection().normalized();
    const Vector3f &ray_in_dir = ray_in.GetDirection().normalized();
    Vector3f reflection = 2 * Vector3f::dot(norm, ray_in_dir) * norm - ray_in_dir;
    Vector3f diffuse = diffuseColor * clamp(Vector3f::dot(ray_in_dir, norm));
    Vector3f specular = specularColor * std::pow(clamp(Vector3f::dot(ray_out_dir, reflection)), shininess);
    return diffuse + specular;
}

Vector3f Material::Sample(const Ray &ray_in, const Hit &hit) const {
    const Vector3f &norm = hit.GetNormal();
    const Vector3f &dir = ray_in.GetDirection();
    float cos_ray_in = Vector3f::dot(norm, dir);
    bool norm_in_diff_side = cos_ray_in > 0;
    Vector3f ray_side_norm = norm_in_diff_side ? -norm : norm;

    switch (illumination_model) {
        case IlluminationModel::diffuse: {  // 1
            Vector3f rand_vec = random_normalized_vector();
            if (Vector3f::dot(rand_vec, ray_side_norm) < 0) rand_vec = -rand_vec;
            return rand_vec;
        }
        case IlluminationModel::blinn: {  // 2
            const Vector3f front = (dir - norm * Vector3f::dot(norm, dir)).normalized();
            const Vector3f side = Vector3f::cross(norm, front);
            float theta = rng.RandUniformFloat() * (float)M_PI * 2; // random angle
            float norm_part = rng.RandUniformFloat();             // random projection on norm
            Vector3f reflection_dir = (front * std::cos(theta) + side * std::sin(theta)) * std::sqrt(1 - norm_part);
            reflection_dir += norm * std::sqrt(norm_part);
            return reflection_dir;
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

} // namespace RT
