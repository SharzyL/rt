#include "./aabb.h"

namespace RT {

AABB::AABB() {
    Reset();
}

void AABB::Reset() {
    x0 = y0 = z0 = std::numeric_limits<float>::max();
    x1 = y1 = z1 = -std::numeric_limits<float>::max();
    num_v = 0;
    x_sum = y_sum = z_sum = 0.f;
}

bool AABB::IsNull() const {
    return num_v == 0;
}

void AABB::AddVertex(const Vector3f &v) {
    num_v ++;
    x0 = std::min(x0, v.x());
    x1 = std::max(x1, v.x());
    y0 = std::min(y0, v.y());
    y1 = std::max(y1, v.y());
    z0 = std::min(z0, v.z());
    z1 = std::max(z1, v.z());
    x_sum += v.x();
    y_sum += v.y();
    z_sum += v.z();
    center = {x_sum / (float) num_v, y_sum / (float) num_v, z_sum / (float) num_v};
}

bool AABB::MayIntersect(const Ray &ray, float tmin, float tmax) const {
    if (num_v == 0) return true;
    const Vector3f &dir = ray.GetDirection();
    const Vector3f &origin = ray.GetOrigin();
    float intersect_x0 = (x0 - origin.x()) / dir.x();
    float intersect_x1 = (x1 - origin.x()) / dir.x();
    if (intersect_x0 > intersect_x1) std::swap(intersect_x0, intersect_x1);
    float intersect_y0 = (y0 - origin.y()) / dir.y();
    float intersect_y1 = (y1 - origin.y()) / dir.y();
    if (intersect_y0 > intersect_y1) std::swap(intersect_y0, intersect_y1);
    float intersect_z0 = (z0 - origin.z()) / dir.z();
    float intersect_z1 = (z1 - origin.z()) / dir.z();
    if (intersect_z0 > intersect_z1) std::swap(intersect_z0, intersect_z1);
    float into = std::max(intersect_x0, std::max(intersect_y0, intersect_z0));
    float out = std::min(intersect_x1, std::min(intersect_y1, intersect_z1));
    return tmin <= into && into <= out && out < tmax;
}

const Vector3f& AABB::Center() const {
    return center;
}

void AABB::FitBox(const AABB &box) {
    num_v += box.num_v;
    x0 = std::min(x0, box.x0);
    x1 = std::max(x1, box.x1);
    y0 = std::min(y0, box.y0);
    y1 = std::max(y1, box.y1);
    z0 = std::min(z0, box.z0);
    z1 = std::max(z1, box.z1);
    x_sum += box.x_sum;
    y_sum += box.y_sum;
    z_sum += box.z_sum;
    center = {x_sum / (float) num_v, y_sum / (float) num_v, z_sum / (float) num_v};
}

int AABB::MaxSpanAxis() const {
    float x_span = x1 - x0;
    float y_span = y1 - y0;
    float z_span = z1 - z0;
    if (x_span >= y_span && x_span >= z_span) {
        return 0;
    } else if (y_span >= z_span) {
        return 1;
    } else {
        return 2;
    }
}

}