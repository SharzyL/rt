#ifndef RT_MATH_UTIL_H
#define RT_MATH_UTIL_H

#include <random>
#include <string>

#include "Vector3f.h"

namespace RT {

class RNG {
public:
    RNG() noexcept;
    float RandUniformFloat();
    float RandNormalFloat();
    float RandTentFloat();
    Vector3f RandNormalizedVector();

private:
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<float> uniform_dist{0, 1};
    std::normal_distribution<float> normal_dist{0, 1};
};

extern RNG rng;

inline float to_radian(float x) { return x / 180.f * (float)M_PI; }

inline float clamp(float x) { return x >= 0 ? x : 0; }

inline float clamp1(float x) {
    if (x > 1)
        return 1;
    if (x < 0)
        return 0;
    return x;
}

inline Vector3f parse_vector(const std::string &str) {
    Vector3f v;
    const char *start = str.c_str();
    char *end = nullptr;
    for (int i = 0; i < 3; i++) {
        v[i] = std::strtof(start, &end);
        start = end + 1;
    }
    return v;
}

inline Vector3f clamp1(Vector3f v) { return {clamp1(v.x()), clamp1(v.y()), clamp1(v.z())}; }

inline float tri_det(const Vector3f &v0, const Vector3f &v1, const Vector3f &v2) {
    return v0[0] * (v1[1] * v2[2] - v1[2] * v2[1]) - v0[1] * (v1[0] * v2[2] - v1[2] * v2[0]) +
           v0[2] * (v1[0] * v2[1] - v1[1] * v2[0]);
}

inline Vector3f gamma_correct(const Vector3f &v, float gamma) {
    return {
        std::pow(v.x(), 1 / gamma),
        std::pow(v.y(), 1 / gamma),
        std::pow(v.z(), 1 / gamma),
    };
}

inline float fsquare(float x) { return x * x; }

} // namespace RT

#endif // RT_MATH_UTIL_H
