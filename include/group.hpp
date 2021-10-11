#ifndef GROUP_H
#define GROUP_H

#include "hit.hpp"
#include "object3d.hpp"
#include "ray.hpp"
#include <iostream>
#include <vector>

// TODO: Implement Group - add data structure to store a list of Object*
class Group : public Object3D {

public:
    Group() = default;

    explicit Group(int num_objects) { objects.reserve(num_objects); }

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        // TODO: group intersection algorithm
        return false;
    }

    void addObject(int index, Object3D *obj) { objects.push_back(obj); }

    size_t getGroupSize() const { return objects.size(); }

    std::vector<Object3D *> objects;

private:
};

#endif
