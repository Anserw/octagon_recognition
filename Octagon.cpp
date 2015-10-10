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
	std::vector<octagon_point> l_points;
	for (int i=0; i<contour.size(); i++) {
		if (i == contour.size()-1
			||simple_line(simple_point(contour[i].x, contour[i].y), simple_point(contour[i+1].x, contour[i+1].y)).length > 10) {
			l_points.push_back(octagon_point(contour[i].x, contour[i].y));
		}else {
			l_points.push_back(octagon_point((contour[i].x+contour[i+1].x)/2, (contour[i].y+contour[i+1].y)/2));
			i++;
		}
		
	}
	for (int i=0; i<7; i++) {
		l_lines.push_back(octagon_line(l_points[i], l_points[i+1]));
	}
	l_lines.push_back(octagon_line(l_points[7], l_points[0]));
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
	for (int i=0;i<8;i++) {
		points.push_back(l_points[(t+i)%8]);
	}
	computeVectorCR();
	int a;
	a = 1;
	accurate = 0.14;		
}

bool Octagon::isOctagon(std::vector<cv::Point> contour)
{
	int count=1;
	for (int i=1;i<contour.size();i++) {
		if (simple_line(simple_point(contour[i].x, contour[i].y), simple_point(contour[i-1].x, contour[i-1].y)).length > 10) {
			count++;
		}
	}
	if (count == 8){
		return true;
	}else {
		return false;
	}
}

bool Octagon::isSame(Octagon* targetOctagon)
{
	for (int i=0;i<8;i++) {
		if (abs(vectorCR[i] - targetOctagon->getVectorCR()[i]) > accurate) {
			return false;
		}
	}
	return true;
}

const std::vector<double>& Octagon::getVectorCR()
{
	return vectorCR;
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


double Octagon::computeAngle(simple_line l1, simple_line l2)
{	
	double ab, ac, bc;
	ab = l1.length;
	ac = l2.length;
	bc = simple_line(l1.b, l2.b).length;
	return sin(acos((ab*ab + ac*ac - bc*bc)/(2*ab*ac)));
}

double Octagon::computeCR(int n)
{
	vector<simple_line> l;
	simple_point *p = &points[n];
	for (int i=1;i<8;i+=2) {
		l.push_back(simple_line(*p, points[(n+i)%8]));
	}	
	return computeAngle(l[0], l[2]) * computeAngle(l[1], l[3]) / (computeAngle(l[0], l[3]) * computeAngle(l[1], l[2]));
}

void Octagon::computeVectorCR()
{
	for (int i=0;i<8;i++) {
		vectorCR.push_back(computeCR(i));
	}
}