#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include "QtGUI.h"

int main(int argv, char **args) {
	// Open the file.
	IplImage *img = cvLoadImage("..\\img\\fisheye_equisolid.png");
	if (!img) {
	        printf("Error: Couldn't open the image file.\n");
	        return 1;
	}

	// Display the image.
	cvNamedWindow("Image:", CV_WINDOW_AUTOSIZE);
	cvShowImage("Image:", img);

	// Wait for the user to press a key in the GUI window.
	cvWaitKey(0);

	// Free the resources.
	cvDestroyWindow("Image:");
	cvReleaseImage(&img);

	return 0;

	//QApplication app(argv, args);
	//QtGUI gui;
	//gui.show();

	//return app.exec();
}