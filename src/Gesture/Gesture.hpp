#ifndef GESTURE_HPP_
#define GESTURE_HPP_

#include <list>
#include <vector>
#include <opencv2/core/core.hpp>

class Gesture {
public:
    Gesture() : status_(-1) {}

    void addPoint(const cv::Point2f& point);
    int endGesture();
    int status() const { return status_; }
private:
    const static size_t buffer_size_ = 5;

    int analyzePoints(const std::vector<cv::Point2f>& points);

    std::list<cv::Point2f> buffer_;
    std::vector<cv::Point2f> points_;
    int status_;
};

#endif
