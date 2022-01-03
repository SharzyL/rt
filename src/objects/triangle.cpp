#include "util.h"
#include "./triangle.h"

namespace RT {

Triangle::Triangle(const Vector3f &a, const Vector3f &b, const Vector3f &c, const Material *m)
    : SimpleObject3D(m, nullptr), a(a), b(b), c(c) {  // not supporting texture yet
    normal = Vector3f::cross(b - a, c - a).normalized();
}

bool Triangle::Intersect(const Ray &r, Hit &h, float tmin) const {
    const Vector3f &rd = r.GetDirection();
    const Vector3f e1 = a - b, e2 = a - c, s = a - r.GetOrigin();
    float det_rd_e1_e2 = tri_det(rd, e1, e2);
    float t = tri_det(s, e1, e2) / det_rd_e1_e2;
    float beta = tri_det(rd, s, e2) / det_rd_e1_e2;
    float gamma = tri_det(rd, e1, s) / det_rd_e1_e2;
    if (t < h.GetT() && t > tmin && 0 <= beta && 0 <= gamma && beta + gamma <= 1) {
        h.Set(t, material, normal, this);
        return true;
    } else {
        return false;
    }
}

Vector3f Triangle::AmbientColorAtHit(const Ray &r, const Hit &hit) const {
    // TODO: implement texture for triangle
    return material->ambientColor;
}

} // namespace RT
