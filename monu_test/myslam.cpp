
// 该文件将打开你电脑的摄像头，并将图像传递给ORB-SLAM2进行定位
 
// opencv
#include <opencv2/opencv.hpp>
 
// ORB-SLAM的系统接口
#include "./include/System.h"
#include <string>
#include <chrono>   // for time stamp
#include <iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;
 
// 参数文件与字典文件
// 如果你系统上的路径不同，请修改它
string parameterFile = "./monu_test/myslam.yaml";
string vocFile = "./Vocabulary/ORBvoc.txt";
 
int main(int argc, char **argv) {


    // 声明 ORB-SLAM2 系统
    ORB_SLAM3::System SLAM(vocFile, parameterFile, ORB_SLAM3::System::MONOCULAR, true);
    // 获取相机图像代码
    cv::VideoCapture cap(0);    // change to 1 if you want to use USB camera.
    // 分辨率设为640x480
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);;//设置采集视频的宽度
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);//设置采集视频的高度
    // 记录系统时间
    auto start = chrono::system_clock::now();
 
    while (1) {
        //cout<<"读取图片"<<endl;
        cv::Mat frame;
        cap >> frame;   // 读取相机数据
        if (!cap.isOpened()) {
            std::cerr << "Error: Failed to open camera!" << std::endl;
            return -1;
        }
        if (!cap.read(frame)) {
            std::cerr << "Error: Failed to read frame from camera!" << std::endl;
            return -1;
        }
        std::cout << "Frame width: " << frame.cols << ", height: " << frame.rows << std::endl;
        // 检查图像数据是否有效
        if (frame.empty()) {
            std::cerr << "Error: Failed to capture frame!" << std::endl;
            break;
        }
        //cout<<"读取完成"<<endl;
        auto now = chrono::system_clock::now();
        auto timestamp = chrono::duration_cast<chrono::milliseconds>(now - start);
        
        SLAM.TrackMonocular(frame, double(timestamp.count())/1000.0);
    }
 
    return 0;
}

