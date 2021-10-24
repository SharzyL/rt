#ifndef PLANE_H
#define PLANE_H

#include "object3d.hpp"
#include <cmath>
#include <vecmath.h>

// function: ax+by+cz=d
// choose your representation , add more fields and fill in the functions
class Plane : public Object3D {
public:
    Plane(const Vector3f &normal, float d, Material *m) : normal(normal), d(d), Object3D(m) {}

    ~Plane() override = default;

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        float t = - (d + Vector3f::dot(r.getDirection(), normal)) / Vector3f::dot(r.getDirection(), normal);
        if (t > tmin) {
            h.set(t, material, normal);
            return true;
        } else {
            return false;
        }
    }

protected:
    const Vector3f normal;
    const float d;
};

#endif // PLANE_H
