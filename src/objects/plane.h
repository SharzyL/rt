#ifndef PLANE_H
#define PLANE_H

#include "vecmath.h"

#include "objects/object3d.h"

namespace RT {

class Plane : public SimpleObject3D {
public:
    // plane: n * v  = d
    Plane(
            const Vector3f &normal,
            float d,
            const Material *material,
            const Texture *texture,
            float texture_scale = 1.f,
            const Vector2f &texture_translate = Vector2f(0, 0),
            const Vector3f &texture_up = Vector3f(0, 1, 0)
    );

    bool Intersect(const Ray &r, Hit &h, float tmin) const override;

    [[nodiscard]] Vector3f AmbientColorAtHit(const Hit &hit) const override;

protected:
    const Vector3f normal;
    const float d;

    float texture_scale;
    Vector2f texture_translate;
    Vector3f texture_up;
    Vector3f texture_right;
};

} // namespace RT
#endif // PLANE_H
