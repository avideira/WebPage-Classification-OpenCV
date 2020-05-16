/*
	File:   gaborfilter.c
 	Author: António Videira
	Created on 19 February, 2013
	Function that extract the Gabor features from an image
	and display it.
	
*/

#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
using namespace cv;
using namespace std;

IplImage *imageRGB = 0;

float GaborF(int x, int y, int x0, int y0, float lambda, float theta, float phi, float sigma, float gamma){
	float xx =  (x-x0)*cos(theta) + (y-y0)*sin(theta);
	float yy = -(x-x0)*sin(theta) + (y-y0)*cos(theta); 
}

int main( int argc, char** argv ) 
{
	imageRGB  = cvLoadImage(argv[1], 1);
	IplImage *gray = cvCreateImage(cvGetSize(imageRGB),IPL_DEPTH_8U,1);
	cvCvtColor(imageRGB, gray, CV_RGB2GRAY);


	cvNamedWindow("Gray Image", 1);
	cvShowImage( "Gray Image", gray);
	cvWaitKey(0);
 	cvDestroyWindow("Gray Image");
}
