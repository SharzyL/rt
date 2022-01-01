//
// Created by sharzy on 12/31/21.
//

#ifndef RT_UTIL_H
#define RT_UTIL_H

#include <random>

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

#endif //RT_UTIL_H
