#ifndef RT_AABB_H
#define RT_AABB_H

#include <Vector3f.h>

#include "core/ray.h"
#include "core/hit.h"

namespace RT {

class AABB {
public:
    AABB();
    void AddVertex(const Vector3f &v);
    void FitBox(const AABB &box);

    [[nodiscard]] bool MayIntersect(const Ray &ray, float tmin, float tmax) const;
    [[nodiscard]] int MaxSpanAxis() const;

    void Reset();
    [[nodiscard]] bool IsNull() const;

    [[nodiscard]] const Vector3f& Center() const;

    float x0, x1, y0, y1, z0, z1;

private:
    int num_v;
    float x_sum, y_sum, z_sum;  // for computing center
    Vector3f center;
};

}

#endif //RT_AABB_H
