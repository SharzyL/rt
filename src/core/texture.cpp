#include <stdexcept>

#include <lodepng.h>

#include "texture.h"
#include "debug.h"

namespace RT {

inline float int_color_to_color(uint8_t x) {
    return (float) x / 256.f;
}

MappedTexture::MappedTexture(const std::string &filename) {
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
                int_color_to_color(int_texture_data[4 * i]),
                int_color_to_color(int_texture_data[4 * i + 1]),
                int_color_to_color(int_texture_data[4 * i + 2])
        );
    }
}

Vector3f MappedTexture::At(float u, float v) const {
    int x = (int) ((u - 0.0000001) * width_f);
    int y = (int) ((v - 0.0000001) * height_f);
    return texture_data[y * width + x];
}

} // namespace RT
