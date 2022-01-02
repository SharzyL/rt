#ifndef MESH_H
#define MESH_H

#include <vector>

#include <tiny_obj_loader.h>

#include "Vector2f.h"
#include "Vector3f.h"

#include "objects/object3d.h"

class BoundingBox {
public:
    BoundingBox();
    void AddVertex(Vector3f v);
    [[nodiscard]] bool MayIntersect(const Ray &ray) const;
private:
    float x0, x1, y0, y1, z0, z1;
};

class Mesh : public Object3D {

public:
    Mesh(const std::vector<Vector3f> &vs, const std::vector<Material> &mats, const tinyobj::shape_t &shape);

    struct TriangleIndex {
        TriangleIndex(int idx1, int idx2, int idx3) : x{idx1, idx2, idx3} {}
        const int &operator[](int i) const { return x[i]; }
        // By Computer Graphics convention, counterclockwise winding is front
        // face
        int x[3]{};
    };

    std::vector<TriangleIndex> tri_idx_list;
    std::vector<int> mat_idx;
    std::vector<Vector3f> face_normals;
    bool Intersect(const Ray &r, Hit &h, float tmin) const override;

    // Normal can be used for light estimation
    void ComputeNormal();

private:
    BoundingBox bbox;
    size_t num_faces;
    const std::vector<Vector3f> &group_vertices;
    const std::vector<Material> &group_materials;
};

#endif
