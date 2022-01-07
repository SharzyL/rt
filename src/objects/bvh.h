#ifndef RT_BVH_H
#define RT_BVH_H

#include <algorithm>
#include <memory>
#include <optional>
#include <vector>

#include "objects/object3d.h"

#include "utils/aabb.h"

namespace RT {

class BVH: public Object3D {
public:
    struct Node {
        int l_idx, r_idx;
        std::unique_ptr<Node> l_child, r_child;  // null if leaf node
        AABB box;
    };

    void Reserve(size_t size) { objects.reserve(size); };
    void AddObject(Object3D *obj) {
        if (obj->GetBox().IsNull()) {
            throw std::runtime_error("BVH cannot contain object without bbox");
        }
        objects.emplace_back(obj);
    };

    void Build();

    bool Intersect(const Ray &r, Hit &h, float tmin) const override;

private:
    void build_impl(Node *node);
    bool node_intersect(const Ray &s, Hit &h, float fmin, Node *node) const;

    std::vector<Object3D*> objects;
    std::unique_ptr<Node> root;
};

}

#endif //RT_BVH_H
