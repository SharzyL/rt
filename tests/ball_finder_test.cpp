#include <gtest/gtest.h>

#include <Vector3f.h>

#include "utils/ball_finder.hpp"
#include "utils/math_util.h"

namespace RT::testing {

struct Ball {
    Vector3f center;
    float radius;
};

TEST(BallFinder, BasicTests) {
    for (int i = 0; i < 100; i++) {
        BallFinder<Ball> bf;
        Vector3f center = 10 * rng.RandNormalizedVector();
        Ball b1{center, 0.48f};
        Ball b2{center + 0.1 * rng.RandNormalizedVector(), 1.f};
        Ball b3{center + Vector3f(3, 0, 0), 2.f};
        bf.AddBall(&b1);
        bf.AddBall(&b2);
        bf.AddBall(&b3);
        int num_balls = 0;
        bf.OperateAndUpdateBalls(center + 0.3 * rng.RandNormalizedVector(), [&](Ball *b) {
            num_balls += 1;
            return -1;
        });
        ASSERT_EQ(num_balls, 2);
        bf.OperateAndUpdateBalls(center + Vector3f(4.9, 0, 0), [&](Ball *b) {
            num_balls += 1;
            return -1;
        });
        ASSERT_EQ(num_balls, 3);
        bf.OperateAndUpdateBalls(center + Vector3f(4.9, 1, 0), [&](Ball *b) {
            num_balls += 1;
            return -1;
        });
        ASSERT_EQ(num_balls, 3);
        bf.OperateAndUpdateBalls(center + Vector3f(4, 1, 0), [&](Ball *b) {
            num_balls += 1;
            return 0.01;
        });
        ASSERT_EQ(num_balls, 4);
        bf.OperateAndUpdateBalls(center + Vector3f(3, 0, 0.009), [&](Ball *b) {
            num_balls += 1;
            return -1;
        });
        ASSERT_EQ(num_balls, 5);
    }
}

} // namespace RT
