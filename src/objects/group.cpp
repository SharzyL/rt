#include "./group.h"

namespace RT {

Group::Group() = default;

Group::Group(int num_objects) { objects.reserve(num_objects); }

bool Group::Intersect(const Ray &r, Hit &h, float tmin) const {
    bool is_intersect = false;
    for (const auto &obj: objects) {
        if (obj->Intersect(r, h, tmin)) {
            is_intersect = true;
        }
    }
    return is_intersect;
}

} // namespace RT