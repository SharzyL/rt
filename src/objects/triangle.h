#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vecmath.h"

#include "objects/object3d.h"

namespace RT {

class Triangle : public SimpleObject3D {

public:
    // a b c are three get_v positions of the triangle
    Triangle(const Vector3f &a, const Vector3f &b, const Vector3f &c, const Material *m);

    bool Intersect(const Ray &r, Hit &h, float tmin) const override;

    [[nodiscard]] Vector3f AmbientColorAtHit(const Hit &hit) const override;

    Vector3f normal;
    Vector3f a, b, c;
};

} // namespace RT

#endif // TRIANGLE_H
