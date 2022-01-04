#ifndef RT_BALL_FINDER_HPP
#define RT_BALL_FINDER_HPP

#include <map>
#include <list>
#include <tuple>
#include <functional>
#include <cmath>

#include <Vector3f.h>

#include <fmt/core.h>

namespace RT {

// This data structure stores a set of balls in space
// For a point, it can find all balls containing the point quickly
// T should have `radius` and `center` members thus behaves like a ball
template<typename T>
class BallFinder {
public:
    BallFinder() = default;

    void AddBall(T *t) {
        float radius = t->radius;
        const Vector3f &p = t->center;
        float grid_size_log = 1 + std::ceil(std::log2(radius));
        auto grid_size = (float) std::pow(2.f, grid_size_log);
        float x = p.x() / grid_size, y = p.y() / grid_size, z = p.z() / grid_size;
        int xf = std::floor(x), yf = std::floor(y), zf = std::floor(z);
        fmt::print("insert ({}, {}, {}) / {} (r = {}): ({}, {}, {})\n",  t->center.x(), t->center.y(), t->center.z(), grid_size, t->radius, xf, yf, zf);
        ball_grids[grid_size_log][std::make_tuple(xf, yf, zf)].emplace_back(t);
    };

    // for each ball containing p, invoke f(center, radius)
    // and update the radius of the ball to the return value of f
    void OperateAndUpdateBalls(const Vector3f &p, const std::function<float(T*)> &f) {
        for (auto &[grid_size_log, ball_grid]: ball_grids) {
            float grid_size = (float) std::pow(2.f, grid_size_log);
            float x = p.x() / grid_size, y = p.y() / grid_size, z = p.z() / grid_size;
            float xf = std::round(x), yf = std::round(y), zf = std::round(z);
            int xi = (int) xf, yi = (int) yf, zi = (int) zf;
//            fmt::print("floored value:  ({}, {}, {})\n",  xf, yf, zf);
            fmt::print("locate ({}, {}, {}) / {}: ({}, {}, {})\n",  p.x(), p.y(), p.z(), grid_size, xi, yi, zi);
            for (int xii = xi - 1; xii <= xi; xii++) {
                for (int yii = yi - 1; yii <= yi; yii++) {
                    for (int zii = zi - 1; zii <= zi; zii++) {
                        std::list<T*> &balls_in_grid = ball_grid[std::make_tuple(xii, yii, zii)];
                        balls_in_grid.remove_if([=](T *t) -> bool {
                            float dist = (t->center - p).length();
                            fmt::print("grid ({}, {}, {}) dist: {} ({} desired)\n", xii, yii, zii, dist, t->radius);
                            if (dist <= t->radius) {
                                float new_radius = f(t);
                                if (new_radius > 0) {
                                    t->radius = new_radius;
                                    AddBall(t);
                                    return true;
                                }
                            }
                            return false;
                        });
                    }
                }
            }
        }
    };

private:
    using grid_index_t = std::tuple<int, int, int>;
    using ball_grid_t = std::map<grid_index_t, std::list<T*>>;

    std::map<int, ball_grid_t> ball_grids;
    // ball_grids[i] is a map from (x, y, z) to all balls centering in \prod [2^i x, 2^i (x + 1))
    // and radius in [2^(i-1), 2^i) i.e. half of grid width
};

}

#endif //RT_BALL_FINDER_HPP
