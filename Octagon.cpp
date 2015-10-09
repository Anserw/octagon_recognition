#include "StdAfx.h"
#include "Octagon.h"

using namespace std;

Octagon::Octagon(void)
{
}


Octagon::~Octagon(void)
{
}

Octagon::Octagon(std::vector<cv::Point> contour)
{
	std::vector<octagon_line> l_lines;
	for (int i=0; i<contour.size(); i++) {
		points.push_back(octagon_point(contour[i].x, contour[i].y));
	}
	for (int i=0; i<7; i++) {
		l_lines.push_back(octagon_line(points[i], points[i+1]));
	}
	l_lines.push_back(octagon_line(points[7], points[0]));
	int t=0;
	double maxlength;
	maxlength = l_lines[0].length;	
	for (int i=1; i<8; i++) {
		if (l_lines[i].length > maxlength) {
			maxlength = l_lines[i].length;
			t = i;
		}
	}
	for (int i=0; i<8; i++) {
		lines.push_back(l_lines[(t+i)%8]);
	}
	computeVectorCR();
	int a;
	a = 1;
		
}

bool Octagon::isOctagon(std::vector<cv::Point> contour)
{
	if (contour.size() == 8){
		return true;
	}else {
		return false;
	}
}

double Octagon::computeSimpleRatio(simple_point a, simple_point b, simple_point c) 
{
	return simple_line(a, c).length / simple_line(b, c).length;
}

double Octagon::computeCrossRatio(simple_point a, simple_point b, simple_point c, simple_point d)
{
	return computeSimpleRatio(a, b, c) / computeSimpleRatio(a, b, d);
}

double Octagon::computeCrossRatio(simple_line a, simple_line b, simple_line c, simple_line d)
{
	//TODO
	return 0;
}

double Octagon::computeTan(double k1, double k2)
{
	return (k2 - k1)/(1 + k1*k2);
}

double Octagon::computeTanToSin(double tan)
{
	return sin(atan(tan));
}

double Octagon::computeAngle(simple_line l1, simple_line l2)
{
	return computeTanToSin(computeTan(l1.k, l2.k));
}

double Octagon::computeCR(int n)
{
	vector<simple_line> l;
	simple_point *p = &points[n];
	for (int i=1;i<5;i++) {
		l.push_back(simple_line(*p, points[(n+i)%8]));
	}
	double tt1, tt2, tt3;
	tt1 = computeAngle(l[0], l[1]);
	tt2 = computeAngle(l[1], l[2]);
	tt3 = computeAngle(l[2], l[3]);
	return (tt1 + tt2) * (tt2 + tt3) / ((tt1 + tt2 + tt3) * (tt2));
}

void Octagon::computeVectorCR()
{
	for (int i=0;i<8;i++) {
		vectorCR.push_back(computeCR(i));
	}
}