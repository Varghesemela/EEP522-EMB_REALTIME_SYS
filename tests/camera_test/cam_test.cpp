#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>
#include <thread>

using namespace cv;
using namespace std;

int main() {
    // 1. Create the capture pipeline using libcamera.
    // This pipeline captures 640x480 video at 30 fps and delivers frames to appsink.
    string capturePipeline = "libcamerasrc ! video/x-raw,width=640,height=480,framerate=30/1 ! videoconvert ! appsink";
    
    // Open the camera using the GStreamer pipeline.
    VideoCapture cap(capturePipeline, cv::CAP_GSTREAMER);
    if (!cap.isOpened()){
        cerr << "Error: Could not open camera using libcamera pipeline." << endl;
        return -1;
    }
    cout << "Camera opened successfully with libcamera pipeline." << endl;
    
    // 2. Create the streaming pipeline.
    // This pipeline takes frames from appsrc, converts them, encodes with x264,
    // packetizes with RTP (H264 payloader), and sends them over UDP.
    // Adjust the destination IP as needed.
    string destIP = "192.168.0.3";  // Replace with your destination IP or hostname.
    string writerPipeline = "appsrc ! videoconvert ! x264enc tune=zerolatency bitrate=500 speed-preset=superfast ! "
                            "rtph264pay config-interval=1 pt=96 ! udpsink host=" + destIP + " port=5000";
    
    // Open the VideoWriter with the GStreamer pipeline.
    // Here, we set the FPS to 30 and frame size to 640x480.
    VideoWriter writer(writerPipeline, cv::CAP_GSTREAMER, 0, 30, Size(640,480), true);
    if (!writer.isOpened()){
        cerr << "Error: Could not open VideoWriter with pipeline:" << endl << writerPipeline << endl;
        return -1;
    }
    cout << "Streaming pipeline opened. Sending stream to " << destIP << " on port 5000." << endl;
    
    // 3. Main capture and streaming loop.
    Mat frame;
    int frameCount = 0;
    while (true) {
        // Capture a frame from the camera.
        cap >> frame;
        if (frame.empty()){
            cerr << "Error: Captured frame is empty." << endl;
            break;
        }
        
        // Write the frame to the streaming pipeline.
        writer.write(frame);
        frameCount++;
        
        // Optionally, print a debug message every 50 frames.
        if (frameCount % 50 == 0) {
            cout << "Processed " << frameCount << " frames." << endl;
        }
        
        // Sleep a short while to roughly maintain 30 fps.
        // (This is approximate—adjust as needed.)
        std::this_thread::sleep_for(std::chrono::milliseconds(33));
        
        int key = waitKey(1);
        if (key == 27 || key == 'q') {  // break if ESC or 'q' is pressed
            break;
        }
    }
    
    // Clean up resources.
    cap.release();
    writer.release();
    
    return 0;
}
