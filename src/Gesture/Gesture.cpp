#include "Gesture.hpp"

#include <iostream>

void Gesture::addPoint(const cv::Point2f& point)
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

int Gesture::endGesture()
{
    status_ = analyzePoints(points_);

    buffer_.clear();
    points_.clear();

    int answer = status_;
    status_ = -1;
    return answer;
}

int Gesture::analyzePoints(const std::vector<cv::Point2f>& points)
{
    return 9;
}
