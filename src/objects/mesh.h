#ifndef MESH_H
#define MESH_H

#include <vector>

#include "objects/triangle.h"
#include "bvh.h"

// forward declaration
namespace tinyobj {
class shape_t;
}

namespace RT {

class Mesh : public Object3D {

public:
    Mesh(const std::vector<Vector3f> &vs, const std::vector<Material> &mats, const tinyobj::shape_t &shape,
         const Material *default_mat = nullptr);

    Mesh(std::vector<Triangle> &&vs, const Material *mat);

    bool Intersect(const Ray &r, Hit &h, float tmin) const override;

private:
    size_t num_faces;
    std::vector<Triangle> triangles;
    BVH bvh;  // bvh stores pointers of triangles, must ensure the pointers do not expire
};

} // namespace RT

#endif
