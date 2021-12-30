#include "./material.h"

Material::Material(const Vector3f &d_color, const Vector3f &s_color, float s)
    : diffuseColor(d_color), specularColor(s_color), shininess(s) {}

Vector3f Material::getDiffuseColor() const { return diffuseColor; }

Vector3f Material::Shade(const Ray &ray, const Hit &hit, const Vector3f &dirToLight, const Vector3f &lightColor) {
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
