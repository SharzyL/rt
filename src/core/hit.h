#ifndef HIT_H
#define HIT_H

#include <memory>
#include <optional>

#include "vecmath.h"

#include "ray.h"
#include "objects/object3d.h"

namespace RT {

class Material;
class SimpleObject3D;

class Hit {
public:
    // constructors
    Hit();

    Hit(float _t, const Material *m, const Vector3f &n);

    Hit(const Hit &h);

    // destructor
    ~Hit() = default;

    [[nodiscard]] float GetT() const;

    [[nodiscard]] const Material *GetMaterial() const;

    [[nodiscard]] const Vector3f &GetNormal() const;
    [[nodiscard]] Vector3f GetAmbient(const Ray &ray) const;

    void Set(float _t, const Material *m, const Vector3f &n, const SimpleObject3D *object);

private:
    float t;
    const Material *material;
    const SimpleObject3D *texture_calculator;

    Vector3f normal;
};

} // namespace RT

#endif // HIT_H
