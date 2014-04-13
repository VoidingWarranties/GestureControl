#ifndef GESTURE_HPP_
#define GESTURE_HPP_

#include <list>
#include <vector>
#include <opencv2/core/core.hpp>

class Gesture_1hand {
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

    Gesture_1hand() : status_(NONE) {}

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

class Gesture_2hand {
public:
    const static int NONE = -1;
    const static int IN_PROGRESS = 0;

    Gesture_2hand() : status_(NONE), point_pair_(std::pair<cv::Point, cv::Point>(cv::Point(0,0), cv::Point(0,0))) {}

    int xPos() const { return ((point_pair_.first.x / 2) + (point_pair_.second.x / 2)); }
    int yPos() const { return ((point_pair_.first.y / 2) + (point_pair_.second.y / 2)); }
    int status() const { return status_; }

    void updatePos(const std::pair<cv::Point2f, cv::Point2f>& points) { point_pair_ = points; status_ = IN_PROGRESS; }
    void endGesture() { point_pair_ = std::pair<cv::Point, cv::Point>(cv::Point(0,0), cv::Point(0,0)); status_ = NONE; }
private:
    std::pair<cv::Point, cv::Point> point_pair_;
    int status_;
};

#endif
