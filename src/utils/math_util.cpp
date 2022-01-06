#include "./math_util.h"
#include "debug.h"

namespace RT {

// fast random number generator copied from smallppm
static int primes[61] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79,
        83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181,
        191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283
};

inline int rev(const int i, const int p) {
    if (i == 0) return i; else return p - i;
}

float fast_uniform_float(int b, int j) {
    const int p = primes[b];
    float h = 0.0f, f = 1.0f / (float) p, fct = f;
    while (j > 0) {
        h += (float) rev(j % p, p) * fct;
        j /= p;
        fct *= f;
    }
    return h;
}

RNG::RNG() noexcept: gen(rd()) {}

float RNG::RandUniformFloat() {
    return uniform_dist(gen);
}

float RNG::RandNormalFloat() {
    return normal_dist(gen);
}

float RNG::RandTentFloat() {
    float r = 2 * RandUniformFloat();
    if (r > 1.f) {
        return 1.f - std::sqrt(2.f - r);
    } else {
        return std::sqrt(r) - 1;
    }
}

Vector3f RNG::RandNormalizedVector() {
    return Vector3f{RandNormalFloat(), RandNormalFloat(), RandNormalFloat()}.normalized();
}

Vector3f parse_vector3f(const std::string &str) {
    Vector3f v;
    const char *start = str.c_str();
    char *end = nullptr;
    for (int i = 0; i < 3; i++) {
        v[i] = std::strtof(start, &end);
        CHECK(start != end);
        start = end + 1;
    }
    return v;
}

Vector2f parse_vector2f(const std::string &str) {
    Vector2f v;
    const char *start = str.c_str();
    char *end = nullptr;
    for (int i = 0; i < 2; i++) {
        v[i] = std::strtof(start, &end);
        CHECK(start != end);
        start = end + 1;
    }
    return v;
}

} // namespace RT