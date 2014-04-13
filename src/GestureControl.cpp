#include <iostream>
#include <vector>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

//const cv::Scalar gatorade_MIN(5, 150, 0);
//const cv::Scalar gatorade_MAX(15, 245, 255);
const cv::Scalar gatorade_MIN(60, 140, 60);
const cv::Scalar gatorade_MAX(230, 190, 100);
const cv::Scalar blue_sticky_MIN(102, 130, 0);
const cv::Scalar blue_sticky_MAX(108, 230, 255);

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
        //cv::imshow("hsv", just_h);

    cv::Mat segmented_image(hsv_image.size(), CV_8UC1);
    cv::inRange(hsv_image, gatorade_MIN, gatorade_MAX, segmented_image);

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

int main(int argc, char** argv)
{
    cv::namedWindow("cam");
    cv::VideoCapture cam(1);

    //cv::namedWindow("hsv");
    cv::setMouseCallback("cam", onMouse, 0);

    while (true) {
        cv::Mat cam_image;
        cam >> cam_image;

        cv::Mat pp_image = preProcess(cam_image);
        cv::namedWindow("foobar");
        cv::imshow("foobar", pp_image);

        std::vector<std::vector<cv::Point> > contours;
        cv::findContours(pp_image, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
        double max_circularity = 0;
        size_t max_circularity_i = 0;
        for (size_t i = 0; i < contours.size(); ++i) {
            double perimeter = cv::arcLength(contours[i], true);
            double area = cv::contourArea(contours[i]);
            double circularity = 4 * 3.14159 * area / (perimeter * perimeter);
            if (circularity > max_circularity && area > 500) {
                max_circularity = circularity;
                max_circularity_i = i;
            }
            cv::drawContours(cam_image, contours, max_circularity_i, cv::Scalar(255,0,0), 2);
        }

        if (max_circularity > 0.77) {
            cv::drawContours(cam_image, contours, max_circularity_i, cv::Scalar(0,255,0), 2);
            cv::drawContours(cam_image, contours, max_circularity_i, cv::Scalar(0,0,255), -1);
        }


/*
        cv::Mat detect_this(cam_image.size(), CV_8UC1, cv::Scalar(0));
        cv::drawContours(detect_this, contours, largest_contour_i, cv::Scalar(255), 2);
        cv::GaussianBlur(detect_this, detect_this, cv::Size(9, 9), 2);
        cv::namedWindow("blur");
        cv::imshow("blur", detect_this);

        std::vector<cv::Vec3f> circles;
        cv::HoughCircles(detect_this, circles, CV_HOUGH_GRADIENT, 1, 1);
        std::cout << circles.size() << std::endl;
        for (size_t i = 0; i < circles.size(); ++i) {
            cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            int radius = cvRound(circles[i][2]);
            circle(cam_image, center, 3, cv::Scalar(0,255,0));
            circle(cam_image, center, radius, cv::Scalar(0,0,255));
        }
        */


        cv::imshow("cam", cam_image);
        //cv::imshow("skin", skin_image);

        int key = cv::waitKey(10);
        if (key == 'q') {
            break;
        }
    }

    return 0;
}
