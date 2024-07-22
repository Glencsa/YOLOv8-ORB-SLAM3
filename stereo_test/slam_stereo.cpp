#include<opencv2/opencv.hpp>
#include"System.h"
#include<string>
#include<chrono>
#include<iostream>
using namespace std;

string parameterFile = "./stereo_test/slam_stereo.yaml";
string vocFile = "./Vocabulary/ORBvoc.txt";

int main(int argc, char **argv) {
    // 创建双目 SLAM 系统
    ORB_SLAM3::System SLAM(vocFile, parameterFile, ORB_SLAM3::System::STEREO, true);

    // 打开双目摄像头
    cv::VideoCapture cap(4);


    // 设置摄像头参数
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 3840);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 1080);

    // 记录开始时间
    auto start = chrono::system_clock::now();

    while (1) {
        cv::Mat frame_left, frame_right,frame;
        cv::Rect left_R(0,0,1920,1080);
        cv::Rect right_R(1920,0,1920,1080);

        // 从左摄像头获取图像
        cap >> frame;
        frame(left_R).copyTo(frame_left);
        frame(right_R).copyTo(frame_right);
        // 获取当前时间戳
        auto now = chrono::system_clock::now();
        auto timestamp = chrono::duration_cast<chrono::milliseconds>(now - start);

        // 调用 TrackStereo 函数
        SLAM.TrackStereo(frame_left, frame_right, double(timestamp.count()) / 1000.0);
    }

    return 0;
}

