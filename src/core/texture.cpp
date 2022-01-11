#include <stdexcept>

#include <lodepng.h>

#include "texture.h"
#include "utils/debug.h"

namespace RT {

inline float int_color_to_color(uint8_t x, float gamma) {
    return std::pow((float) x / 256.f, gamma);
}

MappedTexture::MappedTexture(const std::string &filename, float gamma) {
    std::vector<uint8_t> int_texture_data;
    auto err_code = lodepng::decode(int_texture_data, width, height, filename);

    if (err_code) {
        throw std::runtime_error(fmt::format("load texture '{}' failed: {}", filename, lodepng_error_text(err_code)));
    };

    width_f = (float) width;
    height_f = (float) height;
    texture_data.reserve(width * height);
    for (uint32_t i = 0; i < width * height; i++) {
        texture_data[i] = Vector3f(
                int_color_to_color(int_texture_data[4 * i], gamma),
                int_color_to_color(int_texture_data[4 * i + 1], gamma),
                int_color_to_color(int_texture_data[4 * i + 2], gamma)
        );
    }
}

Vector3f MappedTexture::At(float u, float v) const {
    int x = (int) ((u - std::floor(u)) * width_f);
    int y = (int) ((v - std::floor(v)) * height_f);
    return texture_data[y * width + x];
}

} // namespace RT
