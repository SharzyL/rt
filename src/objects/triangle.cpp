#include "utils/math_util.h"
#include "utils/debug.h"

#include "core/ray.h"
#include "core/hit.h"
#include "core/material.h"
#include "core/texture.h"

#include "./triangle.h"

namespace RT {

Triangle::Triangle(const Vector3f &a, const Vector3f &b, const Vector3f &c, const Material *m, const Texture *tex)
    : SimpleObject3D(m, tex), a(a), b(b), c(c) {  // not supporting texture yet
    normal = Vector3f::cross(b - a, c - a).normalized();
    box.AddVertex(a);
    box.AddVertex(b);
    box.AddVertex(c);
}

bool Triangle::Intersect(const Ray &r, Hit &h, float tmin) const {
    const Vector3f &rd = r.GetDirection();
    const Vector3f e1 = a - b, e2 = a - c, s = a - r.GetOrigin();
    float det_rd_e1_e2 = tri_det(rd, e1, e2);
    float t = tri_det(s, e1, e2) / det_rd_e1_e2;
    float beta = tri_det(rd, s, e2) / det_rd_e1_e2;  // weight of b
    float gamma = tri_det(rd, e1, s) / det_rd_e1_e2;  // weight of c
    if (t < h.GetT() && t > tmin && 0 <= beta && 0 <= gamma && beta + gamma <= 1) {
        Vector3f color = material->ambientColor;
        if (texture != nullptr) {
            CHECK(has_tex_coord);
            Vector2f uv = (1 - beta - gamma) * ta + beta * tb + gamma * tc;
            color = texture->At(uv.x(), uv.y());
        }
        Vector3f true_normal = has_norm
                ? (1 - beta - gamma) * na + beta * nb + gamma * nc
                : normal;
        h.Set(t, material, true_normal, r.PointAtParameter(t), color, this);
        return true;
    } else {
        return false;
    }
}

void Triangle::SetVertexNormal(const Vector3f &_na, const Vector3f &_nb, const Vector3f &_nc) {
    na = _na;
    nb = _nb;
    nc = _nc;
    has_norm = true;
}

void Triangle::SetTextureCoord(const Vector2f &_ta, const Vector2f &_tb, const Vector2f &_tc) {
    ta = _ta;
    tb = _tb;
    tc = _tc;
    has_tex_coord = true;
}

} // namespace RT
