#include <opencv/cv.h>
#include <stdio.h>
#include <math.h>
#include <opencv.h>
#include <highgui.h>
#include "FexWrite.h"
#include "include/Frame.h"
//using namespace cv;
//using namespace std;

int main( int argc, char* argv[] )
{
 
	IplImage* image = 0;

    // load an image
    if( argc < 2 )
        image = cvLoadImage("lena.bmp");
    else
        image = cvLoadImage( argv[1] );

    // display the image, if OpenCV is compiled with GTK or Carbon support!
    //cvNamedWindow("image");
    //cvShowImage("image", image);
    //cvWaitKey();

    ///-- extract global image descriptors --

    // create a Frame object (see include/Frame.h)
    // allocate memory for 3-channel color and 1-channel gray image and mask
    Frame* frame = new Frame( image->width, image->height, true, true, true);

    // set the image of the frame
    frame->setImage(image);
	// EHD
    FexWrite::computeWriteEHD( frame );
}