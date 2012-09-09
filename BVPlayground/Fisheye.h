#pragma once

#define _USE_MATH_DEFINES
#include <iostream>
#include <cv.h>
#include <math.h>

using namespace std;

class Fisheye
{
public:
	Fisheye(IplImage* img, const double focal_length, const double radius, const int centerX, const int centerY);
	~Fisheye(void);

	void deFisheye();
	void deFisheye2();
	void deFisheye3();

private:
	inline double getAngle(const int x, const int y);
	inline double getDefishAngle(const double radius);
	inline double getFisheyeRadius(const double focalLength, const double theta);
	inline double getRadius(double x, double y);
	inline double getScaledPos(int value);
	inline CvPoint getTranslatedPos(const int x, const int y, const int centerX, const int centerY);
	inline double rad2Deg(const double rad);
	inline double deg2Rad(const double deg);

	CvScalar getSubPixel(IplImage* src, double x, double y);
	IplImage* getPolarCoordImage(IplImage* src);

	inline CvPoint3D64d rotateTheta(double theta);
	inline CvPoint3D64d rotateAlpha(CvPoint3D64d v, double alpha);
	inline CvPoint3D64d multiply(CvPoint3D64d v, double factor);
};

