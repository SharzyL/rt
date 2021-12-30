#ifndef CAMERA_H
#define CAMERA_H

#include "ray.hpp"
#include "vecmath.h"
#include <cfloat>
#include <cmath>

class Camera {
public:
    Camera(const Vector3f &center, const Vector3f &direction, const Vector3f &up, int imgW, int imgH) {
        this->center = center;
        this->direction = direction.normalized();
        this->horizontal = Vector3f::cross(this->direction, up).normalized();
        this->up = Vector3f::cross(this->horizontal, this->direction);
        LOG(ERROR) << fmt::format("direction {}, up {}, horizontal {}", this->direction.length(), this->up.length(),
                                  this->horizontal.length());
        this->width = imgW;
        this->height = imgH;
    }

    // Generate rays for each screen-space coordinate
    virtual Ray generateRay(const Vector2f &point) = 0;
    virtual ~Camera() = default;

    [[nodiscard]] int getWidth() const { return width; }
    [[nodiscard]] int getHeight() const { return height; }

protected:
    // Extrinsic parameters
    Vector3f center;
    Vector3f direction;
    Vector3f up;
    Vector3f horizontal;
    // Intrinsic parameters
    int width;
    int height;
};

class PerspectiveCamera : public Camera {

public:
    PerspectiveCamera(const Vector3f &center, const Vector3f &_direction, const Vector3f &_up, int imgW, int imgH,
                      float angle)
        : Camera(center, _direction, _up, imgW, imgH) {
        // angle is in radian.
        auto w = (float)imgW, h = (float)imgH;
        float distToCanvas = h / 2 / std::tan(angle / 2);
        Vector3f canvasCenter = center + direction * distToCanvas;
        canvasOrigin = canvasCenter - horizontal * (w / 2) - up * (w / 2);
    }

    Ray generateRay(const Vector2f &point) override {
        Vector3f pointOnCanvas = canvasOrigin + horizontal * point.x() + up * point.y();
        return {center, pointOnCanvas - center};
    }

protected:
    Vector3f canvasOrigin;
};

#endif // CAMERA_H
