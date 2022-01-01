#include <fstream>
#include <iostream>

#include <tiny_obj_loader.h>

#include "core/hit.h"
#include "core/ray.h"
#include "mesh.h"

bool Mesh::intersect(const Ray &r, Hit &h, float tmin) const {

    // Optional: Change this brute force method into a faster one.
    bool result = false;
    for (int triId = 0; triId < (int)t.size(); ++triId) {
        const TriangleIndex &triIndex = t[triId];
        Triangle triangle(vs.at(triIndex.at(0)), vs.at(triIndex.at(1)), vs.at(triIndex.at(2)),
                          &mats.at(mat_idx[triId]));
        //        triangle.normal = n[triId];
        result |= triangle.intersect(r, h, tmin);
    }
    return result;
}

void Mesh::computeNormal() {
    for (int triId = 0; triId < (int)t.size(); ++triId) {
        TriangleIndex &triIndex = t[triId];
        Vector3f a = vs[triIndex[1]] - vs[triIndex[0]];
        Vector3f b = vs[triIndex[2]] - vs[triIndex[0]];
        b = Vector3f::cross(a, b);
        n[triId] = b / b.length();
    }
}

Mesh::Mesh(const std::vector<Vector3f> &vs, const std::vector<Material> &mats, size_t size) : vs(vs), mats(mats) {
    t.reserve(size);
    n.reserve(size);
    mat_idx.reserve(size);
}
