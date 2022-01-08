#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vecmath.h"

#include "objects/object3d.h"

namespace RT {

class Triangle : public SimpleObject3D {

public:
    // a b c are three get_v positions of the triangle
    Triangle(const Vector3f &a, const Vector3f &b, const Vector3f &c, const Material *m, const Texture *tex = nullptr);

    bool Intersect(const Ray &r, Hit &h, float tmin) const override;

    [[nodiscard]] Vector3f AmbientColorAtHit(const Hit &hit) const override;

    void SetVertexNormal(const Vector3f &_na, const Vector3f &_nb, const Vector3f &_nc);
    void SetTextureCoord(const Vector2f &_ta, const Vector2f &_tb, const Vector2f &_tc);

    Vector3f normal;

    Vector3f a, b, c;

    Vector3f na, nb, nc;
    bool has_norm = false;

    Vector2f ta, tb, tc;
    bool has_tex_coord = false;
};

} // namespace RT

#endif // TRIANGLE_H
