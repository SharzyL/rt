#ifndef SPHERE_H
#define SPHERE_H

#include "vecmath.h"

#include "objects/object3d.h"

namespace RT {

class Sphere : public SimpleObject3D {
public:
    Sphere(const Vector3f &center, float radius, const Material *material);

    ~Sphere() override;

    bool Intersect(const Ray &r, Hit &h, float tmin) const override;

    const Vector3f center;
    const float radius;
};

} // namespace RT

#endif
