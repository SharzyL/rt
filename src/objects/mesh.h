#ifndef MESH_H
#define MESH_H

#include <vector>

#include <tiny_obj_loader.h>

#include "Vector2f.h"
#include "Vector3f.h"

#include "objects/object3d.h"
#include "objects/triangle.h"

namespace RT {
class BoundingBox {
public:
    BoundingBox();
    void AddVertex(const Vector3f &v);
    [[nodiscard]] bool MayIntersect(const Ray &ray) const;
private:
    float x0, x1, y0, y1, z0, z1;
};

class Mesh : public Object3D {

public:
    Mesh(const std::vector<Vector3f> &vs, const std::vector<Material> &mats, const tinyobj::shape_t &shape);

    bool Intersect(const Ray &r, Hit &h, float tmin) const override;

private:
    BoundingBox bbox;
    size_t num_faces;
    std::vector<Triangle> triangles;
};

} // namespace RT

#endif
