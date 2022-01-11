#include "./bvh.h"
#include "utils/debug.h"

namespace RT {

void BVH::Build() {
    root = std::make_unique<Node>();
    root->l_idx = 0;
    root->r_idx = (int) objects.size();
    build_impl(root.get());
    box = root->box;  // for compatibility of GetBox() interface
    LOG(ERROR) << fmt::format("bvh box: ({}, {}), ({}, {}), ({}, {})",
                              box.x0, box.x1, box.y0, box.y1, box.z0, box.z1);
}

void BVH::build_impl(Node *node) {
    int l = node->l_idx, r = node->r_idx;
    for (int i = l; i < r; i++) {
        node->box.FitBox(objects[i]->GetBox());
    }
    if (r - l < 3) {  // leaf node
        return;
    }
    int dim = node->box.MaxSpanAxis();
    std::sort(&objects[l], &objects[r], [dim] (Object3D *obj1, Object3D *obj2) {
        return obj1->GetBox().Center()[dim] < obj2->GetBox().Center()[dim];
    });
    int mid = (l + r) / 2;
    node->l_child = std::make_unique<Node>();
    node->l_child->l_idx = l;
    node->l_child->r_idx = mid;
    build_impl(node->l_child.get());
    node->r_child = std::make_unique<Node>();
    node->r_child->l_idx = mid;
    node->r_child->r_idx = r;
    build_impl(node->r_child.get());
}

bool BVH::Intersect(const Ray &r, Hit &h, float tmin) const {
    return node_intersect(r, h, tmin, root.get());
}

bool BVH::node_intersect(const Ray &ray, Hit &h, float fmin, Node *node) const {
    int l = node->l_idx, r = node->r_idx;
    bool result = false;
    if (!node->box.MayIntersect(ray, fmin, h.GetT())) return false;

    if (node->l_child != nullptr) {  // non-leaf
        result |= node_intersect(ray, h, fmin, node->l_child.get());
        result |= node_intersect(ray, h, fmin, node->r_child.get());
    } else {  // leaf
        for (int i = l; i < r; i++) {
            result |= objects[i]->Intersect(ray, h, fmin);
        }
    }
    return result;
}

} // namespace RT