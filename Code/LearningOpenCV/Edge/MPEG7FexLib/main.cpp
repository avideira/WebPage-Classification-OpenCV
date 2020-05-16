/***************************************************************
 * Name:      main.cpp
 * Purpose:   Illustrate the usage of the MPEG-7 Feature Extraction library
 *            uses OpenCV for image handling (as does the library)
 * Author:    Muhammet Bastan (bastan@cs.bilkent.edu.tr)
 * Created:   2010-02-20
 * Copyright: Muhammet Bastan (http://www.cs.bilkent.edu.tr/~bastan)
 * License:
 **************************************************************/

#include <iostream>
#include "FexWrite.h"
#include "include/Frame.h"
#include "include/Feature.h"
#include "highgui.h"
#include "cv.h"

int main( int argc, char* argv[] )
{
    IplImage* image = 0;

    // load an image
    if( argc < 2 )
        image = cvLoadImage("motor.jpg");
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

    cvReleaseImage(&image);

    delete frame;
	
    return 0;
}
