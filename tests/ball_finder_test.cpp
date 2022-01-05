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
    RNG rng;
    for (int i = 0; i < 100; i++) {
        BallFinder<Ball> bf(4.1);
        Vector3f center = 10 * rng.RandNormalizedVector();
        Vector3f center2 = center + 0.1 * rng.RandNormalizedVector();
        Ball b1{center, 0.48f};
        Ball b2{center2, 1.f};
        Ball b3{center + Vector3f(3, 0, 0), 2.f};
        bf.AddBall(&b1);
        bf.AddBall(&b2);
        bf.AddBall(&b3);
        {
            int num_balls = 0;
            bf.FindAndOperateBalls(center + 0.3 * rng.RandNormalizedVector(), [&](Ball *b) {
                num_balls += 1;
            });
            ASSERT_EQ(num_balls, 2);
        }

        {
            int num_balls = 0;
            bf.FindAndOperateBalls(center + Vector3f(4.9, 0, 0), [&](Ball *b) {
                num_balls += 1;
                b->radius = 0.1;
            });
            ASSERT_EQ(num_balls, 1);
        }

        {
            int num_balls = 0;
            bf.FindAndOperateBalls(center + Vector3f(3, 0.05, 0.05), [&](Ball *b) {
                num_balls += 1;
            });
            ASSERT_EQ(num_balls, 1);
        }

        {
            int num_balls = 0;
            bf.FindAndOperateBalls(center + Vector3f(3, 0.1, 0.05), [&](Ball *b) {
                num_balls += 1;
            });
            ASSERT_EQ(num_balls, 0);
        }
    }
}

} // namespace RT
