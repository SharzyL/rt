#ifndef RT_LIGHT_H
#define RT_LIGHT_H

#include <Vector3f.h>

namespace RT {

class Photon {};

class Light {
    [[nodiscard]] virtual Photon EmitPhoton() const = 0;
};

class PointLight : Light {

    PointLight(const Vector3f &center, const Vector3f &color) : Light() {}

    [[nodiscard]] Photon EmitPhoton() const override;

private:
    Vector3f center;
    Vector3f color;
};

} // namespace RT

#endif // RT_LIGHT_H
