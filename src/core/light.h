#ifndef RT_LIGHT_H
#define RT_LIGHT_H

#include <Vector3f.h>

#include <core/ray.h>

namespace RT {

class RNG;

class ColoredRay : public Ray {
public:
    ColoredRay(const Ray &ray, const Vector3f &color) : Ray(ray), color(color){};
    ColoredRay(const Vector3f &orig, const Vector3f &dir, const Vector3f &color) : Ray(orig, dir), color(color){};

    [[nodiscard]] const Vector3f &GetColor() const { return color; }

private:
    Vector3f color;
};

class Light {
public:
    [[nodiscard]] virtual ColoredRay EmitRay(RNG &rng) const = 0;
    virtual ~Light() = default;
};

class PointLight : public Light {
public:
    PointLight(const Vector3f &center, const Vector3f &color);

    [[nodiscard]] ColoredRay EmitRay(RNG &rng) const override;

private:
    Vector3f center;
    Vector3f color;
};

} // namespace RT

#endif // RT_LIGHT_H
