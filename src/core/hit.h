#ifndef HIT_H
#define HIT_H

#include <memory>

#include "vecmath.h"

#include "ray.h"

namespace RT {

class Material;

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

    void set(float _t, const Material *m, const Vector3f &n);

private:
    float t;
    const Material *material;
    Vector3f normal;
};

} // namespace RT

#endif // HIT_H
