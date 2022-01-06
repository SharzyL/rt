#include "./math_util.h"
#include "debug.h"

namespace RT {

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