#include "./transform.h"

namespace RT {

static Vector3f transformPoint(const Matrix4f &mat, const Vector3f &point) { return (mat * Vector4f(point, 1)).xyz(); }

// transform a 3D direction using a matrix, returning a direction
static Vector3f transformDirection(const Matrix4f &mat, const Vector3f &dir) { return (mat * Vector4f(dir, 0)).xyz(); }

Transform::Transform(const Matrix4f &m, Object3D *obj) : o(obj) { transform = m.inverse(); }

bool Transform::Intersect(const Ray &r, Hit &h, float tmin) const {
    Vector3f trSource = transformPoint(transform, r.GetOrigin());
    Vector3f trDirection = transformDirection(transform, r.GetDirection());
    Ray tr(trSource, trDirection);
    bool inter = o->Intersect(tr, h, tmin);
    if (inter) {
        h.set(h.GetT(), h.GetMaterial(), transformDirection(transform.transposed(), h.GetNormal()).normalized());
    }
    return inter;
}

} // namespace RT
