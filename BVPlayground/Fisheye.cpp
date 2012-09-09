#include "Fisheye.h"

#include <cxcore.h>
#include <highgui.h>

IplImage* mImage;
int mCenterX;
int mCenterY;
double mFocalLength;
double mRadius;

Fisheye::Fisheye(IplImage* img, const double focalLength, const double radius, const int centerX, const int centerY)
{
	mImage = img;
	mFocalLength = focalLength;
	mRadius = radius;
	mCenterX = centerX;
	mCenterY = centerY;
}

Fisheye::~Fisheye(void)
{
}

void Fisheye::deFisheye() {
	int texSize = 720;
	IplImage* dst = cvCreateImage(cvSize(texSize, texSize), 8, 3);
	double scaleFactor = (double) texSize / 360.0;

	for (int i=0; i<mImage->width; ++i) {
		for (int j=0; j<mImage->height; ++j) {
			CvPoint p = getTranslatedPos(i, j);

			double r = getRadius(p.x, p.y);
			r = r / mRadius;
			if (r > 1.0) {
				continue;
			}

			double theta = getDefishAngle(r);
			double test = rad2Deg(theta);

			double alpha = getAngle(p.x, p.y);
			double test2 = rad2Deg(alpha);

			int theta2 = (int)rad2Deg(theta * 2.0 * scaleFactor);

			if (alpha < 0.0) {
				alpha = alpha + 2.0 * M_PI;
			}
			if (alpha > 2.0 * M_PI) {
				alpha = 2.0 * M_PI;
			}
			int alpha2 = (int)rad2Deg(alpha * scaleFactor);

			CvPoint pt = {i,j};

			int x = i - mCenterX;
			int y = j - mCenterY;

			uchar* temp_ptr = &((uchar*)(mImage->imageData + mImage->widthStep*pt.y))[pt.x*3];

			uchar* temp_ptrDst = &((uchar*)(dst->imageData + dst->widthStep*theta2))[alpha2*3];
			temp_ptrDst[0] = temp_ptr[0];
			temp_ptrDst[1] = temp_ptr[1];
			temp_ptrDst[2] = temp_ptr[2];
		}
	}

	cvNamedWindow("Image:", CV_WINDOW_AUTOSIZE);
	cvShowImage("Image:", dst);

	// Wait for the user to press a key in the GUI window.
	cvWaitKey(0);

	cvSaveImage("texture.png", dst);

	// Free the resources.
	cvDestroyWindow("Image:");
	cvReleaseImage(&dst);


	cout << rad2Deg(getAngle(-100, 100)) << " = 315" << endl;
	cout << rad2Deg(getAngle(0, 100)) << " = 0" << endl;
	cout << rad2Deg(getAngle(100, 100)) << " = 45" << endl;
	cout << rad2Deg(getAngle(100, -100)) << " = 135" << endl;
	cout << rad2Deg(getAngle(-100, -100)) << " = 225 " << endl;

	cout << rad2Deg(getDefishAngle(1.0)) << endl;
	cout << rad2Deg(getDefishAngle(1.2)) << endl;
	cout << rad2Deg(getDefishAngle(1.4)) << endl;
	cout << rad2Deg(getDefishAngle(1.5)) << endl;

	cout << getScaledPos(0) << endl;
	cout << getScaledPos(100) << endl;
	cout << getScaledPos(500) << endl;
	cout << getScaledPos(mRadius) << " = " << 2.0 * mFocalLength << endl;

	CvPoint test = getTranslatedPos(100, 100);
	cout << test.x << " x " << test.y << endl;
	test = getTranslatedPos(0, 0);
	cout << test.x << " x " << test.y << endl;
	test = getTranslatedPos(500, 500);
	cout << test.x << " x " << test.y << endl;
}

void Fisheye::deFisheye2() {
	int texSize = 500;
	IplImage* dst = cvCreateImage(cvSize(texSize, texSize), 8, 3);

	// distance of image plane
	double dist = 300.0;
	
	for (int i=0; i<mImage->width; ++i) {
		for (int j=0; j<mImage->height; ++j) {
			CvPoint p = getTranslatedPos(i, j);

			double r = getRadius(p.x, p.y);
			r = r / mRadius;
			if (r > 1.0) {
				continue;
			}

			double theta = getDefishAngle(r);
			double theta_deg = rad2Deg(theta);

			double alpha = getAngle(p.x, p.y);
			double alpha_deg = rad2Deg(alpha);
				
			if (alpha < 0.0) {
				alpha = alpha + 2.0 * M_PI;
			}
			if (alpha > 2.0 * M_PI) {
				alpha = 2.0 * M_PI;
			}
			
			CvPoint pt = {i,j};
			
			/*int x = i - mCenterX;
			int y = j - mCenterY;*/

			uchar* value = &((uchar*)(mImage->imageData + mImage->widthStep*pt.y))[pt.x*3];

			CvPoint3D64d v = rotateTheta(theta);
			//cout << v.x << " " << v.y << " " << v.z << endl;

			CvPoint3D64d v2 = rotateAlpha(v, alpha);
			//cout << v2.x << " " << v2.y << " " << v2.z << endl;
			
			double t = dist / -v2.z;
			CvPoint3D64d v3 = multiply(v2, t);
			//cout << v3.x << " " << v3.y << " " << v3.z << endl;

			v3.x += 250.0;
			v3.y += 250.0;

			if (v3.x >= 0 && v3.x < 500 && v3.y >= 0 && v3.y < 500)  {
				uchar* destPixel = &((uchar*)(dst->imageData + dst->widthStep*(int)v3.x))[(int)v3.y*3];
				destPixel[0] = value[0];
				destPixel[1] = value[1];
				destPixel[2] = value[2];
			}
		}
	}

	double angle = 45.0 * M_PI / 180.0;
	CvPoint3D64d v = rotateTheta(angle);
	cout << v.x << " " << v.y << " " << v.z << endl;

	CvPoint3D64d v2 = rotateAlpha(v, angle);
	cout << v2.x << " " << v2.y << " " << v2.z << endl;

	double t = dist / -v2.z;

	CvPoint3D64d v3 = multiply(v2, t);
	cout << v3.x << " " << v3.y << " " << v3.z << endl;

	cvNamedWindow("Image:", CV_WINDOW_AUTOSIZE);
	cvShowImage("Image:", dst);

	// Wait for the user to press a key in the GUI window.
	cvWaitKey(0);

	cvSaveImage("perspective.png", dst);

	// Free the resources.
	cvDestroyWindow("Image:");
	cvReleaseImage(&dst);
}

inline double Fisheye::getAngle(int x, int y) {
	return atan2((double)x, (double)y);
}

inline double Fisheye::getDefishAngle(const double radius) {
	double tmp = radius / (2.0 * mFocalLength);
	double angle = 2.0 * asin(tmp);

	return angle;
}

inline double Fisheye::getRadius(double x, double y) {
	return sqrt(x * x + y * y);
}

inline double Fisheye::rad2Deg(const double rad) {
	return (rad * 180.0) / M_PI;
}
inline double Fisheye::deg2Rad(const double deg) {
	return (deg * M_PI) / 180.0;
}

inline CvPoint Fisheye::getTranslatedPos(const int x, const int y) {
	CvPoint res = {x - mCenterX, mCenterY - y};
	return res;
}

inline double Fisheye::getScaledPos(int value) {
	return ((2.0 * mFocalLength) / mRadius) * value;
}

inline CvPoint3D64d Fisheye::rotateTheta(double theta) {
	double y = -sin(theta);
	double z = cos(theta);
	CvPoint3D64d v = {0, y, z};
	return v;

}

inline CvPoint3D64d Fisheye::rotateAlpha(CvPoint3D64d v, double alpha) {
	CvPoint3D64d res = {v.x * cos(alpha) + v.y * -sin(alpha), v.x * sin(alpha) + v.y * cos(alpha), v.z};
	return res;
}

inline CvPoint3D64d Fisheye::multiply(CvPoint3D64d v, double factor) {
	CvPoint3D64d res = {v.x*factor, v.y*factor, v.z*factor};
	return res;
}