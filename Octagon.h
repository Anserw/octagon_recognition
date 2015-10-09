#pragma once
#include "opencv2/imgproc/imgproc.hpp"
#include "math.h"

typedef struct simple_point{
	double x, y;
	simple_point(void) { x=0; y=0; }
	simple_point(int x0, int y0) { x = x0; y = y0; }
}octagon_point;

typedef struct simple_line{
	octagon_point a, b;
	double length, k;
	simple_line(octagon_point p1, octagon_point p2) { 
		a = p1; 
		b = p2; 
		length = sqrt(double((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y))); 
		k = (a.y-b.y) / (a.x-b.x);
	}
}octagon_line;

class Octagon
{
public:
	Octagon(void);
	Octagon(std::vector<cv::Point> contour);
	~Octagon(void);
	static bool isOctagon(std::vector<cv::Point> contour);
	//void compute

private:
	static double computeSimpleRatio(simple_point a, simple_point b, simple_point c);
	static double computeCrossRatio(simple_point a, simple_point b, simple_point c, simple_point d);
	static double computeCrossRatio(simple_line a, simple_line b, simple_line c, simple_line d);
	static double computeTan(double k1, double k2);
	static double computeTanToSin(double tan);

	double computeAngle(simple_line l1, simple_line l2);
	double computeCR(int i);
	void computeVectorCR(void);

private:
	std::vector<octagon_point> points;
	std::vector<octagon_line> lines;
	std::vector<double> vectorCR;
};

