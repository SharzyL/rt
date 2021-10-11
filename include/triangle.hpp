#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "object3d.hpp"
#include <cmath>
#include <iostream>
#include <vecmath.h>
using namespace std;

// TODO: implement this class and add more fields as necessary,
class Triangle : public Object3D {

public:
    // a b c are three vertex positions of the triangle
    Triangle(const Vector3f &a, const Vector3f &b, const Vector3f &c, Material *m) : Object3D(m) {
        // TODO: triangle intersection algorithm
    }

    bool intersect(const Ray &ray, Hit &hit, float tmin) override { return false; }
    Vector3f normal;
    Vector3f vertices[3];

protected:
};

#endif // TRIANGLE_H
