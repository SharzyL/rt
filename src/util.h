//
// Created by sharzy on 12/31/21.
//

#ifndef RT_UTIL_H
#define RT_UTIL_H

#include <random>

#include <Vector3f.h>

inline float rand_float() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dis(0, 1);
    return dis(gen);
}

inline float to_radian(float x) {
    return x / 180.f * (float) M_PI;
}

inline float clamp(float x) { return x >= 0 ? x : 0; }

inline float clamp1(float x) {
    if (x > 1) return 1;
    if (x < 0) return 0;
    return x;
}

inline Vector3f clamp1(Vector3f v) {
    return {clamp1(v.x()), clamp1(v.y()), clamp1(v.z())};
}

#endif //RT_UTIL_H
