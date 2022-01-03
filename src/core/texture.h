#ifndef RT_TEXTURE_H
#define RT_TEXTURE_H

#include <string>

#include <Vector3f.h>

namespace RT {

class Texture {
public:

    explicit Texture(const std::string &filename);

    Vector3f At(float u, float v) const;

private:
    // Not supporting alpha channel yet
    std::vector<Vector3f> texture_data;
    uint32_t width, height, channels;
    float width_f, height_f;
};

} // namespace RT

#endif //RT_TEXTURE_H
