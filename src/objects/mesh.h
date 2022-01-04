#ifndef MESH_H
#define MESH_H

#include <vector>

#include "objects/triangle.h"

// forward declaration
namespace tinyobj {
class shape_t;
}

namespace RT {

class BoundingBox : public SimpleObject3D {
public:
    explicit BoundingBox(const Material *mat);
    void AddVertex(const Vector3f &v);

    [[nodiscard]] Vector3f AmbientColorAtHit(const Hit &hit) const override;

    [[nodiscard]] bool MayIntersect(const Ray &ray, float tmin, float tmax) const;
    [[nodiscard]] bool Intersect(const Ray &ray, Hit &hit, float tmin) const override;

private:
    float x0, x1, y0, y1, z0, z1;
};

class Mesh : public Object3D {

public:
    Mesh(const std::vector<Vector3f> &vs, const std::vector<Material> &mats, const tinyobj::shape_t &shape,
         const Material *default_mat = nullptr);

    bool Intersect(const Ray &r, Hit &h, float tmin) const override;

private:
    BoundingBox bbox;
    size_t num_faces;
    std::vector<Triangle> triangles;
};

} // namespace RT

#endif
