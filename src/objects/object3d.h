#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "core/hit.h"
#include "core/material.h"
#include "core/ray.h"

namespace RT {

// Base class for all 3d entities.
class Object3D {
public:
    Object3D() = default;

    virtual ~Object3D() = default;

    // MayIntersect Ray with this object. If hit, store information in hit
    // structure.
    virtual bool Intersect(const Ray &r, Hit &h, float tmin) const = 0;
};

class SimpleObject3D: public Object3D {
public:
    explicit SimpleObject3D(const Material *material) : material(material) {};
protected:
    const Material *material;
};

} // namespace RT

#endif
