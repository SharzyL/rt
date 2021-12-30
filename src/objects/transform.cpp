#include "./transform.h"

static Vector3f transformPoint(const Matrix4f &mat, const Vector3f &point) { return (mat * Vector4f(point, 1)).xyz(); }

// transform a 3D direction using a matrix, returning a direction
static Vector3f transformDirection(const Matrix4f &mat, const Vector3f &dir) { return (mat * Vector4f(dir, 0)).xyz(); }

Transform::Transform(const Matrix4f &m, Object3D *obj) : o(obj) { transform = m.inverse(); }

bool Transform::intersect(const Ray &r, Hit &h, float tmin) {
    Vector3f trSource = transformPoint(transform, r.getOrigin());
    Vector3f trDirection = transformDirection(transform, r.getDirection());
    Ray tr(trSource, trDirection);
    bool inter = o->intersect(tr, h, tmin);
    if (inter) {
        h.set(h.getT(), h.getMaterial(), transformDirection(transform.transposed(), h.getNormal()).normalized());
    }
    return inter;
}
