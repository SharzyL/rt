#include "./curve.h"

namespace RT {

template<typename T>
std::vector<T> slice(std::vector<T> const &v, size_t m, size_t len) {
    auto first = v.cbegin() + m;
    auto last = v.cbegin() + m + len;

    std::vector<T> vec(first, last);
    return vec;
}

BezierCurve::BezierCurve(std::vector<Vector3f> &&points) : Curve(std::move(points)) {
    if (controls.size() < 4 || controls.size() % 3 != 1) {
        printf("Number of control points of BezierCurve must be 3n+1!\n");
        exit(0);
    }
}

void BezierCurve::discretize(int resolution, std::vector<CurvePoint> &data) const {
    data.clear();
    size_t N = controls.size() - 1;
    resolution = resolution * (int) N;

    float x_step = 1.f / (float) (resolution - 1);
    std::vector<float> coef_buf(N * (N + 1) * resolution);  // 1 <= n <= N, 0 <= i <= n, 0 <= xi < resolution
    auto coef = [&](size_t n, size_t i, size_t xi) -> float & {
        return coef_buf[(n - 1) * (N + 1) * resolution + i * resolution + xi];
    };

    // init for n = 1
    for (int xi = 0; xi < resolution; xi++) {
        float x = (float) xi * x_step;
        coef(1, 0, xi) = 1 - x;
        coef(1, 1, xi) = x;
    }

    // recursion
    for (int n = 2; n <= N; n++) {
        for (int i = 0; i <= n; i++) {
            for (int xi = 0; xi < resolution; xi++) {
                float x = (float) xi * x_step;
                float term1 = i == n ? 0 : coef(n - 1, i, xi);
                float term2 = i == 0 ? 0 : coef(n - 1, i - 1, xi);
                coef(n, i, xi) = (1 - x) * term1 + x * term2;
            }
        }
    }

    // calculate derivative
    std::vector<float> derivative_buf((N + 1) * resolution);
    auto derivative = [&](size_t i, size_t xi) -> float & { return derivative_buf[i * resolution + xi]; };
    for (int i = 0; i <= N; i++) {
        for (int xi = 0; xi < resolution; xi++) {
            float term1 = i == N ? 0 : coef(N - 1, i, xi);
            float term2 = i == 0 ? 0 : coef(N - 1, i - 1, xi);
            derivative(i, xi) = (float) N * (term1 - term2);
        }
    }

    for (int x_idx = 0; x_idx < resolution; x_idx++) {
        Vector3f p, t;
        for (int i = 0; i <= N; i++) {
            p += controls[i] * coef(N, i, x_idx);
            t += controls[i] * derivative(i, x_idx);
        }
        data.emplace_back(CurvePoint{p, -t.normalized()});
    }
}

BsplineCurve::BsplineCurve(std::vector<Vector3f> &&points) : Curve(std::move(points)) {
    if (points.size() < 4) {
        printf("Number of control points of BspineCurve must be more than 4!\n");
        exit(0);
    }
}

void BsplineCurve::discretize(int resolution, std::vector<CurvePoint> &data) const {
    data.clear();
    const int K = 4;
    const size_t N = controls.size() - 1;
    resolution = resolution * (int) (N + 3 - K);
    const float x_step = 1.f / (float) (resolution - 1);
    float t_step = 1.f / (float) (N + K);

    std::vector<std::vector<float>> coef_buf(K);
    for (int k = 1; k <= K; k++) {
        coef_buf[k - 1].resize((N + K - k + 1) * resolution);
    }

    // 1 <= k <= K, 0 <= t_idx <= N + K - k, 0 <= x_idx < resolution
    auto coef = [&](size_t k, size_t t_idx, size_t x_idx) -> float & {
        return coef_buf.at(k - 1).at(t_idx * resolution + x_idx);
    };
    std::vector<float> ti(N + K + 1), xi(resolution);
    for (int i = 0; i < N + K + 1; i++) ti[i] = (float) i / (float) (N + K);
    for (int i = 0; i < resolution; i++)
        xi[i] = ti[K - 1] + (float) i / (float) resolution * (ti[N + 1] - ti[K - 1]);

    // initialize array
    for (int x_idx = 0; x_idx < resolution; x_idx++) {
        for (int t_idx = 0; t_idx < N + K + 1; t_idx++) {
            if (ti[t_idx] < xi[x_idx] && xi[x_idx] <= ti[t_idx + 1]) {
                coef(1, t_idx, x_idx) = 1.f;
            }
        }
    }

    // recurse
    for (int k = 2; k <= K; k++) {
        for (int t = 0; t <= N + K - k; t++) {
            for (int x_idx = 0; x_idx < resolution; x_idx++) {
                float x = xi[x_idx];
                float w1 = (x - ti[t]) / (ti[t + k - 1] - ti[t]);
                float w2 = (ti[t + k] - x) / (ti[t + k] - ti[t + 1]);
                coef(k, t, x_idx) = w1 * coef(k - 1, t, x_idx) + w2 * coef(k - 1, t + 1, x_idx);
            }
        }
    }

    std::vector<float> derivative_buf(resolution * (N + 1));
    auto derivative = [&](size_t i, size_t xi) -> float & { return derivative_buf[i * resolution + xi]; };
    for (int i = 0; i <= N; i++) {
        for (int x_idx = 0; x_idx < resolution; x_idx++) {
            derivative(i, x_idx) = (K - 1) / (ti[i + K - 1] - ti[i]) * coef(K - 1, i, x_idx) -
                                   (K - 1) / (ti[i + K] - ti[i + 1]) * coef(K - 1, i + 1, x_idx);
        }
    }
    for (int x_idx = 0; x_idx < resolution; x_idx++) {
        Vector3f p, t;
        for (int i = 0; i <= N; i++) {
            p += controls[i] * coef(K, i, x_idx);
            t += controls[i] * derivative(i, x_idx);
        }
        data.emplace_back(CurvePoint{p, t.normalized()});
    }
}

std::unique_ptr<Mesh> makeMeshFromRotateCurve(const Curve *curve, const Material *mat) {
    std::vector<Triangle> triangles;

    using Tup3u = std::tuple<unsigned, unsigned, unsigned>;
    // Surface is just a struct that contains vertices, normals, and
    // faces.  VV[i] is the position of vertex i, and VN[i] is the normal
    // of vertex i.  A face is a triple i,j,k corresponding to a triangle
    // with (vertex i, normal i), (vertex j, normal j), ...
    // Currently this struct is computed every time when canvas refreshes.
    // You can store this as member function to accelerate rendering.

    std::vector<Vector3f> VV;
    std::vector<Vector3f> VN;
    std::vector<Tup3u> VF;

    std::vector<CurvePoint> curvePoints;
    curve->discretize(30, curvePoints);
    const int steps = 40;
    for (int ci = 0; ci < curvePoints.size(); ++ci) {
        const CurvePoint &cp = curvePoints[ci];
        for (int i = 0; i < steps; ++i) {
            float t = (float)i / steps;
            Quat4f rot;
            rot.setAxisAngle(t * 2 * M_PI, Vector3f::UP);
            Vector3f pnew = Matrix3f::rotation(rot) * cp.V;
            Vector3f pNormal = Vector3f::cross(cp.T, -Vector3f::FORWARD);
            Vector3f nnew = Matrix3f::rotation(rot) * pNormal;
            VV.push_back(pnew);
            VN.push_back(nnew);
            int i1 = (i + 1 == steps) ? 0 : i + 1;
            if (ci != curvePoints.size() - 1) {
                VF.emplace_back((ci + 1) * steps + i, ci * steps + i1, ci * steps + i);
                VF.emplace_back((ci + 1) * steps + i, (ci + 1) * steps + i1, ci * steps + i1);
            }
        }
    }

    for (auto & i : VF) {
        Triangle &tri = triangles.emplace_back(
                VV[std::get<0>(i)],
                VV[std::get<1>(i)],
                VV[std::get<2>(i)],
                mat
        );
    }

    return std::make_unique<Mesh>(std::move(triangles), mat);
};

} // namespace RT
