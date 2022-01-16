#ifndef RT_HIT_H
#define RT_HIT_H

#include "vecmath.h"

namespace RT {

class Material;
class SimpleObject3D;
class Ray;

class Hit {
public:
    // constructors
    Hit();

    Hit(const Hit &h);

    // destructor
    ~Hit() = default;

    [[nodiscard]] float GetT() const;

    [[nodiscard]] const Material *GetMaterial() const;
    [[nodiscard]] const Vector3f &GetNormal() const;
    [[nodiscard]] const Vector3f &GetPos() const;

    [[nodiscard]] Vector3f GetAmbient() const;

    void Set(float _t, const Material *m, const Vector3f &n,
             const Vector3f &hit_point, const Vector3f &color,
             const SimpleObject3D *object);

private:
    float t;
    const Material *material;
    const SimpleObject3D *obj = nullptr;

    Vector3f normal;
    Vector3f pos;
    Vector3f ambient;
};

} // namespace RT

#endif // RT_HIT_H
