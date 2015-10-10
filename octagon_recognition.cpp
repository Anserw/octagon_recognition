// octagon_recognition.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

#include "Octagon.h"

using namespace cv;
using namespace std;

#define MIN_AREA 100
#define WHRATIO 0.1

bool getOctagon(const char* filename, vector<Octagon>& octagons, Mat& src, vector<vector<Point>>& contours, vector<Vec4i>& hierarchy)
{
	Mat dst, canny_output;
	/// Load source image and convert it to gray  
	src = imread(filename, 0);

	if (!src.data)
	{
		std::cout << "read data error!" << std::endl;
		return -1;
	}
	blur(src, src, Size(3, 3));	

	/// Detect edges using canny  
	Canny(src, canny_output, 80, 255, 3);
	/// Find contours  	
	findContours(canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_TC89_KCOS, Point(0, 0));	

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
	
	for( size_t k = 0; k < contours0.size(); k++ ) {
		if (Octagon::isOctagon(contours0[k])) {
			octagons.push_back(Octagon(contours0[k]));
		}
	}
}



int main()
{	
	bool a;
	Mat img1, img2;
	vector<Octagon> octagons;
	vector<int> octagons_to_draw;
	Octagon octagon0;
	//the pram. for findContours,  
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	getOctagon("image\\octagon.jpg", octagons, img1, contours, hierarchy);
	if (octagons.size()) {
		octagon0 = octagons[0];
		octagons.clear();
	}
	contours.clear();
	hierarchy.clear();
	getOctagon("image\\recognition1.jpg", octagons, img2, contours, hierarchy);
	for (int i=0;i<octagons.size();i++) {
		if (octagon0.isSame(&octagons[i])) {
			octagons_to_draw.push_back(i);
		}
	}


	/// Draw contours
	Mat dst;
	dst = Mat::zeros(img2.size(), CV_8UC3);
	
	for (int i = 0; i< octagons_to_draw.size(); i++)
	{
		
		Scalar color = Scalar(255, 255, 0);
		drawContours(dst, contours, octagons_to_draw[i], color, 2, 8, hierarchy, 0, Point());
	}
	// Create Window  
	
	char* source_window = "countors";
	namedWindow(source_window, CV_WINDOW_NORMAL);
	imshow(source_window, dst);
cv:; waitKey(0);

	return 0;
}

