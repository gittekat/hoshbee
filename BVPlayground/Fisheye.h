#pragma once

#include <cv.h>

class Fisheye
{
public:
	Fisheye(IplImage* img, const int centerX, const int centerY);
	~Fisheye(void);

	void deFisheye();

	void test();

private:
	
};

