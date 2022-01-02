#include <tiny_obj_loader.h>
#include <limits>
#include <cassert>

#include "core/hit.h"
#include "core/ray.h"
#include "mesh.h"
#include "objects/triangle.h"

bool Mesh::intersect(const Ray &r, Hit &h, float tmin) const {
    if (num_faces > 2 && !bbox.MayIntersect(r)) return false;
    bool result = false;
    for (int triId = 0; triId < (int) tri_idx_list.size(); ++triId) {
        const TriangleIndex &triIndex = tri_idx_list[triId];
        Triangle triangle(group_vertices[triIndex[0]], group_vertices[triIndex[1]], group_vertices[triIndex[2]],
                          &group_materials[mat_idx[triId]]);
        triangle.normal = face_normals[triId];
        result |= triangle.intersect(r, h, tmin);
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
    const Vector3f &dir = ray.getDirection();
    const Vector3f &origin = ray.getOrigin();
    Vector3f intersect_x0 = ray.pointAtParameter((x0 - origin.x()) / dir.x());
    if (y0 <= intersect_x0.y() && intersect_x0.y() <= y1 && z0 <= intersect_x0.z() && intersect_x0.z() <= z1) {
        return true;
    }

    Vector3f intersect_x1 = ray.pointAtParameter((x1 - origin.x()) / dir.x());
    if (y0 <= intersect_x1.y() && intersect_x1.y() <= y1 && z0 <= intersect_x1.z() && intersect_x1.z() <= z1) {
        return true;
    }

    Vector3f intersect_y0 = ray.pointAtParameter((y0 - origin.y()) / dir.y());
    if (x0 <= intersect_y0.x() && intersect_y0.x() <= x1 && z0 <= intersect_y0.z() && intersect_y0.z() <= z1) {
        return true;
    }

    Vector3f intersect_y1 = ray.pointAtParameter((y1 - origin.y()) / dir.y());
    if (x0 <= intersect_y1.x() && intersect_y1.x() <= x1 && z0 <= intersect_y1.z() && intersect_y1.z() <= z1) {
        return true;
    }

    Vector3f intersect_z0 = ray.pointAtParameter((z0 - origin.z()) / dir.z());
    if (y0 <= intersect_z0.y() && intersect_z0.y() <= y1 && x0 <= intersect_z0.x() && intersect_z0.x() <= x1) {
        return true;
    }

    Vector3f intersect_z1 = ray.pointAtParameter((z1 - origin.z()) / dir.z());
    if (y0 <= intersect_z1.y() && intersect_z1.y() <= y1 && x0 <= intersect_z1.x() && intersect_z1.x() <= x1) {
        return true;
    }

    return false;
}
