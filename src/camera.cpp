#include "./camera.h"

Camera::Camera(const Vector3f &center, const Vector3f &direction, const Vector3f &up, int imgW, int imgH) {
    this->center = center;
    this->direction = direction.normalized();
    this->horizontal = Vector3f::cross(this->direction, up).normalized();
    this->up = Vector3f::cross(this->horizontal, this->direction);
    this->width = imgW;
    this->height = imgH;
}

// Generate rays for each screen-space coordinate
Camera::~Camera() = default;

[[nodiscard]] int Camera::getWidth() const { return width; }
[[nodiscard]] int Camera::getHeight() const { return height; }

PerspectiveCamera::PerspectiveCamera(const Vector3f &center, const Vector3f &_direction, const Vector3f &_up, int imgW,
                                     int imgH, float angle)
    : Camera(center, _direction, _up, imgW, imgH) {
    // angle is in radian.
    auto w = (float)imgW, h = (float)imgH;
    float distToCanvas = h / 2 / std::tan(angle / 2);
    Vector3f canvasCenter = center + direction * distToCanvas;
    canvasOrigin = canvasCenter - horizontal * (w / 2) - up * (w / 2);
}

Ray PerspectiveCamera::generateRay(const Vector2f &point) {
    Vector3f pointOnCanvas = canvasOrigin + horizontal * point.x() + up * point.y();
    return {center, pointOnCanvas - center};
}
