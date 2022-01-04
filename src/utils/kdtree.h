#ifndef RT_KDTREE_H
#define RT_KDTREE_H

#include <Vector3f.h>

namespace RT {


struct KDTRee {
    struct Node {
        Vector3f min, max;
        float radius;
        Node *l_child, *r_child;
    };


};

} // namespace RT

#endif //RT_KDTREE_H
