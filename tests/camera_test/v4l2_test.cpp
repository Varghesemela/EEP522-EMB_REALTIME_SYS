#include <opencv2/opencv.hpp>
#include <iostream>
#include <unistd.h>
using namespace cv;
using namespace std;

int main() {
    VideoCapture cap(0, cv::CAP_V4L2);
    if (!cap.isOpened()){
        cerr << "Error: Could not open camera." << endl;
        return -1;
    }
    Mat frame;
    // Give the camera a little time to warm up
    sleep(1);
    cap >> frame;
    if (frame.empty()){
        cerr << "Error: Captured frame is empty." << endl;
        return -1;
    }
    imshow("Test Frame", frame);
    waitKey(0);
    return 0;
}
