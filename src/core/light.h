#ifndef RT_LIGHT_H
#define RT_LIGHT_H

#include <Vector3f.h>

#include <core/ray.h>

namespace RT {

class Light {
public:
    [[nodiscard]] virtual Ray EmitRay() const = 0;
};

class PointLight : Light {
public:
    PointLight(const Vector3f &center, const Vector3f &color);

    [[nodiscard]] Ray EmitRay() const override;

private:
    Vector3f center;
    Vector3f color;
};

} // namespace RT

#endif // RT_LIGHT_H
