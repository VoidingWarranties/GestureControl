#include "Gesture.hpp"

#include <opencv2/imgproc/imgproc.hpp>

#include <opencv2/highgui/highgui.hpp>

#include <iostream>

void Gesture_1hand::addPoint(const cv::Point2f& point)
{
    status_ = 0;

    std::list<cv::Point2f>::iterator itr;
    for (itr = buffer_.begin(); itr != buffer_.end(); ++itr) {
        if (point.x < itr->x || point.y < itr->y) {
            buffer_.insert(itr, point);
            break;
        }
    }
    if (itr == buffer_.end() || buffer_.size() == 0) {
        buffer_.push_back(point);
    }
    if (buffer_.size() >= buffer_size_) {
        itr = buffer_.begin();
        for (size_t i = 0; i < buffer_size_ / 2; ++i, ++itr) {}
        points_.push_back(*itr);
        buffer_.clear();
    }
}

int Gesture_1hand::endGesture()
{
    status_ = analyzePoints(points_);

    buffer_.clear();
    points_.clear();

    int answer = status_;
    status_ = -1;
    return answer;
}

int Gesture_1hand::analyzePoints(const std::vector<cv::Point2f>& points)
{
    if (points.empty()) {
        return UNKNOWN;
    }

    cv::Rect bounding_rect = boundingRect(points);

    if (bounding_rect.width > bounding_rect.height) {
        if (points.front().x > points.back().x) {
            return SWIPE_RIGHT;
        } else {
            return SWIPE_LEFT;
        }
    } else {
        if (points.front().y > points.back().y) {
            return SWIPE_UP;
        } else {
            return SWIPE_DOWN;
        }
    }

    return UNKNOWN;
}
