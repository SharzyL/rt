#include "./material.h"
#include "util.h"

Material::Material(const tinyobj::material_t &mat) {
    illumination_model = static_cast<IlluminationModel>(mat.illum);

    ambientColor = mat.ambient;
    diffuseColor = mat.diffuse;
    specularColor = mat.specular;
    emissionColor = mat.emission;

    shininess = mat.shininess;
    refraction = mat.ior;
}

Vector3f Material::Ambient() const {
    return ambientColor;
}

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
    const Vector3f front = (dir - norm * Vector3f::dot(norm, dir)).normalized();
    const Vector3f side = Vector3f::cross(norm, front);
    float theta = rand_float() * 2 * (float) M_PI;  // random angle
    float norm_part = rand_float();          // random projection on norm
    Vector3f reflection_dir = (front * std::cos(theta) + side * std::sin(theta)) * std::sqrt(1 - norm_part);
    reflection_dir += norm * std::sqrt(norm_part);
    reflection_dir.normalize();
    return reflection_dir;
}
