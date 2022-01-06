#include <gtest/gtest.h>

#include "objects/rotate_bezier.h"
#include "utils/math_util.h"
#include "utils/debug.h"

namespace RT::testing {

TEST(ParseVector, BasicTests) {
    fmt::print("{}\n", parse_vector3f("3 2 4"));
    fmt::print("{}\n", parse_vector2f("3 2"));
}

TEST(Bezier, BasicTests) {
    std::vector<Vector2f> controls{{0, 0}, {1, 1}, {0, 2.1}};
    RotateBezier rb(std::move(controls), Vector2f(0, 0), nullptr, nullptr);
    Ray ray(Vector3f(-2, 0, 0), Vector3f(1, 1, 0));
    Hit hit;
    rb.Intersect(ray, hit, 0);
    fmt::print("intersect t = {} {}, n = {}\n", hit.GetT(), hit.GetPos(), hit.GetNormal());
}

}