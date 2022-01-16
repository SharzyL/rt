#ifndef RT_CAMERA_H
#define RT_CAMERA_H

#include <Vector2f.h>
#include <Vector3f.h>

#include "utils/math_util.h"

namespace RT {

class Ray;

class Camera {
public:
    Camera(const Vector3f &center, const Vector3f &direction, const Vector3f &up, int imgW, int imgH, float shutter_time);

    // Generate rays for each screen-space coordinate
    [[nodiscard]] virtual Ray generateRay(const Vector2f &point, RNG &rng) const = 0;
    virtual ~Camera();

    [[nodiscard]] int getWidth() const;
    [[nodiscard]] int getHeight() const;

protected:
    // Extrinsic parameters
    Vector3f center;
    Vector3f direction;
    Vector3f up;
    Vector3f right;
    // Intrinsic parameters
    int width;
    int height;

    float shutter_time = 0;
};

class PerspectiveCamera : public Camera {

public:
    PerspectiveCamera(const Vector3f &center, const Vector3f &_direction, const Vector3f &_up, int imgW, int imgH,
                      float angle, float focal_len, float aperture, float shutter_time);

    [[nodiscard]] Ray generateRay(const Vector2f &point, RNG &rng) const override;

protected:
    Vector3f focal_origin;
    float dist_to_pixel_plane, focal_len, aperture, focal_scale;
};

} // namespace RT

#endif // RT_CAMERA_H
