#include <limits>
#include <cassert>
#include <algorithm>

#include <tiny_obj_loader.h>

#include <Vector3f.h>

#include "core/hit.h"
#include "core/ray.h"
#include "core/material.h"

#include "utils/debug.h"

#include "./mesh.h"

namespace RT {

bool Mesh::Intersect(const Ray &r, Hit &h, float tmin) const {
    return bvh.Intersect(r, h, tmin);
}

Mesh::Mesh(const std::vector<Vector3f> &vs, const std::vector<Material> &mats, const tinyobj::shape_t &shape, const Material *default_mat) {
    num_faces = shape.mesh.num_face_vertices.size();
    size_t index_offset = 0;
    bvh.Reserve(num_faces);
    triangles.reserve(num_faces);  // necessary, otherwise pointers in bvh expires

    for (size_t f = 0; f < num_faces; f++) { // iterate faces
        int this_mat_idx = shape.mesh.material_ids[f];
        CHECK((this_mat_idx >= 0 && this_mat_idx < mats.size()) || default_mat != nullptr) << "No material is given, and no default material";
        const Material *mat = this_mat_idx >= 0 ? &mats[this_mat_idx] : default_mat;
        CHECK(mat != nullptr) << "nullptr material detected";

        CHECK(shape.mesh.num_face_vertices[f] == 3) << "non-triangle face in a mesh";
        const Vector3f &v1 = vs[shape.mesh.indices[index_offset + 0].vertex_index];
        const Vector3f &v2 = vs[shape.mesh.indices[index_offset + 1].vertex_index];
        const Vector3f &v3 = vs[shape.mesh.indices[index_offset + 2].vertex_index];
        Triangle &tri = triangles.emplace_back(v1, v2, v3, mat);
        bvh.AddObject(&tri);
        index_offset += 3;
    }
    bvh.Build();
    box = bvh.GetBox();
}

Mesh::Mesh(std::vector<Triangle> &&triangles, const Material *mat): triangles(triangles) {
    num_faces = triangles.size();
    for (Triangle &tri: this->triangles) {
        bvh.AddObject(&tri);
    }
    bvh.Build();
    box = bvh.GetBox();
}

} // namespace RT
