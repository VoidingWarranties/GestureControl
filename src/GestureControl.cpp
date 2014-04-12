#include <iostream>

#include <opencv2/highgui/highgui.hpp>

int main(int argc, char** argv)
{
    cv::namedWindow("Output");
    cv::VideoCapture cam(1);
    while (true) {
        cv::Mat image;
        cam >> image;

        cv::imshow("Output", image);

        int key = cv::waitKey(10);
        if (key == 'q') {
            break;
        }
    }

    return 0;
}
