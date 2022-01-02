#ifndef HIT_H
#define HIT_H

#include "vecmath.h"

#include "ray.h"

class Material;

class Hit {
public:
    // constructors
    Hit();

    Hit(float _t, Material *m, const Vector3f &n);

    Hit(const Hit &h);

    // destructor
    ~Hit() = default;

    [[nodiscard]] float getT() const;

    [[nodiscard]] const Material *getMaterial() const;

    [[nodiscard]] const Vector3f &getNormal() const;

    void set(float _t, const Material *m, const Vector3f &n);

private:
    float t;
    const Material *material;
    Vector3f normal;
};

#endif // HIT_H
