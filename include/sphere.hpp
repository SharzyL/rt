#ifndef SPHERE_H
#define SPHERE_H

#include "object3d.hpp"
#include <cmath>
#include <vecmath.h>

class Sphere : public Object3D {
public:
    Sphere(const Vector3f &center, float radius, Material *material)
        : Object3D(material), center(center), radius(radius) {}

    ~Sphere() override = default;

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        // TODO: sphere intersection algorithm
        return false;
    }

protected:
    const Vector3f center;
    const float radius;
};

#endif
