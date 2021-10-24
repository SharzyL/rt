#ifndef CAMERA_H
#define CAMERA_H

#include "ray.hpp"
#include <cfloat>
#include <cmath>
#include <vecmath.h>

class Camera {
public:
    Camera(const Vector3f &center, const Vector3f &direction, const Vector3f &up, int imgW, int imgH) {
        this->center = center;
        this->direction = direction.normalized();
        this->horizontal = Vector3f::cross(this->direction, up);
        this->up = Vector3f::cross(this->horizontal, this->direction);
        this->width = imgW;
        this->height = imgH;
    }

    // Generate rays for each screen-space coordinate
    virtual Ray generateRay(const Vector2f &point) = 0;
    virtual ~Camera() = default;

    int getWidth() const { return width; }
    int getHeight() const { return height; }

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
    PerspectiveCamera(const Vector3f &center, const Vector3f &direction, const Vector3f &up, int imgW, int imgH,
                      float angle)
        : Camera(center, direction, up, imgW, imgH) {
        // angle is in radian.
        auto w = (float) imgW, h = (float) imgH;
        right = Vector3f::cross(direction, up);
        float distToCanvas = w / 2 / std::tan(angle / 2);
        Vector3f canvasCenter = center + direction * distToCanvas;
        canvasOrigin = canvasCenter - right * (w / 2) - up * (w / 2);
    }

    Ray generateRay(const Vector2f &point) override {
        Vector3f pointOnCanvas = canvasOrigin + right * point.x() + up * point.y();
        return {center, (pointOnCanvas - center).normalized()};
    }

protected:
    Vector3f right;
    Vector3f canvasOrigin;
};

#endif // CAMERA_H
