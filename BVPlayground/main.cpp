#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

int main()
{
        // Open the file.
        IplImage *img = cvLoadImage("d:\\Stuff\\temp\\lange haare.jpg");
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
}