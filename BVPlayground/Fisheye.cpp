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
			CvPoint p = getTranslatedPos(i, j, mCenterX, mCenterY);

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

	CvPoint test = getTranslatedPos(100, 100, mCenterX, mCenterY);
	cout << test.x << " x " << test.y << endl;
	test = getTranslatedPos(0, 0, mCenterX, mCenterY);
	cout << test.x << " x " << test.y << endl;
	test = getTranslatedPos(500, 500, mCenterX, mCenterY);
	cout << test.x << " x " << test.y << endl;
}

void Fisheye::deFisheye2() {
	int dstSizeX = 500;
	int dstSizeY = 500;
	IplImage* dst = cvCreateImage(cvSize(dstSizeX, dstSizeY), 8, 3);

	// distance of image plane
	double dist = 300.0;
	
	for (int i=0; i<mImage->width; ++i) {
		for (int j=0; j<mImage->height; ++j) {
			CvPoint p = getTranslatedPos(i, j, mCenterX, mCenterY);

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
			
			CvPoint3D64d v = rotateTheta(theta);
			CvPoint3D64d v2 = rotateAlpha(v, alpha);
			
			double t = dist / -v2.z;
			CvPoint3D64d v3 = multiply(v2, t);

			v3.x += 250.0;
			v3.y += 250.0;

			uchar* value = &((uchar*)(mImage->imageData + mImage->widthStep*i))[j*3];
			if (v3.x >= 0 && v3.x < dstSizeX && v3.y >= 0 && v3.y < dstSizeY)  {
				uchar* destPixel = &((uchar*)(dst->imageData + dst->widthStep*(int)v3.x))[(int)v3.y*3];
				destPixel[0] = value[0];
				destPixel[1] = value[1];
				destPixel[2] = value[2];
			}
		}
	}

	/*double angle = 45.0 * M_PI / 180.0;
	CvPoint3D64d v = rotateTheta(angle);
	cout << v.x << " " << v.y << " " << v.z << endl;

	CvPoint3D64d v2 = rotateAlpha(v, angle);
	cout << v2.x << " " << v2.y << " " << v2.z << endl;

	double t = dist / -v2.z;

	CvPoint3D64d v3 = multiply(v2, t);
	cout << v3.x << " " << v3.y << " " << v3.z << endl;*/

	cvNamedWindow("Image:", CV_WINDOW_AUTOSIZE);
	cvShowImage("Image:", dst);

	// Wait for the user to press a key in the GUI window.
	cvWaitKey(0);

	cvSaveImage("perspective.png", dst);

	// Free the resources.
	cvDestroyWindow("Image:");
	cvReleaseImage(&dst);
}

void Fisheye::deFisheye3() {
	int dstSizeX = 500;
	int dstSizeY = 500;
	IplImage* dst = cvCreateImage(cvSize(dstSizeX, dstSizeY), 8, 3);

	//IplImage *polarImage = getPolarCoordImage(mImage);

	// distance of image plane
	double dist = 300.0;

	for (int i=0; i<dst->width; ++i) {
		for (int j=0; j<dst->height; ++j) {
			// translate pos
			CvPoint pt = getTranslatedPos(i, j, dst->width/2, dst->height/2);
			
			double perspectiveRadius = getRadius(pt.x, pt.y);
			double theta = atan(perspectiveRadius/300.0); // perspective focal length?

			double fisheyeRadius = getFisheyeRadius(mFocalLength, theta);
			double fisheyeRadius_deg = rad2Deg(fisheyeRadius);
			double theta_deg = rad2Deg(theta);

			double alpha = getAngle(pt.x, pt.y);
			double alpha_deg = rad2Deg(alpha);
				
			if (alpha < 0.0) {
				alpha = alpha + 2.0 * M_PI;
			}
			if (alpha > 2.0 * M_PI) {
				alpha = 2.0 * M_PI;
			}
			alpha_deg = rad2Deg(alpha);

			double x = fisheyeRadius * cos(alpha);
			double y = fisheyeRadius * sin(alpha);

			// scale to fisheye image size
			x *= mImage->width/2;
			y *= mImage->height/2;

			// TODO get sub pixel value!!!

			// translate to image coordinates
			//CvPoint imgCoord = getTranslatedPos((int)x, (int)y, mImage->width/2, mImage->height/2);
			int x2 = x + mImage->width/2;
			int y2 = y + mImage->height/2;
			CvPoint imgCoord = {x2, y2};
			
			if (imgCoord.x < 0 || imgCoord.x > mImage->width || imgCoord.y < 0 || imgCoord.y > mImage->height) {
				continue;
			}

			uchar* value = &((uchar*)(mImage->imageData + mImage->widthStep*imgCoord.x))[imgCoord.y*3];

			uchar* destPixel = &((uchar*)(dst->imageData + dst->widthStep*i))[j*3];
			destPixel[0] = value[0];
			destPixel[1] = value[1];
			destPixel[2] = value[2];

		}
	}

	cvNamedWindow("Image:", CV_WINDOW_AUTOSIZE);
	cvShowImage("Image:", dst);

	// Wait for the user to press a key in the GUI window.
	cvWaitKey(0);

	cvSaveImage("reverse.png", dst);

	// Free the resources.
	cvDestroyWindow("Image:");
	cvReleaseImage(&dst);
}

IplImage* Fisheye::getPolarCoordImage(IplImage* src) {
	
	IplImage* dst = cvCreateImage( cvSize(256,256), 8, 3 );
	IplImage* src2 = cvCreateImage( cvGetSize(src), 8, 3 );
	
	cvLogPolar( src, dst, cvPoint2D32f(src->width/2,src->height/2), 40,
		CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS );
	cvLogPolar( dst, src2, cvPoint2D32f(src->width/2,src->height/2), 40,
		CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS+CV_WARP_INVERSE_MAP );
	
	cvNamedWindow( "log-polar", 1 );
	cvShowImage( "log-polar", dst );
	cvNamedWindow( "inverse log-polar", 1 );
	cvShowImage( "inverse log-polar", src2 );
	cvWaitKey();

	cvReleaseImage(&src2);

	return dst;
}

inline double Fisheye::getAngle(int x, int y) {
	return atan2((double)x, (double)y);
}

inline double Fisheye::getDefishAngle(const double radius) {
	double tmp = radius / (2.0 * mFocalLength);
	double angle = 2.0 * asin(tmp);

	return angle;
}

inline double Fisheye::getFisheyeRadius(const double focalLength, const double theta) {
	return (2.0 * focalLength * sin (theta/2.0));
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

inline CvPoint Fisheye::getTranslatedPos(const int x, const int y, const int centerX, const int centerY) {
	CvPoint res = {x - centerX, centerY - y};
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