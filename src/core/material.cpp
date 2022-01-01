#include "./material.h"
#include "util.h"

Vector3f random_normalized_vector() {
    return Vector3f{rand_float(), rand_float(), rand_float()}.normalized();
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

Vector3f Material::Ambient() const { return ambientColor; }

Vector3f Material::BDRF(const Ray &ray_in, const Ray &ray_out, const Hit &hit) const {
    assert(illumination_model == IlluminationModel::blinn);

    const Vector3f &norm = hit.getNormal();
    const Vector3f &ray_out_dir = -ray_out.getDirection().normalized();
    const Vector3f &ray_in_dir = ray_in.getDirection().normalized();
    Vector3f reflection = 2 * Vector3f::dot(norm, ray_in_dir) * norm - ray_in_dir;
    Vector3f diffuse = diffuseColor * clamp(Vector3f::dot(ray_in_dir, norm));
    Vector3f specular = specularColor * std::pow(clamp(Vector3f::dot(ray_out_dir, reflection)), shininess);
    return diffuse + specular;
}

Vector3f Material::Sample(const Ray &ray_in, const Hit &hit) const {
    const Vector3f &norm = hit.getNormal();
    const Vector3f &dir = ray_in.getDirection();

    switch (illumination_model) {
        case IlluminationModel::diffuse: {
            Vector3f rand_vec = random_normalized_vector();
            if (Vector3f::dot(rand_vec, norm) < 0) rand_vec = -rand_vec;
            return rand_vec;
        }
        case IlluminationModel::blinn: {
            const Vector3f front = (dir - norm * Vector3f::dot(norm, dir)).normalized();
            const Vector3f side = Vector3f::cross(norm, front);
            float theta = rand_float() * (float)M_PI * 2; // random angle
            float norm_part = rand_float();             // random projection on norm
            Vector3f reflection_dir = (front * std::cos(theta) + side * std::sin(theta)) * std::sqrt(1 - norm_part);
            reflection_dir += norm * std::sqrt(norm_part);
            return reflection_dir;
        }
        case IlluminationModel::reflective: {
            return dir - 2 * norm * Vector3f::dot(norm, dir);
        }
        default:
            throw std::runtime_error("Not implement illum type");
    }
}

const std::string &Material::GetName() const { return name; }

Vector3f Material::Emission() const { return emissionColor; }