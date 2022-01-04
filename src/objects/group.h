#ifndef RT_GROUP_H
#define RT_GROUP_H

#include <iostream>
#include <memory>
#include <vector>

#include "core/hit.h"
#include "core/material.h"
#include "core/ray.h"

#include "objects/object3d.h"

namespace RT {

class Group : public Object3D {

public:
    Group();

    explicit Group(int num_objects);

    bool Intersect(const Ray &r, Hit &h, float tmin) const override;

    std::vector<std::unique_ptr<Object3D>> objects;

private:
    std::vector<Vector3f> vs;
    std::vector<Material> mats;
};

} // namespace RT

#endif
