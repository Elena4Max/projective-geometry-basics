#include <Eigen/Dense>
#include <cassert>
#include <projective/homography_estimation.hpp>

namespace projective {

std::pair<std::vector<geometry::Point2D>, core::Mat3> normalizePoints(
    const std::vector<geometry::Point2D>& pts) {
    double cx = 0, cy = 0;

    for (const auto& p : pts) {
        auto pn = geometry::normalize(p);
        cx += pn.x;
        cy += pn.y;
    }

    cx /= static_cast<double>(pts.size());
    cy /= static_cast<double>(pts.size());

    double meanDist = 0;

    for (auto& p : pts) {
        double x = p.x / p.z - cx;
        double y = p.y / p.z - cy;
        meanDist += std::sqrt(x * x + y * y);
    }

    meanDist /= static_cast<double>(pts.size());

    double s = std::sqrt(2.0) / meanDist;

    auto S = transform::Transform2D::scale(s);
    auto Tr = transform::Transform2D::translation(-cx, -cy);

    core::Mat3 T = S.H * Tr.H;

    std::vector<geometry::Point2D> norm;
    norm.reserve(pts.size());

    for (auto& p : pts) {
        norm.push_back(T * p);
    }

    return {norm, T};
}

core::Mat3 estimateHomography(
    const std::vector<std::pair<geometry::Point2D, geometry::Point2D>>& correspondences) {
    const int N = static_cast<int>(correspondences.size());
    assert(N >= 4 && "Need at least 4 point correspondences");

    std::vector<geometry::Point2D> pts1, pts2;
    pts1.reserve(N);
    pts2.reserve(N);

    for (auto& [p, p2] : correspondences) {
        pts1.push_back(p);
        pts2.push_back(p2);
    }

    auto [n1, T1] = normalizePoints(pts1);
    auto [n2, T2] = normalizePoints(pts2);

    Eigen::MatrixXd A(2 * N, 9);

    for (int i = 0; i < N; ++i) {
        const auto& p = n1[i];
        const auto& p_prime = n2[i];

        const double x = p.x;
        const double y = p.y;
        const double w = p.z;

        const double xp = p_prime.x;
        const double yp = p_prime.y;

        A(2 * i, 0) = 0;
        A(2 * i, 1) = 0;
        A(2 * i, 2) = 0;
        A(2 * i, 3) = -x;
        A(2 * i, 4) = -y;
        A(2 * i, 5) = -w;
        A(2 * i, 6) = yp * x;
        A(2 * i, 7) = yp * y;
        A(2 * i, 8) = yp * w;

        A(2 * i + 1, 0) = x;
        A(2 * i + 1, 1) = y;
        A(2 * i + 1, 2) = w;
        A(2 * i + 1, 3) = 0;
        A(2 * i + 1, 4) = 0;
        A(2 * i + 1, 5) = 0;
        A(2 * i + 1, 6) = -xp * x;
        A(2 * i + 1, 7) = -xp * y;
        A(2 * i + 1, 8) = -xp * w;
    }

    Eigen::JacobiSVD<Eigen::MatrixXd> svd(A, Eigen::ComputeFullV);
    Eigen::VectorXd h = svd.matrixV().col(8);

    core::Mat3 H_norm(h(0), h(1), h(2), h(3), h(4), h(5), h(6), h(7), h(8));

    core::Mat3 H = T2.inverse() * H_norm * T1;

    if (std::abs(H.m[2][2]) > core::kEps) {
        double s = H.m[2][2];
        H = core::Mat3(H.m[0][0] / s, H.m[0][1] / s, H.m[0][2] / s, H.m[1][0] / s, H.m[1][1] / s,
                       H.m[1][2] / s, H.m[2][0] / s, H.m[2][1] / s, H.m[2][2] / s);
    }

    return H;
}

}  // namespace projective