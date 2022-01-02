#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <cmath>
#include <iostream>

#include "vecmath.h"

#include "objects/object3d.h"

class Triangle : public Object3D {

public:
    // a b c are three get_v positions of the triangle
    Triangle(const Vector3f &a, const Vector3f &b, const Vector3f &c, const Material *m);

    bool Intersect(const Ray &r, Hit &h, float tmin) const override;

    Vector3f normal;
    Vector3f a, b, c;

protected:
};

#endif // TRIANGLE_H
