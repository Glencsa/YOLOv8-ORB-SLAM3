#include <iostream>
#include<opencv2/opencv.hpp>
#include<math.h>
#include "yolov8_seg.h"
#include<time.h>
#include<Eigen/Dense>
//#define  VIDEO_OPENCV //if define, use opencv for video.

using namespace std;
using namespace cv;
using namespace dnn;

template<typename _Tp>
int yolov8(_Tp& task, cv::Mat& img, std::string& model_path,std::vector<OutputParams>& result)
{


	cv::dnn::Net net;
	if (task.ReadModel(net, model_path, false)) {
		std::cout << "read net ok!" << std::endl;
	}
	else {
		return -1;
	}
	//生成随机颜色
	std::vector<cv::Scalar> color;
	srand(time(0));
	for (int i = 0; i < 80; i++) {
		int b = rand() % 256;
		int g = rand() % 256;
		int r = rand() % 256;
		color.push_back(cv::Scalar(b, g, r));
	}

	// bool isPose = false;
	// if (typeid(task) == typeid(Yolov8Pose)) {
	// 	isPose = true;
	// }
	// PoseParams poseParams;
	if (task.Detect(img, net, result)) {

		// if (isPose)
		// 	DrawPredPose(img, result, poseParams);
		// else
		DrawPred(img, result, task._className, color);
	}
	else {
		std::cout << "Detect Failed!" << std::endl;
	}

	//system("pause");
	return 0;
}



int main() {

	std::string img_path = "../bg3.jpg";

	//std::string model_path_detect = "./models/yolov8s-pose1.onnx";
	//std::string model_path_rtdetr = "./models/rtdetr-l.onnx";  //yolov8-redetr
	//std::string model_path_obb = "./models/yolov8s-obb.onnx";
	std::string model_path_seg = "/home/glencs/code_file/yolov8s-seg.onnx";
	//std::string model_path_pose = "./models/yolov8s-pose.onnx";

	cv::Mat src = imread(img_path);
	cv::Mat img = src.clone();


	Yolov8Seg			task_segment_ocv;


	img = src.clone();
	vector<OutputParams> result;
	vector<Mat> mask_result;
	yolov8(task_segment_ocv,img,model_path_seg,result);   //yolov8 opencv segment
	
	for(int i = 0;i<result.size();++i)
	{
		if(result[i].id ==0)
		{
			mask_result.push_back(result[i].boxMask);
		}
	}
	// for(auto a:mask_graphic)
	// {
	// 	cout<<"mask matrix"<<endl;
		
	// 	//cout<<a<<endl;
	// }

#ifdef VIDEO_OPENCV
	video_demo(task_detect_ocv, model_path_detect);
	//video_demo(task_segment_ocv, model_path_seg);
	//video_demo(task_pose_ocv, model_path_pose);
#else
	//video_demo(task_detect_ort, model_path_detect);
	//video_demo(task_rtdetr_ort, model_path_rtdetr);
	//video_demo(task_segment_ort, model_path_seg);
	//video_demo(task_pose_ort, model_path_pose);
#endif
	return 0;
}


