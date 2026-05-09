#include <Eigen/Dense>
#include <cassert>
#include <iostream>
#include <projective/homography_estimation.hpp>
#include <random>
#include <unordered_set>

namespace projective {

std::pair<std::vector<geometry::Point2D>, core::Mat3> normalizePoints(
    const std::vector<geometry::Point2D>& pts) noexcept {
    if (pts.empty()) {
        return {{}, core::Mat3::identity()};
    }

    double cx = 0.0, cy = 0.0;
    std::vector<geometry::Point2D> normalized;
    normalized.reserve(pts.size());

    for (const auto& p : pts) {
        auto pn = geometry::normalize(p);
        cx += pn.x;
        cy += pn.y;
        normalized.push_back(pn);
    }

    cx /= static_cast<double>(pts.size());
    cy /= static_cast<double>(pts.size());

    double meanDist = 0.0;

    for (const auto& pn : normalized) {
        double dx = pn.x - cx;
        double dy = pn.y - cy;
        meanDist += std::sqrt(dx * dx + dy * dy);
    }

    meanDist /= static_cast<double>(normalized.size());

    if (meanDist < core::kEps) {
        return {normalized, core::Mat3::identity()};
    }

    double s = std::sqrt(2.0) / meanDist;

    auto S = transform::Transform2D::scale(s);
    auto Tr = transform::Transform2D::translation(-cx, -cy);

    core::Mat3 T = S.H * Tr.H;

    std::vector<geometry::Point2D> norm(normalized.size());

    std::transform(normalized.begin(), normalized.end(), norm.begin(),
                   [&T](const auto& pn) { return T * pn; });

    return {norm, T};
}

core::Mat3 estimateHomography(
    const std::vector<std::pair<geometry::Point2D, geometry::Point2D>>& correspondences) noexcept {
    assert(correspondences.size() >= 4 && "Need at least 4 point correspondences");

    auto [src, T1] = normalizePoints([&]() {
        std::vector<geometry::Point2D> v;
        for (const auto& c : correspondences) v.push_back(c.first);
        return v;
    }());

    auto [dst, T2] = normalizePoints([&]() {
        std::vector<geometry::Point2D> v;
        for (const auto& c : correspondences) v.push_back(c.second);
        return v;
    }());

    Eigen::MatrixXd A(2 * correspondences.size(), 9);

    for (size_t i = 0; i < correspondences.size(); ++i) {
        const double x = src[i].x;
        const double y = src[i].y;
        const double xp = dst[i].x;
        const double yp = dst[i].y;

        A.row(2 * i) << -x, -y, -1, 0, 0, 0, x * xp, y * xp, xp;
        A.row(2 * i + 1) << 0, 0, 0, -x, -y, -1, x * yp, y * yp, yp;
    }

    Eigen::JacobiSVD<Eigen::MatrixXd> svd(A, Eigen::ComputeFullV);
    if (svd.rank() < 8) {
        return core::Mat3::identity();
    }
    Eigen::VectorXd h = svd.matrixV().col(8);

    core::Mat3 H(h(0), h(1), h(2), h(3), h(4), h(5), h(6), h(7), h(8));

    H = T2.inverse() * H * T1;

    if (std::abs(H.m[2][2]) > core::kEps) {
        double s = H.m[2][2];
        H = core::Mat3(H.m[0][0] / s, H.m[0][1] / s, H.m[0][2] / s, H.m[1][0] / s, H.m[1][1] / s,
                       H.m[1][2] / s, H.m[2][0] / s, H.m[2][1] / s, H.m[2][2] / s);
    }

    return H;
}

core::Mat3 estimateHomographyRansac(
    const std::vector<std::pair<geometry::Point2D, geometry::Point2D>>& correspondences,
    double threshold, int iterations) noexcept {
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(0, correspondences.size() - 1);

    core::Mat3 bestH;
    size_t bestInliers = 0;

    for (int it = 0; it < iterations; ++it) {
        std::unordered_set<int> indices;
        while (indices.size() < 4) {
            indices.insert(dist(rng));
        }

        std::vector<std::pair<geometry::Point2D, geometry::Point2D>> sample;
        for (int idx : indices) {
            sample.push_back(correspondences[idx]);
        }

        auto H = estimateHomography(sample);

        size_t count = 0;
        std::vector<std::pair<geometry::Point2D, geometry::Point2D>> inliers;

        for (const auto& c : correspondences) {
            double err = reprojectionError(H, c.first, c.second);

            if (err < threshold) {
                count++;
                inliers.push_back(c);
            }
        }

        std::cout << "Iteration: " << it << ", inliers: " << count << " / "
                  << correspondences.size() << '\n';

        if (count > bestInliers) {
            bestInliers = count;
            bestH = estimateHomography(inliers);
        }
    }

    std::cout << "Best inliers: " << bestInliers << " / " << correspondences.size() << '\n';

    return bestH;
}

static double reprojectionError(const core::Mat3& H, const geometry::Point2D& p,
                                const geometry::Point2D& q) noexcept {
    auto Hp = H * p;

    if ((std::abs(Hp.z) < core::kEps) || (std::abs(q.z) < core::kEps)) {
        return core::kInf;
    }

    auto Hn = geometry::normalize(Hp);
    auto qn = geometry::normalize(q);

    double dx = Hn.x - qn.x;
    double dy = Hn.y - qn.y;

    return std::sqrt(dx * dx + dy * dy);
}

}  // namespace projective