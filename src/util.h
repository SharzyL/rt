//
// Created by sharzy on 12/31/21.
//

#ifndef RT_UTIL_H
#define RT_UTIL_H

#include <random>
#include <string>

#include <Vector3f.h>

inline float rand_float() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dis(0, 1);
    return dis(gen);
}

inline float rand_float_tent() {
    float rd = 2 * rand_float();
    if (rd > 1.f) {
        return 1.f - std::sqrt(2.f - rd);
    } else {
        return std::sqrt(rd) - 1;
    }
}

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
    return v0[0] * (v1[1] * v2[2] - v1[2] * v2[1])
           - v0[1] * (v1[0] * v2[2] - v1[2] * v2[0])
           + v0[2] * (v1[0] * v2[1] - v1[1] * v2[0]);
}

#endif // RT_UTIL_H
