#ifndef CURVE_HPP
#define CURVE_HPP

#include <utility>
#include <vecmath.h>
#include <vector>

#include <algorithm>

#include <fmt/core.h>
#include <fmt/ranges.h>
#include <fmt/ostream.h>
#include <glog/logging.h>

#include "objects/object3d.h"
#include "objects/mesh.h"

namespace RT {

// The CurvePoint object stores information about a point on a curve
// after it has been tesselated: the vertex (V) and the tangent (T)
// It is the responsiblility of functions that create these objects to fill in all the data.
struct CurvePoint {
    Vector3f V; // Vertex
    Vector3f T; // Tangent  (unit)
};

class Curve : public Object3D {
protected:
    std::vector<Vector3f> controls;

public:
    explicit Curve(std::vector<Vector3f> &&points) : controls(std::move(points)) {}

    bool Intersect(const Ray &r, Hit &h, float tmin) const override { return false; }

    virtual void discretize(int resolution, std::vector<CurvePoint> &data) const = 0;
};

class BezierCurve : public Curve {
public:
    explicit BezierCurve(std::vector<Vector3f> &&points);

    void discretize(int resolution, std::vector<CurvePoint> &data) const override;
};

class BsplineCurve : public Curve {
public:
    explicit BsplineCurve(std::vector<Vector3f> &&points);

    void discretize(int resolution, std::vector<CurvePoint> &data) const override;
};

std::unique_ptr<Mesh> makeMeshFromRotateCurve(const Curve *curve, const Material *mat);

} // namespace RT
#endif // CURVE_HPP
