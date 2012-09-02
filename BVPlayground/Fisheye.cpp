#include "Fisheye.h"

IplImage* mImage;
int mCenterX;
int mCenterY;

#include <cxcore.h>
#include <highgui.h>


Fisheye::Fisheye(IplImage* img, const int centerX, const int centerY)
{
	mImage = img;
	mCenterX = centerX;
	mCenterY = centerY;
}

Fisheye::~Fisheye(void)
{
}

void Fisheye::deFisheye() {
	for (int i=1; i<mImage->width; ++i) {
		for (int j=1; j<mImage->height; ++j) {
			CvPoint pt = {i,j};

			/*double x = (double)i + (double) img->width / 2.0;
			double y = (double)j + (double) img->height / 2.0;
			double r = x / y;
			double alpha = tan(r);
			double alpha2 = (alpha * 180.0) / M_PI;*/
			//double alpha2 = rad2deg(alpha);
			//std::cout << alpha2 << std::endl;
			uchar* temp_ptr = &((uchar*)(mImage->imageData + mImage->widthStep*pt.y))[pt.x*3];
			temp_ptr[0] = 0;
			temp_ptr[1] = 0;
			temp_ptr[2] = 255;
		}
	}
}

void Fisheye::test() {
	IplImage* dst = cvCreateImage( cvSize(256,256), 8, 3 );
	IplImage* src2 = cvCreateImage( cvGetSize(mImage), 8, 3 );
	cvLogPolar( mImage, dst, cvPoint2D32f(mImage->width/2,mImage->height/2), 40,
		CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS );
	cvLogPolar( dst, src2, cvPoint2D32f(mImage->width/2,mImage->height/2), 40,
		CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS+CV_WARP_INVERSE_MAP );
	cvNamedWindow( "log-polar", 1 );
	cvShowImage( "log-polar", dst );
	cvNamedWindow( "inverse log-polar", 1 );
	cvShowImage( "inverse log-polar", src2 );
	cvWaitKey();
}