#ifndef SPHERE_H
#define SPHERE_H

#include "vecmath.h"

#include "objects/object3d.h"

class Sphere : public Object3D {
public:
    Sphere(const Vector3f &center, float radius, Material *material);

    ~Sphere() override;

    bool intersect(const Ray &r, Hit &h, float tmin) override;

protected:
    const Vector3f center;
    const float radius;
};

#endif
