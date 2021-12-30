#ifndef CAMERA_H
#define CAMERA_H

#include <cfloat>
#include <cmath>

#include "ray.h"
#include "vecmath.h"

class Camera {
public:
    Camera(const Vector3f &center, const Vector3f &direction, const Vector3f &up, int imgW, int imgH);

    // Generate rays for each screen-space coordinate
    virtual Ray generateRay(const Vector2f &point) = 0;
    virtual ~Camera();

    [[nodiscard]] int getWidth() const;
    [[nodiscard]] int getHeight() const;

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
                      float angle);

    Ray generateRay(const Vector2f &point) override;

protected:
    Vector3f canvasOrigin;
};

#endif // CAMERA_H
