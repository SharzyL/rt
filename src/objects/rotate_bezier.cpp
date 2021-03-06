#include <tuple>
#include <fmt/color.h>

#include "./rotate_bezier.h"
#include "utils/debug.h"
#include "core/material.h"

namespace RT {

static RNG rng;

RotateBezier::RotateBezier(std::vector<Vector2f> &&controls_points, Vector2f axis, Material *mat, Texture *texture) :
        SimpleObject3D(mat, texture), controls(std::move(controls_points)), axis(axis) {
    yfirst = controls[0].y();
    ylast = controls.back().y();

    float ymin = std::numeric_limits<float>::max();
    float ymax = -std::numeric_limits<float>::max();
    float xmax = -std::numeric_limits<float>::max();
    for (const auto &cp: controls_points) {
        ymin = std::min(ymin, cp.y());
        ymax = std::max(ymax, cp.y());
        xmax = std::max(xmax, std::abs(cp.x()));
    }
    box.AddVertex({axis.x() - xmax, ymin, axis.y() - xmax});
    box.AddVertex({axis.x() + xmax, ymax, axis.y() + xmax});

    surrounding_mesh = MakeMesh(mat, texture, 30, 30);
}

bool RotateBezier::Intersect(const Ray &ray, Hit &hit, float tmin) const {
    const auto &orig = ray.GetOrigin();
    float x0 = orig.x() - axis.x(), y0 = orig.y(), z0 = orig.z() - axis.y();
    const auto &dir = ray.GetDirection();
    float rx = dir.x(), ry = dir.y(), rz = dir.z();

//    LOG(ERROR) << fmt::format(fg(fmt::color::purple), "find intersect orig={}, dir={}", orig, dir);

    // Newton iteration
    Hit fake_hit = hit;
    bool hit_mesh = surrounding_mesh->Intersect(ray, fake_hit, tmin);
    if (hit_mesh && tmin < fake_hit.GetT() && fake_hit.GetT() < hit.GetT()) {
        bool found = false;
        int iter_times = 0;
        float t = (hit.GetPos().y() - yfirst) / (ylast - yfirst);
        Vector2f b, b_deriv;  // left out of while loop, for future use
        while (true) {
            iter_times++;

            std::tie(b, b_deriv) = bezier_evaluate(t, 0, 1);
            float yt = b.y(), xt = b.x();
            float yt_deriv = b_deriv.y(), xt_deriv = b_deriv.x();
            float ft = fsquare(x0 + rx * (yt - y0) / ry) + fsquare(z0 + rz * (yt - y0) / ry) - fsquare(xt);

            float ft_deriv = 0;
            ft_deriv += 2 * (rx / ry) * (x0 + rx * (yt - y0) / ry) * yt_deriv;
            ft_deriv += 2 * (rz / ry) * (z0 + rz * (yt - y0) / ry) * yt_deriv;
            ft_deriv -= 2 * xt * xt_deriv;
            float t_delta = ft / ft_deriv;
//            LOG(ERROR) << fmt::format("iterate {}: {} = {} - {} ({} / {}) ({}, {})\n",
//                                      iter_times, t - t_delta, t, t_delta, ft, ft_deriv, b, b_deriv);
            t = t - t_delta;  // t_{n + 1} = t_n - f(t) / f'(t)
            if (std::abs(ft) < 0.0001) {
                found = true;
                break;
            };
            if (iter_times > 10) break;
        }

        float ray_t = (b.y() - y0) / ry;
        if (found && tmin <= ray_t && ray_t < hit.GetT() && 0 <= t && t <= 1) {
            auto hit_point = ray.PointAtParameter(ray_t);
            auto hit_point_to_axis = Vector2f(hit_point.x() - axis.x(), hit_point.z() - axis.y()).normalized();
            auto normal = Vector3f(
                    hit_point_to_axis.x() * b_deriv.y(),
                    -b_deriv.x(),
                    hit_point_to_axis.y() * b_deriv.y());
            auto color = material->ambientColor;
            // TODO: texture
            hit.Set(ray_t, material, normal.normalized(), hit_point, color, nullptr);
//            LOG(ERROR) << fmt::format(fg(fmt::color::blue), "hit {} (ray_t = {}, normal = {})", hit_point, ray_t, normal);
        } else {
//            LOG(ERROR) << fmt::format(fg(fmt::color::red), "not found since ray_t = {}, t = {}", ray_t, t);
        }
        return found;
    } else {
        return false;
    }
}

std::pair<Vector2f, Vector2f> RotateBezier::bezier_evaluate(float bt, float min_t, float max_t) const {
    if (bt < min_t - 0.0001) {
        return {Vector2f(controls[0].x(), yfirst + bt * (ylast - yfirst)), Vector2f(0, ylast - yfirst)};
    } else if (bt > max_t + 0.0001) {
        return {Vector2f(controls.back().x(), yfirst + bt * (ylast - yfirst)), Vector2f(0, ylast - yfirst)};
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


std::unique_ptr<Mesh> RotateBezier::MakeMesh(const Material *mat, const Texture *tex, int density_x, int density_y) const {
    using Tup3u = std::tuple<unsigned, unsigned, unsigned>;  // idx_a, idx_b, idx_c, extra
    std::vector<Vector3f> vertices;
    std::vector<Vector2f> curve_points;
    std::vector<Vector2f> curve_tangents;
    curve_points.reserve(density_y);
    for (int i = 0; i < density_y; i++) {
        auto [x, dx] = bezier_evaluate((float) i / (float) (density_y - 1));
        curve_points.emplace_back(x);
        curve_tangents.emplace_back(dx);
    }

    int points_num = density_x * density_y;
    std::vector<Vector3f> normals; normals.reserve(points_num);
    std::vector<Tup3u> indices; indices.reserve(points_num);
    std::vector<Vector2f> tex_coord; tex_coord.reserve(points_num);

    for (int ci = 0; ci < density_y; ++ci) {
        Vector2f cp = curve_points[ci];
        for (int i = 0; i < density_x; ++i) {
            float angle = (float) i / (float) density_x * 2 * (float) M_PI;
            Vector3f pnew{
                cp.x() * std::cos(angle) + axis.x(),
                cp.y(),
                cp.x() * std::sin(angle) + axis.y()};
            vertices.emplace_back(pnew);

            Vector2f cp_normalized = cp.normalized();
            Vector2f dx = curve_tangents[ci];
            Vector3f normal{
                std::cos(angle) * dx.y(),
                -dx.x(),
                std::sin(angle) * dx.y()
            };
            normals.emplace_back(normal.normalized());
            tex_coord.emplace_back(Vector2f{
                (float) i / (float) density_x,
                (float) ci / (float) density_y
            });
            int i1 = (i + 1 == density_x) ? 0 : i + 1;
            if (ci != curve_points.size() - 1) {
                indices.emplace_back((ci + 1) * density_x + i, ci * density_x + i1, ci * density_x + i);
                indices.emplace_back((ci + 1) * density_x + i, (ci + 1) * density_x + i1, ci * density_x + i1);
            }
        }
    }

    std::vector<Triangle> triangles;
    for (auto & i : indices) {
        Triangle &tri = triangles.emplace_back(
                vertices[std::get<0>(i)],
                vertices[std::get<1>(i)],
                vertices[std::get<2>(i)],
                mat, tex
        );
        tri.SetVertexNormal(
                normals[std::get<0>(i)],
                normals[std::get<1>(i)],
                normals[std::get<2>(i)]
        );
        tri.SetTextureCoord(
                tex_coord[std::get<0>(i)],
                tex_coord[std::get<1>(i)],
                tex_coord[std::get<2>(i)]
        );
    }

    return std::make_unique<Mesh>(std::move(triangles));
};
} // namespace RT