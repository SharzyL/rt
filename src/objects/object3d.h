#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "core/hit.h"
#include "core/material.h"
#include "core/ray.h"

// Base class for all 3d entities.
class Object3D {
public:
    Object3D() : material(nullptr) {}

    virtual ~Object3D() = default;

    explicit Object3D(const Material *material) : material(material) {}

    // MayIntersect Ray with this object. If hit, store information in hit
    // structure.
    virtual bool intersect(const Ray &r, Hit &h, float tmin) const = 0;

protected:
    const Material *material;
};

#endif
