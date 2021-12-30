#ifndef GROUP_H
#define GROUP_H

#include "hit.hpp"
#include "objects/object3d.hpp"
#include "ray.hpp"
#include <iostream>
#include <vector>

class Group : public Object3D {

public:
    Group() = default;

    explicit Group(int num_objects) { objects.reserve(num_objects); }

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        bool is_intersect = false;
        for (const auto obj : objects) {
            if (obj->intersect(r, h, tmin)) {
                is_intersect = true;
            }
        }
        return is_intersect;
    }

    void addObject(int index, Object3D *obj) { objects.push_back(obj); }

    size_t getGroupSize() const { return objects.size(); }

    std::vector<Object3D *> objects;

private:
};

#endif
