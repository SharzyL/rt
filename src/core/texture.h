#ifndef RT_TEXTURE_H
#define RT_TEXTURE_H

#include <string>

#include <Vector3f.h>

namespace RT {

class Texture {
public:
    [[nodiscard]] virtual Vector3f At(float u, float v) const = 0;
};

class MappedTexture: public Texture {
public:
    explicit MappedTexture(const std::string &filename);
    [[nodiscard]] Vector3f At(float u, float v) const override;
private:
    // Not supporting alpha channel yet
    std::vector<Vector3f> texture_data;
    uint32_t width, height, channels;
    float width_f, height_f;
};

} // namespace RT

#endif //RT_TEXTURE_H
