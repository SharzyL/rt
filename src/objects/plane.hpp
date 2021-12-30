#ifndef PLANE_H
#define PLANE_H

#include "object3d.hpp"
#include "vecmath.h"
#include <cmath>

// function: ax+by+cz=d
// choose your representation , add more fields and fill in the functions
class Plane : public Object3D {
public:
    Plane(const Vector3f &normal, float d, Material *m) : normal(normal), d(d), Object3D(m) {}

    ~Plane() override = default;

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        const Vector3f &rd = r.getDirection();
        float t = (d + Vector3f::dot(r.getOrigin(), normal)) / Vector3f::dot(rd, normal);
        if (t > tmin && t < h.getT()) {
            LOG(INFO) << fmt::format("hit plane at {}", r.pointAtParameter(t));
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
