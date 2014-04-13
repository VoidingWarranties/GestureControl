#ifndef GESTURE_HPP_
#define GESTURE_HPP_

#include <list>
#include <vector>
#include <opencv2/core/core.hpp>

class Gesture {
public:
    const static int NONE = -1;
    const static int IN_PROGRESS = 0;
    const static int UNKNOWN = 1;
    const static int SWIPE_LEFT = 2;
    const static int SWIPE_RIGHT = 3;
    const static int SWIPE_UP = 4;
    const static int SWIPE_DOWN = 5;
    const static int LOOP_CW = 6;
    const static int LOOP_CCW = 7;

    Gesture() : status_(NONE) {}

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
