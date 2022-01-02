#ifndef RAY_H
#define RAY_H

#include <iostream>

#include "Vector3f.h"

namespace RT {

// Ray class mostly copied from Peter Shirley and Keith Morley
class Ray {
public:
    Ray() = delete;
    Ray(const Vector3f &orig, const Vector3f &dir);

    Ray(const Ray &r);

    [[nodiscard]] const Vector3f &GetOrigin() const;

    [[nodiscard]] const Vector3f &GetDirection() const;

    [[nodiscard]] Vector3f PointAtParameter(float t) const;

    void set(const Vector3f &orig, const Vector3f &dir);

private:
    Vector3f origin;
    Vector3f direction;
};

inline std::ostream &operator<<(std::ostream &os, const Ray &r);

} // namespace RT

#endif // RAY_H
