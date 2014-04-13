#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "Gesture.hpp"

/* TO DO:
 * - background subtraction so that only moving objects are considered for tracking
 */

#define MIN_CIRCULARITY 0.6
#define MIN_AREA 500

int Y_val_min = 100, Y_val_max = 220;
int Cb_val_min = 140, Cb_val_max = 175;
int Cr_val_min = 70, Cr_val_max = 110;

cv::Mat global_image;

void onMouse(int event, int x, int y, int, void*)
{
    if (event != CV_EVENT_LBUTTONDOWN) {
        return;
    }

    cv::Vec3b values = global_image.at<cv::Vec3b>(y,x);
    float h_val = values[0];
    float s_val = values[1];
    float v_val = values[2];

    std::cout << "H: " << h_val << ", S: " << s_val << ", V: " << v_val << std::endl;
}

cv::Mat segmentByColor(const cv::Mat& image)
{
    cv::Mat hsv_image;
    cv::cvtColor(image, hsv_image, CV_BGR2YCrCb);

        cv::Mat just_h(hsv_image.size(), CV_8UC1);
        int from_to[] = {0,0};
        cv::mixChannels(&hsv_image, 1, &just_h, 1, from_to, 1);
        global_image = hsv_image;

    cv::Mat segmented_image(hsv_image.size(), CV_8UC1);
    cv::inRange(hsv_image, cv::Scalar(Y_val_min, Cb_val_min, Cr_val_min), cv::Scalar(Y_val_max, Cb_val_max, Cr_val_max), segmented_image);

    return segmented_image;
}

void removeNoise(cv::Mat* image)
{
    cv::Mat kernel(3, 3, CV_8UC1, cv::Scalar(1));
    cv::erode(*image, *image, kernel, cv::Point(-1,-1), 2);
    cv::dilate(*image, *image, kernel, cv::Point(-1,-1), 1);
}

cv::Mat preProcess(const cv::Mat& image)
{
        cv::Mat pp_image = segmentByColor(image);
        removeNoise(&pp_image);

        cv::GaussianBlur(pp_image, pp_image, cv::Size(9, 9), 2);

        return pp_image;
}

bool contour_comp(const std::pair<std::vector<cv::Point>, float>& a, const std::pair<std::vector<cv::Point>, float>& b)
{
    return (a.second > b.second);
}

std::vector<std::vector<cv::Point> > getNMostCircularObjects(const cv::Mat& image, size_t n = 1)
{
    std::vector<std::vector<cv::Point> > contours;
    std::vector<std::pair<std::vector<cv::Point>, float> > contour_circularities;
    cv::findContours(image, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
    for (size_t i = 0; i < contours.size(); ++i) {
        double perimeter = cv::arcLength(contours[i], true);
        double area = cv::contourArea(contours[i]);
        double circularity = 4 * 3.14159 * area / (perimeter * perimeter);
        contour_circularities.push_back(std::pair<std::vector<cv::Point>, float>(contours[i], circularity));
    }

    //std::sort(contour_circularities.begin(), contour_circularities.end(), [](const std::pair<std::vector<cv::Point>, float>& a, const std::pair<std::vector<cv::Point>, float>& b) { return (a.second > b.second); });
    std::sort(contour_circularities.begin(), contour_circularities.end(), contour_comp);

    std::vector<std::vector<cv::Point> > most_circular;
    for (size_t i = 0; i < contour_circularities.size(); ++i) {
        if (contour_circularities[i].second > MIN_CIRCULARITY && cv::contourArea(contour_circularities[i].first) > MIN_AREA) {
            most_circular.push_back(contour_circularities[i].first);
            if (most_circular.size() >= n) {
                break;
            }
        }
    }

    return most_circular;
}

std::vector<std::pair<cv::Point2f, float> > boundingCircles(const std::vector<std::vector<cv::Point> >& contours)
{
    std::vector<std::pair<cv::Point2f, float> > circles;
    for (size_t i = 0; i < contours.size(); ++i) {
        cv::Point2f tmp_center;
        float tmp_radius;
        cv::minEnclosingCircle(contours[i], tmp_center, tmp_radius);
        circles.push_back(std::pair<cv::Point2f, float>(tmp_center, tmp_radius));
    }
    return circles;
}

int main(int argc, char** argv)
{
    cv::namedWindow("cam");
    cv::createTrackbar("Y min value", "cam", &Y_val_min, 255);
    cv::createTrackbar("Y max value", "cam", &Y_val_max, 255);
    cv::createTrackbar("Cb min value", "cam", &Cb_val_min, 255);
    cv::createTrackbar("Cb max value", "cam", &Cb_val_max, 255);
    cv::createTrackbar("Cr min value", "cam", &Cr_val_min, 255);
    cv::createTrackbar("Cr max value", "cam", &Cr_val_max, 255);

    cv::VideoCapture cam(1);

    cv::setMouseCallback("cam", onMouse, 0);

    Gesture current_gesture;

    size_t no_detects = 0;

    while (true) {
        cv::Mat cam_image;
        cam >> cam_image;

        cv::Mat pp_image = preProcess(cam_image);
        std::vector<std::vector<cv::Point> > most_circular = getNMostCircularObjects(pp_image, 2);
        std::vector<std::pair<cv::Point2f, float> > circles = boundingCircles(most_circular);

        std::sort(circles.begin(), circles.end(), [](const std::pair<cv::Point2f, float>& a, const std::pair<cv::Point2f, float>& b)
            {
                return (a.first.x < b.first.x);
            });

        for (size_t i = 0; i < circles.size(); ++i) {
            cv::Scalar color;
            switch (i) {
                case 0:
                    color = cv::Scalar(255,0,0);
                    break;
                case 1:
                    color = cv::Scalar(0,255,0);
                    break;
                case 2:
                    color = cv::Scalar(0,0,255);
                    break;
            }
            cv::circle(cam_image, circles[i].first, circles[i].second, color, 2);
        }

/*
        if (circles.size() > 0) {
            // this is hardcoded for 1 circle. need to change in the future for multiple circles
            current_gesture.addPoint(circles[0].first); // could use the radius as the third dimension
            no_detects = 0;
        } else if (current_gesture.status() == Gesture::IN_PROGRESS) {
            ++no_detects;
            if (no_detects > 15) {
                int gest = current_gesture.endGesture();
                std::cout << "Gesture detected! " << gest << std::endl;
                no_detects = 0;
            }
        }
        */

        cv::imshow("cam", cam_image);

        int key = cv::waitKey(1);
        if (key == 'q') {
            break;
        }
    }

    return 0;
}
