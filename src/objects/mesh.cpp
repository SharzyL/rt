#include <tiny_obj_loader.h>

#include "core/hit.h"
#include "core/ray.h"
#include "mesh.h"
#include "objects/triangle.h"

bool Mesh::intersect(const Ray &r, Hit &h, float tmin) const {

    // Optional: Change this brute force method into a faster one.
    bool result = false;
    for (int triId = 0; triId < (int)tri_idx_list.size(); ++triId) {
        const TriangleIndex &triIndex = tri_idx_list[triId];
        Triangle triangle(group_vertices.at(triIndex.at(0)), group_vertices.at(triIndex.at(1)), group_vertices.at(triIndex.at(2)),
                          &group_materials.at(mat_idx[triId]));
        triangle.normal = n[triId];
        result |= triangle.intersect(r, h, tmin);
    }
    return result;
}

void Mesh::computeNormal() {
    for (int triId = 0; triId < (int)tri_idx_list.size(); ++triId) {
        TriangleIndex &triIndex = tri_idx_list[triId];
        Vector3f a = group_vertices[triIndex[1]] - group_vertices[triIndex[0]];
        Vector3f b = group_vertices[triIndex[2]] - group_vertices[triIndex[0]];
        b = Vector3f::cross(a, b);
        n[triId] = b / b.length();
    }
}

Mesh::Mesh(const std::vector<Vector3f> &vs, const std::vector<Material> &mats, size_t size) : group_vertices(vs), group_materials(mats) {
    tri_idx_list.reserve(size);
    n.reserve(size);
    mat_idx.reserve(size);
}
