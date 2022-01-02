#include <tiny_obj_loader.h>
#include <limits>
#include <cassert>
#include <algorithm>

#include "core/hit.h"
#include "core/ray.h"
#include "mesh.h"
#include "objects/triangle.h"

namespace RT {

bool Mesh::Intersect(const Ray &r, Hit &h, float tmin) const {
    if (num_faces > 2 && !bbox.MayIntersect(r)) return false;
    bool result = false;
    for (int triId = 0; triId < (int) tri_idx_list.size(); ++triId) {
        const TriangleIndex &triIndex = tri_idx_list[triId];
        Triangle triangle(group_vertices[triIndex[0]], group_vertices[triIndex[1]], group_vertices[triIndex[2]],
                          &group_materials[mat_idx[triId]]);
        triangle.normal = face_normals[triId];
        result |= triangle.Intersect(r, h, tmin);
    }
    return result;
}

void Mesh::ComputeNormal() {
    for (int triId = 0; triId < (int) tri_idx_list.size(); ++triId) {
        TriangleIndex &triIndex = tri_idx_list[triId];
        Vector3f a = group_vertices[triIndex[1]] - group_vertices[triIndex[0]];
        Vector3f b = group_vertices[triIndex[2]] - group_vertices[triIndex[0]];
        b = Vector3f::cross(a, b);
        face_normals[triId] = b / b.length();
    }
}

Mesh::Mesh(const std::vector<Vector3f> &vs, const std::vector<Material> &mats, const tinyobj::shape_t &shape)
        : group_vertices(vs), group_materials(mats) {
    num_faces = shape.mesh.num_face_vertices.size();
    tri_idx_list.reserve(num_faces);
    face_normals.reserve(num_faces);
    mat_idx.reserve(num_faces);
    size_t index_offset = 0;
    for (size_t f = 0; f < num_faces; f++) { // iterate faces
        assert(shape.mesh.num_face_vertices[f] == 3);
        int v1_idx = shape.mesh.indices[index_offset + 0].vertex_index;
        int v2_idx = shape.mesh.indices[index_offset + 1].vertex_index;
        int v3_idx = shape.mesh.indices[index_offset + 2].vertex_index;
        tri_idx_list.emplace_back(v1_idx, v2_idx, v3_idx);
        bbox.AddVertex(group_vertices[v1_idx]);
        bbox.AddVertex(group_vertices[v2_idx]);
        bbox.AddVertex(group_vertices[v3_idx]);

        int this_mat_idx = shape.mesh.material_ids[f];
        if (this_mat_idx < 0) this_mat_idx = 0;
        mat_idx.emplace_back(this_mat_idx);
        index_offset += 3;
    }
    ComputeNormal();
}

BoundingBox::BoundingBox() {
    x0 = y0 = z0 = std::numeric_limits<float>::max();
    x1 = y1 = z1 = -std::numeric_limits<float>::max();
}

void BoundingBox::AddVertex(Vector3f v) {
    x0 = std::min(x0, v.x());
    x1 = std::max(x1, v.x());
    y0 = std::min(y0, v.y());
    y1 = std::max(y1, v.y());
    z0 = std::min(z0, v.z());
    z1 = std::max(z1, v.z());
}

bool BoundingBox::MayIntersect(const Ray &ray) const {
    const Vector3f &dir = ray.GetDirection();
    const Vector3f &origin = ray.GetOrigin();
    float intersect_x0 = (x0 - origin.x()) / dir.x();
    float intersect_x1 = (x1 - origin.x()) / dir.x();
    if (intersect_x0 > intersect_x1) std::swap(intersect_x0, intersect_x1);
    float intersect_y0 = (y0 - origin.y()) / dir.y();
    float intersect_y1 = (y1 - origin.y()) / dir.y();
    if (intersect_y0 > intersect_y1) std::swap(intersect_y0, intersect_y1);
    float intersect_z0 = (z0 - origin.z()) / dir.z();
    float intersect_z1 = (z1 - origin.z()) / dir.z();
    if (intersect_z0 > intersect_z1) std::swap(intersect_z0, intersect_z1);
    float into = std::max(intersect_x0, std::max(intersect_y0, intersect_y1));
    float out = std::min(intersect_x1, std::min(intersect_y1, intersect_z1));
    return into <= out;
}

} // namespace RT
