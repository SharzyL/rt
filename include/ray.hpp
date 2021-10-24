#ifndef RAY_H
#define RAY_H

#include <Vector3f.h>
#include <cassert>
#include <iostream>

#include <fmt/core.h>
#include <fmt/ranges.h>
#include <glog/logging.h>

template <> struct fmt::formatter<Vector3f> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }
    template <typename FormatContext>
    auto format(const Vector3f &p, FormatContext &ctx) {
        return format_to(ctx.out(), "({:.3f}, {:.3f}, {:.3f})", p.x(), p.y(), p.z());
    }
};

// Ray class mostly copied from Peter Shirley and Keith Morley
class Ray {
public:
    Ray() = delete;
    Ray(const Vector3f &orig, const Vector3f &dir) {
        origin = orig;
        direction = dir;
    }

    Ray(const Ray &r) {
        origin = r.origin;
        direction = r.direction;
    }

    const Vector3f &getOrigin() const { return origin; }

    const Vector3f &getDirection() const { return direction; }

    Vector3f pointAtParameter(float t) const { return origin + direction * t; }

private:
    Vector3f origin;
    Vector3f direction;
};

inline std::ostream &operator<<(std::ostream &os, const Ray &r) {
    os << "Ray <" << r.getOrigin() << ", " << r.getDirection() << ">";
    return os;
}

#endif // RAY_H
