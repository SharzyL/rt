#ifndef RT_BALL_FINDER_H
#define RT_BALL_FINDER_H

#include <map>
#include <list>
#include <tuple>
#include <functional>
#include <cmath>

#include <Vector3f.h>

namespace RT {

// This data structure stores a set of balls in space
// For a point, it can find all balls containing the point quickly
// T should have `radius` and `center` members thus behaves like a ball
template<typename T>
class BallFinder {
public:
    BallFinder() = default;

    void add_ball(T &t) {
        float radius = t.radius;
        const Vector3f &p = t.center;
        int grid_size_log = 1 + (int) std::floor(std::log(radius));
        auto grid_size = (float) std::pow(2.f, grid_size_log);
        float x = p.x() / grid_size, y = p.y() / grid_size, z = p.z() / grid_size;
        int xf = std::floor(x), yf = std::floor(y), zf = std::floor(z);
        grid_index_t grid_idx = std::make_tuple(xf, yf, zf);
        ball_grids[grid_size_log][grid_idx].emplace_back(t);
    };

    // for each ball containing p, invoke f(center, radius)
    // and update the radius of the ball to the return value of f
    void operate_and_update_balls(const Vector3f &p, const std::function<float(T&)> &f) {
        for (auto &[grid_size_log, ball_grid]: ball_grids) {
            const float grid_size = std::pow(2.f, grid_size_log);
            float x = p.x() / grid_size, y = p.y() / grid_size, z = p.z() / grid_size;
            float xf = std::floor(x), yf = std::floor(y), zf = std::floor(z);
            int xi = x - xf >= 0.5 ? (int) x : (int) x - 1;
            int yi = y - yf >= 0.5 ? (int) y : (int) y - 1;
            int zi = z - zf >= 0.5 ? (int) z : (int) z - 1;
            for (int xii = xi; xii <= xi + 1; xii++) {
                for (int yii = yi; yii <= yi + 1; yii++) {
                    for (int zii = zi; zii <= zi + 1; zii++) {
                        grid_index_t grid_idx = std::make_tuple(xii, yii, zii);
                        std::list<T&> &balls_in_grid = ball_grid[grid_idx];
                        for (auto &t: balls_in_grid) {
                            if ((t.center - p).length() <= t.radius) {
                                t.radius = f(t);
                                balls_in_grid.remove(t);
                                add_ball(t);
                            }
                        }
                    }
                }
            }
        }
    };

private:
    using grid_index_t = std::tuple<int, int, int>;
    using ball_grid_t = std::map<grid_index_t, std::list<T&>>;

    std::map<int, ball_grid_t> ball_grids;
    // ball_grids[i] is a map from (x, y, z) to all balls centering in \prod [2^i x, 2^i (x + 1))
    // and radius in [2^(i-1), 2^i) i.e. half of grid width

};

}

#endif //RT_BALL_FINDER_H
