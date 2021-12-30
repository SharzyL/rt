#ifndef SPHERE_H
#define SPHERE_H

#include "object3d.hpp"
#include "vecmath.h"
#include <cmath>

class Sphere : public Object3D {
public:
    Sphere(const Vector3f &center, float radius, Material *material)
        : Object3D(material), center(center), radius(radius) {}

    ~Sphere() override = default;

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        const Vector3f &rd = r.getDirection();
        auto origin_to_center = center - r.getOrigin();
        float tp = Vector3f::dot(origin_to_center, rd);
        float dist_to_center = origin_to_center.length();
        float dist = std::sqrt(dist_to_center * dist_to_center - tp * tp);

        if (dist > radius) {
            return false;
        } else {
            float t_prime = sqrt(radius * radius - dist * dist);
            float t = dist_to_center > radius ? tp - t_prime : tp + t_prime;
            if (t >= tmin && t < h.getT()) {
                Vector3f intersection = r.pointAtParameter(t);
                Vector3f normal_at_intersection = (intersection - center).normalized();
                h.set(t, material, normal_at_intersection);
                return true;
            } else {
                return false;
            }
        }
    }

protected:
    const Vector3f center;
    const float radius;
};

#endif
