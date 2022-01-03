#ifndef RT_TEXTURE_H
#define RT_TEXTURE_H

#include <string>

#include <Vector3f.h>

namespace RT {

class Texture {
public:
    [[nodiscard]] virtual Vector3f At(float u, float v) const = 0;
    virtual ~Texture() = default;
};

class MappedTexture: public Texture {
public:
    explicit MappedTexture(const std::string &filename, float gamma);
    [[nodiscard]] Vector3f At(float u, float v) const override;

    ~MappedTexture() override = default;
private:
    // Not supporting alpha channel yet
    std::vector<Vector3f> texture_data;
    uint32_t width, height, channels;
    float width_f, height_f;
};

} // namespace RT

#endif //RT_TEXTURE_H
