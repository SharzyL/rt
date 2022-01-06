#ifndef RT_ROTATE_BEZIER_H
#define RT_ROTATE_BEZIER_H

#include <vector>

#include <Vector2f.h>
#include <Vector3f.h>

#include "core/ray.h"
#include "core/hit.h"

#include "objects/object3d.h"

#include "utils/math_util.h"

namespace RT {

class RotateBezier: SimpleObject3D {
public:
    RotateBezier(std::vector<Vector2f> &&controls, Vector2f axis, Material *mat, Texture *texture);

    bool Intersect(const Ray &ray, Hit &hit, float tmin) const override;

    [[nodiscard]] Vector3f AmbientColorAtHit(const Hit &hit) const override;

    [[nodiscard]] std::pair<Vector2f, Vector2f> bezier_evaluate(float bt) const;

    std::vector<Vector2f> controls;
    Vector2f axis;
    float ymin, ymax;
};


} // namespace RT
#endif //RT_ROTATE_BEZIER_H
