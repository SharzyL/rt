#ifndef PLANE_H
#define PLANE_H

#include "vecmath.h"

#include "objects/object3d.h"

namespace RT {

class Plane : public SimpleObject3D {
public:
    // plane: n * v  = d
    Plane(const Vector3f &normal, float d, const Material *m, const Texture *texture = nullptr);

    bool Intersect(const Ray &r, Hit &h, float tmin) const override;

    [[nodiscard]] Vector3f AmbientColorAtHit(const Hit &hit) const override;

protected:
    const Vector3f normal;
    const float d;
};

} // namespace RT
#endif // PLANE_H
