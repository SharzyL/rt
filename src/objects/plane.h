#ifndef PLANE_H
#define PLANE_H

#include <cmath>

#include "vecmath.h"

#include "objects/object3d.h"

// function: ax+by+cz=d
// choose your representation , add more fields and fill in the functions
class Plane : public Object3D {
public:
    Plane(const Vector3f &normal, float d, Material *m);

    ~Plane() override;

    bool intersect(const Ray &r, Hit &h, float tmin) override;

protected:
    const Vector3f normal;
    const float d;
};

#endif // PLANE_H
