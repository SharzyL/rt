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
    if (num_faces > 100) return bbox.Intersect(r, h, tmin);
    if (!bbox.MayIntersect(r, tmin, h.GetT())) return false;
    bool result = false;
    for (const auto &triangle: triangles) {
        result |= triangle.Intersect(r, h, tmin);
    }
    return result;
}

Mesh::Mesh(const std::vector<Vector3f> &vs, const std::vector<Material> &mats, const tinyobj::shape_t &shape, const Material *default_mat)
: bbox(default_mat) {
    num_faces = shape.mesh.num_face_vertices.size();
    size_t index_offset = 0;
    for (size_t f = 0; f < num_faces; f++) { // iterate faces
        int this_mat_idx = shape.mesh.material_ids[f];
        CHECK((this_mat_idx >= 0 && this_mat_idx < mats.size()) || default_mat != nullptr) << "No material is given, and no default material";
        const Material *mat = this_mat_idx >= 0 ? &mats[this_mat_idx] : default_mat;
        CHECK(mat != nullptr) << "nullptr material detected";

        CHECK(shape.mesh.num_face_vertices[f] == 3) << "non-triangle face in a mesh";
        const Vector3f &v1 = vs[shape.mesh.indices[index_offset + 0].vertex_index];
        const Vector3f &v2 = vs[shape.mesh.indices[index_offset + 1].vertex_index];
        const Vector3f &v3 = vs[shape.mesh.indices[index_offset + 2].vertex_index];
        triangles.emplace_back(v1, v2, v3, mat);

        bbox.AddVertex(v1);
        bbox.AddVertex(v2);
        bbox.AddVertex(v3);

        index_offset += 3;
    }
}

BoundingBox::BoundingBox(const Material *mat): SimpleObject3D(mat, nullptr) {
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

bool BoundingBox::MayIntersect(const Ray &ray, float tmin, float tmax) const {
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
    return tmin <= into && into <= out && out < tmax;
}

bool BoundingBox::Intersect(const Ray &ray, Hit &hit, float tmin) const {
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

    float out = std::min(intersect_x1, std::min(intersect_y1, intersect_z1));

    Vector3f norm;
    float t;
    if (intersect_x0 > intersect_y0 && intersect_x0 > intersect_z0) {
        t = intersect_x0;
        norm = Vector3f(1, 0, 0);
    } else if (intersect_y0 > intersect_z0) {
        t = intersect_y0;
        norm = Vector3f(0, 1, 0);
    } else {
        t = intersect_z0;
        norm = Vector3f(0, 0, 1);
    }
    if (tmin < t && t < hit.GetT() && t < out){
        if (Vector3f::dot(norm, dir) > 0) norm = -norm;
        hit.Set(t, material, norm, ray.PointAtParameter(t), nullptr);
        return true;
    } else {
        return false;
    }
}

Vector3f BoundingBox::AmbientColorAtHit(const Hit &hit) const {
    throw std::runtime_error("bbox texture not implemented");
}

} // namespace RT
