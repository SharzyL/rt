#include "./group.h"

Group::Group() = default;

Group::Group(int num_objects) { objects.reserve(num_objects); }

bool Group::intersect(const Ray &r, Hit &h, float tmin) {
    bool is_intersect = false;
    for (const auto obj : objects) {
        if (obj->intersect(r, h, tmin)) {
            is_intersect = true;
        }
    }
    return is_intersect;
}

void Group::addObject(int index, Object3D *obj) { objects.push_back(obj); }

size_t Group::getGroupSize() const { return objects.size(); }
