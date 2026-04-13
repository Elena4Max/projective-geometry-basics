#include <algorithm>
#include <geometry/types.hpp>
#include <opencv2/opencv.hpp>
#include <projective/homography_estimation.hpp>
#include <random>

using geometry::Point2D;

std::vector<cv::KeyPoint> detectKeypoints(const cv::Mat& img, cv::Mat& desc) {
    auto orb = cv::ORB::create();
    std::vector<cv::KeyPoint> kp;
    orb->detectAndCompute(img, cv::noArray(), kp, desc);
    return kp;
}

std::vector<cv::DMatch> matchDescriptors(const cv::Mat& d1, const cv::Mat& d2) {
    cv::BFMatcher matcher(cv::NORM_HAMMING);

    std::vector<std::vector<cv::DMatch>> knn;
    matcher.knnMatch(d1, d2, knn, 2);

    std::vector<cv::DMatch> good;

    for (auto& m : knn) {
        if (m.size() < 2) {
            continue;
        }
        if (m[0].distance < 0.75 * m[1].distance) {
            good.push_back(m[0]);
        }
    }

    std::sort(good.begin(), good.end(),
              [](const auto& a, const auto& b) { return a.distance < b.distance; });

    if (good.size() > 50) {
        good.resize(50);
    }

    return good;
}

std::vector<std::pair<Point2D, Point2D>> buildCorrespondences(
    const std::vector<cv::KeyPoint>& kp1, const std::vector<cv::KeyPoint>& kp2,
    const std::vector<cv::DMatch>& matches) {
    std::vector<std::pair<Point2D, Point2D>> corr;
    corr.reserve(matches.size());

    for (auto& m : matches) {
        auto p1 = kp1[m.queryIdx].pt;
        auto p2 = kp2[m.trainIdx].pt;

        corr.emplace_back(Point2D{p2.x, p2.y, 1.0}, Point2D{p1.x, p1.y, 1.0});
    }

    return corr;
}

double reprojectionError(const core::Mat3& H, const Point2D& p, const Point2D& p2) {
    auto pe = H * p;

    if (std::abs(pe.z) < core::kEps) {
        return 1e9;
    }

    pe.x /= pe.z;
    pe.y /= pe.z;

    return std::sqrt((pe.x - p2.x) * (pe.x - p2.x) + (pe.y - p2.y) * (pe.y - p2.y));
}

core::Mat3 estimateRANSAC(const std::vector<std::pair<Point2D, Point2D>>& corr) {
    const int iterations = 500;
    const double threshold = 2.0;

    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(0, static_cast<int>(corr.size()) - 1);

    core::Mat3 bestH;
    int bestInliers = 0;

    for (int it = 0; it < iterations; ++it) {
        std::vector<std::pair<Point2D, Point2D>> sample;

        while (sample.size() < 4) {
            sample.push_back(corr[dist(rng)]);
        }

        auto H = projective::estimateHomography(sample);

        int inliers = 0;
        for (auto& [p, p2] : corr) {
            if (reprojectionError(H, p, p2) < threshold) {
                inliers++;
            }
        }

        if (inliers > bestInliers) {
            bestInliers = inliers;
            bestH = H;
        }
    }

    std::cout << "My inliers: " << bestInliers << "\n";

    std::vector<std::pair<Point2D, Point2D>> bestSet;

    for (auto& [p, p2] : corr) {
        if (reprojectionError(bestH, p, p2) < threshold) {
            bestSet.emplace_back(p, p2);
        }
    }

    return projective::estimateHomography(bestSet);
}

cv::Mat estimateOpenCV(const std::vector<cv::KeyPoint>& kp1, const std::vector<cv::KeyPoint>& kp2,
                       const std::vector<cv::DMatch>& matches) {
    std::vector<cv::Point2f> pts1, pts2;

    for (auto& m : matches) {
        pts1.push_back(kp1[m.queryIdx].pt);
        pts2.push_back(kp2[m.trainIdx].pt);
    }

    cv::Mat mask;

    cv::Mat H = cv::findHomography(pts2, pts1, cv::RANSAC, 2.0, mask, 2000, 0.995);

    int inliers = 0;
    for (int i = 0; i < mask.rows; ++i) {
        if (mask.at<uchar>(i)) {
            inliers++;
        }
    }

    std::cout << "OpenCV inliers: " << inliers << " / " << pts1.size() << "\n";

    return H;
}

cv::Mat warp(const cv::Mat& img, const core::Mat3& H, cv::Size size) {
    cv::Mat Hcv = (cv::Mat_<double>(3, 3) << H.m[0][0], H.m[0][1], H.m[0][2], H.m[1][0], H.m[1][1],
                   H.m[1][2], H.m[2][0], H.m[2][1], H.m[2][2]);

    cv::Mat out;
    cv::warpPerspective(img, out, Hcv, size);
    return out;
}

cv::Mat warpCV(const cv::Mat& img, const cv::Mat& H, cv::Size size) {
    cv::Mat out;
    cv::warpPerspective(img, out, H, size);
    return out;
}

cv::Mat blend(const cv::Mat& left, const cv::Mat& right) {
    cv::Mat result = right.clone();

    for (int y = 0; y < result.rows; ++y) {
        for (int x = 0; x < result.cols; ++x) {
            if (x < left.cols && y < left.rows) {
                auto l = left.at<cv::Vec3b>(y, x);
                auto& r = result.at<cv::Vec3b>(y, x);

                if (r != cv::Vec3b(0, 0, 0)) {
                    double alpha = double(x) / left.cols;
                    r = (1 - alpha) * l + alpha * r;
                } else {
                    r = l;
                }
            }
        }
    }

    return result;
}

std::pair<cv::Mat, cv::Mat> buildPanoramaCompare(const cv::Mat& img1, const cv::Mat& img2) {
    cv::Mat d1, d2;

    auto kp1 = detectKeypoints(img1, d1);
    auto kp2 = detectKeypoints(img2, d2);

    auto matches = matchDescriptors(d1, d2);

    auto corr = buildCorrespondences(kp1, kp2, matches);

    auto H_my = estimateRANSAC(corr);
    auto H_cv = estimateOpenCV(kp1, kp2, matches);

    cv::Size canvas(img1.cols * 2, img1.rows);

    auto warped_my = warp(img2, H_my, canvas);
    auto warped_cv = warpCV(img2, H_cv, canvas);

    auto pano_my = blend(img1, warped_my);
    auto pano_cv = blend(img1, warped_cv);

    return {pano_my, pano_cv};
}

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: ./visualize img1 img2\n";
        return 1;
    }

    cv::Mat img1 = cv::imread(argv[1]);
    cv::Mat img2 = cv::imread(argv[2]);

    if (img1.empty() || img2.empty()) {
        std::cerr << "Failed to load images\n";
        return 1;
    }

    auto [pano_my, pano_cv] = buildPanoramaCompare(img1, img2);

    cv::imwrite("pano_my.jpg", pano_my);
    cv::imwrite("pano_cv.jpg", pano_cv);

    std::cout << "Saved pano_my.jpg and pano_cv.jpg\n";

    return 0;
}