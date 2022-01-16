#ifndef RT_ROTATE_BEZIER_H
#define RT_ROTATE_BEZIER_H

#include <vector>

#include <Vector2f.h>
#include <Vector3f.h>

#include "core/ray.h"
#include "core/hit.h"

#include "objects/object3d.h"
#include "objects/mesh.h"

#include "utils/math_util.h"

namespace RT {

class RotateBezier: public SimpleObject3D {
public:
    RotateBezier(std::vector<Vector2f> &&controls, Vector2f axis, Material *mat, Texture *texture);

    bool Intersect(const Ray &ray, Hit &hit, float tmin) const override;

    std::unique_ptr<Mesh> MakeMesh(const Material *mat, const Texture *tex, int density_x, int density_y) const;

    [[nodiscard]] std::pair<Vector2f, Vector2f> bezier_evaluate(float bt, float min_t = 0.f, float max_t = 1.f) const;

    std::vector<Vector2f> controls;
    Vector2f axis;
    float yfirst, ylast;

    std::unique_ptr<Mesh> surrounding_mesh;
};

} // namespace RT
#endif //RT_ROTATE_BEZIER_H
