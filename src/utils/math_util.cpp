#include "./math_util.h"

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

RNG rng;

} // namespace RT