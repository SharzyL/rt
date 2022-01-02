#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "vecmath.h"

#include "objects/object3d.h"

// transforms a 3D point using a matrix, returning a 3D point
static Vector3f transformPoint(const Matrix4f &mat, const Vector3f &point);

// transform a 3D direction using a matrix, returning a direction
static Vector3f transformDirection(const Matrix4f &mat, const Vector3f &dir);

class Transform : public Object3D {
public:
    Transform(const Matrix4f &m, Object3D *obj);

    bool Intersect(const Ray &r, Hit &h, float tmin) const override;

protected:
    Object3D *o; // un-transformed object
    Matrix4f transform;
};

#endif // TRANSFORM_H
