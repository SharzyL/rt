#include "./triangle.h"

Triangle::Triangle(const Vector3f &a, const Vector3f &b, const Vector3f &c, Material *m)
    : Object3D(m), a(a), b(b), c(c) {
    normal = Vector3f::cross(b - a, c - a).normalized();
}

bool Triangle::intersect(const Ray &r, Hit &h, float tmin) {
    const Vector3f &rd = r.getDirection();
    const Vector3f e1 = a - b, e2 = a - c, s = a - r.getOrigin();
    float det_rd_e1_e2 = Matrix3f(rd, e1, e2).determinant();
    float t = Matrix3f(s, e1, e2).determinant() / det_rd_e1_e2;
    float beta = Matrix3f(rd, s, e2).determinant() / det_rd_e1_e2;
    float gamma = Matrix3f(rd, e1, s).determinant() / det_rd_e1_e2;
    if (t < h.getT() && t >= tmin && 0 <= beta && beta <= 1 && 0 <= gamma && gamma <= 1 && beta + gamma <= 1) {
        h.set(t, material, normal);
        return true;
    } else {
        return false;
    }
}
