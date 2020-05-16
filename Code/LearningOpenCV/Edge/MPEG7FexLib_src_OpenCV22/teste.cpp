#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <math.h>
#include <cxcore.h>
#include "Feature.h"
#include "Convert.h"
#include "Frame.h"
#include "Descriptors.h"
#include "EdgeHistExtraction.h"
#include "EdgeHist.h"

using namespace cv;
using namespace std;
int main( int argc, char* argv[] )
{
    Mat image;
    // load an image
    if( argc < 2 )
        image = imread("lena.bmp");
    else
        image = imread( argv[1] );
    // display the image
    //namedWindow("image");
    //imshow("image", image);
    //waitKey();
	// create a Frame object (see include/Frame.h)
    // allocate memory for 3-channel color and 1-channel gray image and mask
    Frame* frame = new Frame( image.cols, image.rows, true, true, true);
	// set the image of the frame
    frame->setImage(image);
	
    // compute the descriptor
	XM::EdgeHistogramDescriptor* ehd = Feature::getEdgeHistogramD( frame );

    // get a pointer to the values
	char* de = ehd->GetEdgeHistogramElement();

	// write to screen
	for( unsigned int i = 0; i < ehd->GetSize(); i++)
		std::cout << (int)de[i] << " ";
	std::cout  << std::endl;

    // release the descriptor
	delete ehd;
}