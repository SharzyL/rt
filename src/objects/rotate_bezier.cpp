#include <tuple>

#include "./rotate_bezier.h"
#include "utils/debug.h"

namespace RT {

RotateBezier::RotateBezier(std::vector<Vector2f> &&controls_points, Vector2f axis, Material *mat, Texture *texture) :
        SimpleObject3D(mat, texture), controls(std::move(controls_points)), axis(axis) {
    ymin = controls[0].y();
    ymax = controls.back().y();
}

bool RotateBezier::Intersect(const Ray &ray, Hit &hit, float tmin) const {
    const auto &orig = ray.GetOrigin();
    float x0 = orig.x() - axis.x(), y0 = orig.y(), z0 = orig.z() - orig.y();
    const auto &dir = ray.GetDirection();
    float rx = dir.x(), ry = dir.y(), rz = dir.z();

    float t = 0;  // initial value
    int iter_times = 0;
    Vector2f b, b_deriv;  // left out of while loop, for future use

    // Newton iteration
    while (true) {
        iter_times++;
        if (iter_times > 100) break;

        std::tie(b, b_deriv) = bezier_evaluate(t);
        float yt = b.y(), xt = b.x();
        float yt_deriv = b_deriv.y(), xt_deriv = b_deriv.x();
        float ft = fsquare(x0 + rx * (yt - y0) / ry) + fsquare(z0 + rz * (yt - y0) / ry) - fsquare(xt);

        float ft_deriv = 0;
        ft_deriv += 2 * (rx / ry) * (x0 + rx * (yt - y0) / ry) * yt_deriv;
        ft_deriv += 2 * (rz / ry) * (z0 + rz * (yt - y0) / ry) * yt_deriv;
        ft_deriv -= 2 * xt * xt_deriv;
        float t_delta = ft / ft_deriv;
        fmt::print("{} = {} - {} ({} / {})\n", t - t_delta, t, t_delta, ft, ft_deriv);
        t = t - t_delta;  // t_{n + 1} = t_n - f(t) / f'(t)
        if (std::abs(t_delta) < 0.001) break;
    }
    float ray_t = (b.y() - y0) / ry;
    auto hit_point = ray.PointAtParameter(ray_t);
    auto hit_point_to_axis = Vector2f(hit_point.x() - axis.x(), hit_point.z() - axis.y()).normalized();
    auto normal = Vector3f(
            hit_point_to_axis.x() * b_deriv.y(),
            -b_deriv.x(),
            hit_point_to_axis.y() * b_deriv.y());
    hit.Set(ray_t, material, normal.normalized(), hit_point, nullptr);
    return true;
}

[[nodiscard]] Vector3f RotateBezier::AmbientColorAtHit(const Hit &hit) const {
    throw std::runtime_error("bezier texture implemented");
}

std::pair<Vector2f, Vector2f> RotateBezier::bezier_evaluate(float bt) const {
    if (bt < 0) {
        return {Vector2f(controls[0].x(), ymin + bt * (ymax - ymin)), Vector2f(0, ymax - ymin)};
    } else if (bt > 1) {
        return {Vector2f(controls.back().x(), ymin + bt * (ymax - ymin)), Vector2f(0, ymax - ymin)};
    }
    std::vector<Vector2f> val;
    std::vector<Vector2f> deriv;
    size_t n = controls.size();
    val.resize(n - 1);
    deriv.resize(n - 1);
    for (size_t i = 0; i < n - 1; i++) {
        val[i] = (1 - bt) * controls[i] + bt * controls[i + 1];
        deriv[i] = controls[i + 1] - controls[i];
    }
    for (size_t iter = n - 2; iter > 0; iter--) {
        for (size_t i = 0; i < iter; i++) {
            auto vi = val[i], vi1 = val[i + 1];
            auto di = deriv[i], di1 = deriv[i + 1];
            val[i] = (1 - bt) * vi + bt * vi1;
            deriv[i] = (1 - bt) * di - vi + bt * di1 + vi1;
        }
    }
    return {val[0], deriv[0]};
}

} // namespace RT