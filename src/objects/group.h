#ifndef GROUP_H
#define GROUP_H

#include <iostream>
#include <vector>

#include "hit.h"
#include "ray.h"

#include "objects/object3d.h"

class Group : public Object3D {

public:
    Group();

    explicit Group(int num_objects);

    bool intersect(const Ray &r, Hit &h, float tmin) override;

    void addObject(int index, Object3D *obj);

    [[nodiscard]] size_t getGroupSize() const;

    std::vector<Object3D *> objects;

private:
};

#endif
