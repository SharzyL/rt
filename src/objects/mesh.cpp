#include <tiny_obj_loader.h>
#include <limits>
#include <cassert>
#include <algorithm>

#include "core/hit.h"
#include "core/ray.h"
#include "./mesh.h"

#include "debug.h"

namespace RT {

bool Mesh::Intersect(const Ray &r, Hit &h, float tmin) const {
    if (!bbox.MayIntersect(r)) return false;
    bool result = false;
    for (const auto &triangle: triangles) {
        result |= triangle.Intersect(r, h, tmin);
    }
    return result;
}

Mesh::Mesh(const std::vector<Vector3f> &vs, const std::vector<Material> &mats, const tinyobj::shape_t &shape) {
    num_faces = shape.mesh.num_face_vertices.size();
    size_t index_offset = 0;
    for (size_t f = 0; f < num_faces; f++) { // iterate faces
        int this_mat_idx = shape.mesh.material_ids[f];
        if (this_mat_idx < 0) this_mat_idx = 0;  // use default material if no material is given

        assert(shape.mesh.num_face_vertices[f] == 3);
        const Vector3f &v1 = vs[shape.mesh.indices[index_offset + 0].vertex_index];
        const Vector3f &v2 = vs[shape.mesh.indices[index_offset + 1].vertex_index];
        const Vector3f &v3 = vs[shape.mesh.indices[index_offset + 2].vertex_index];
        triangles.emplace_back(v1, v2, v3, &mats[this_mat_idx]);

        bbox.AddVertex(v1);
        bbox.AddVertex(v2);
        bbox.AddVertex(v3);

        index_offset += 3;
    }
}

BoundingBox::BoundingBox() {
    x0 = y0 = z0 = std::numeric_limits<float>::max();
    x1 = y1 = z1 = -std::numeric_limits<float>::max();
}

void BoundingBox::AddVertex(const Vector3f &v) {
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
    float into = std::max(intersect_x0, std::max(intersect_y0, intersect_z0));
    float out = std::min(intersect_x1, std::min(intersect_y1, intersect_z1));
    return into <= out;
}

} // namespace RT
