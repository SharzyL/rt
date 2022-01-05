#ifndef RT_BALL_FINDER_HPP
#define RT_BALL_FINDER_HPP

#include <map>
#include <vector>
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
    explicit BallFinder(float grid_size): grid_size(grid_size) {};

    void AddBall(T *t) {
        assert(t->radius * 2 <= grid_size);
        const Vector3f &p = t->center;
        float x = p.x() / grid_size, y = p.y() / grid_size, z = p.z() / grid_size;
        int xf = std::floor(x), yf = std::floor(y), zf = std::floor(z);
        ball_grid[std::make_tuple(xf, yf, zf)].emplace_back(t);
    };

    // for each ball containing p, invoke f(center, radius)
    // and update the radius of the ball to the return value of f
    // IMPORTANT: the caller should only reduce the radius of stored balls, DO NOT ENLARGE
    void FindAndOperateBalls(const Vector3f &p, const std::function<void(T*)> &f) const {
        float x = p.x() / grid_size, y = p.y() / grid_size, z = p.z() / grid_size;
        float xf = std::round(x), yf = std::round(y), zf = std::round(z);
        int xi = (int) xf, yi = (int) yf, zi = (int) zf;
        for (int xii = xi - 1; xii <= xi; xii++) {
            for (int yii = yi - 1; yii <= yi; yii++) {
                for (int zii = zi - 1; zii <= zi; zii++) {
                    auto grid_idx = std::make_tuple(xii, yii, zii);
                    if (ball_grid.count(grid_idx) > 0) {
                        for (auto t: ball_grid.at(grid_idx)) {
                            if ((t->center - p).length() <= t->radius) {
                                f(t);
                            } // if dist
                        } // for balls
                    } // if grid not empty
                } // for zii
            } // for yii
        } // for xii
    };

    void Reset() {
        ball_grid.clear();
    }

private:
    using grid_index_t = std::tuple<int, int, int>;
    using ball_grid_t = std::map<grid_index_t, std::vector<T*>>;

    ball_grid_t ball_grid;
    float grid_size;
};

}

#endif //RT_BALL_FINDER_HPP
