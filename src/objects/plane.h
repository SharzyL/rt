#ifndef PLANE_H
#define PLANE_H

#include <cmath>
#include <memory>

#include "vecmath.h"

#include "objects/object3d.h"

namespace RT {

class Plane : public SimpleObject3D {
public:
    Plane(const Vector3f &normal, float d, const Material *m);

    ~Plane() override;

    bool Intersect(const Ray &r, Hit &h, float tmin) const override;

protected:
    const Vector3f normal;
    const float d;
};

} // namespace RT
#endif // PLANE_H
