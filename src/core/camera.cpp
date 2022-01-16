#include <cmath>

#include <Vector3f.h>
#include <Vector2f.h>

#include "core/ray.h"

#include "./camera.h"
#include "utils/math_util.h"
#include "utils/debug.h"

namespace RT {

Camera::Camera(const Vector3f &center, const Vector3f &direction, const Vector3f &up, int imgW, int imgH, float shutter_time) {
    this->center = center;
    this->direction = direction.normalized();
    this->up = (up - this->direction* Vector3f::dot(up, this->direction)).normalized();
    this->right = Vector3f::cross(this->direction, this->up);
    this->up = Vector3f::cross(this->right, this->direction);
    this->width = imgW;
    this->height = imgH;
    this->shutter_time = shutter_time;
}

// Generate rays for each screen-space coordinate
Camera::~Camera() = default;

[[nodiscard]] int Camera::getWidth() const { return width; }
[[nodiscard]] int Camera::getHeight() const { return height; }

PerspectiveCamera::PerspectiveCamera(const Vector3f &center, const Vector3f &_direction, const Vector3f &_up, int imgW,
                                     int imgH, float angle, float focal_len, float aperture, float shutter_time)
    : Camera(center, _direction, _up, imgW, imgH, shutter_time), focal_len(focal_len), aperture(aperture) {
    // angle is in radian.
    auto w = (float)imgW, h = (float)imgH;
    dist_to_pixel_plane = h / 2 / std::tan(angle / 2);
    focal_scale = focal_len / dist_to_pixel_plane;  // the ratio between focal_plane / pixel_plane
    Vector3f focal_center = center + direction * focal_len;
    focal_origin = focal_center - (right * (w / 2) + up * (h / 2)) * focal_scale;
}

Ray PerspectiveCamera::generateRay(const Vector2f &point, RNG &rng) const {
    Vector3f point_on_focal = focal_origin + (right * point.x() + up * point.y()) * focal_scale;

    float x_disturb, y_disturb;
    do {
        x_disturb = 2 * rng.RandUniformFloat() - 1;
        y_disturb = 2 * rng.RandUniformFloat() - 1;
    } while (x_disturb * x_disturb + y_disturb * y_disturb > 1);

    Vector3f ray_point = center + right * aperture * x_disturb + up * aperture * y_disturb;
    return {ray_point, point_on_focal - ray_point, shutter_time * rng.RandUniformFloat()};
}

} // namespace RT
