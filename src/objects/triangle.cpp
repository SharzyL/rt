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
        if (has_norm) {
            Vector3f n = (1 - beta - gamma) * na + beta * nb + gamma * nc;
            h.Set(t, material, n.normalized(), r.PointAtParameter(t), this);
        } else {
            h.Set(t, material, normal, r.PointAtParameter(t), this);
        }
        return true;
    } else {
        return false;
    }
}

Vector3f Triangle::AmbientColorAtHit(const Hit &hit) const {
    if (texture != nullptr) {
        CHECK(has_tex_coord);
        const Vector3f e1 = a - b, e2 = a - c, s = a - hit.GetPos();
        float det_rd_e1_e2 = tri_det(normal, e1, e2);
        float beta = tri_det(normal, s, e2) / det_rd_e1_e2;  // weight of b
        float gamma = tri_det(normal, e1, s) / det_rd_e1_e2;  // weight of c
        Vector2f uv = (1 - beta - gamma) * ta + beta * tb + gamma * tc;
        return texture->At(uv.x(), uv.y());
    } else {
        return material->ambientColor;
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
