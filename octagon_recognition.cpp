// octagon_recognition.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
using namespace cv;
using namespace std;

#define MIN_AREA 100
#define WHRATIO 0.1

int main()
{
	const char* inputImage = "recognition.jpg";
	Mat src, dst, canny_output;
	/// Load source image and convert it to gray  
	src = imread(inputImage, 0);

	if (!src.data)
	{
		std::cout << "read data error!" << std::endl;
		return -1;
	}
	blur(src, src, Size(3, 3));


	//the pram. for findContours,  
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	/// Detect edges using canny  
	Canny(src, canny_output, 80, 255, 3);
	/// Find contours  	
	findContours(canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_TC89_KCOS, Point(0, 0));
	//CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE  

	double maxarea = 0;
	int maxAreaIdx = 0;

	for (unsigned int i = 0; i<contours.size(); i++)
	{

		double tmparea = fabs(contourArea(contours[i]));
		if (tmparea>maxarea)
		{
			maxarea = tmparea;
			maxAreaIdx = i;
			continue;
		}

		if (tmparea < MIN_AREA)
		{
			//删除面积小于设定值的轮廓  
			contours.erase(contours.begin() + i);
			std::wcout << "delete a small area" << std::endl;
			continue;
		}
		//计算轮廓的直径宽高  
		Rect aRect = boundingRect(contours[i]);
		if ((aRect.width / aRect.height)<WHRATIO)
		{
			//删除宽高比例小于设定值的轮廓  
			contours.erase(contours.begin() + i);
			std::wcout << "delete a unnomalRatio area" << std::endl;
			continue;
		}
	}

	vector<vector<Point> > contours0;
	contours0.resize(contours.size());
	for( size_t k = 0; k < contours.size(); k++ )
		approxPolyDP(Mat(contours[k]), contours0[k], 3, true);

	/// Draw contours,彩色轮廓  
	dst = Mat::zeros(canny_output.size(), CV_8UC3);
	RNG rng;
	for (int i = 0; i< contours0.size(); i++)
	{
		//随机颜色  
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(dst, contours0, i, color, 2, 8, hierarchy, 0, Point());
	}
	// Create Window  
	char* source_window = "countors";
	namedWindow(source_window, CV_WINDOW_NORMAL);
	imshow(source_window, dst);
cv:; waitKey(0);

	return 0;
}

