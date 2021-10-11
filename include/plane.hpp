#ifndef PLANE_H
#define PLANE_H

#include "object3d.hpp"
#include <cmath>
#include <vecmath.h>

// function: ax+by+cz=d
// choose your representation , add more fields and fill in the functions
class Plane : public Object3D {
public:
    Plane(const Vector3f &normal, float d, Material *m) : Object3D(m) {
        // TODO: sphere intersection algorithm
    }

    ~Plane() override = default;

    bool intersect(const Ray &r, Hit &h, float tmin) override { return false; }

protected:
};

#endif // PLANE_H
