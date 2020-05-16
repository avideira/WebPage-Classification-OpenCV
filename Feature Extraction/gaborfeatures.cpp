#include "cvGabor.h"
#include "cvGabor.cpp"
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <math.h>
#include <iostream>

#include "gaborfeatures.h"

using namespace std;
using namespace cv;


void ExtractGaborFeatures( IplImage *image , double* GaborVector )
{
	int i,j;
	int gaborbins = 36;
	double Sigma = 2*PI;  
	double F = sqrt(2.0);  
	
	Scalar     mean;
	Scalar     stddev;
	int 	index = 0;
	double GaborDescriptor[36] = {0};
	
	CvGabor *gabor1 = new CvGabor;
	
	for( i = 0; i < 3;  i++){ //3 scales
		for( j = 0; j < 6; j++){ //6 orientations
			double dPhi = (PI/(j+1));
			int iNu = (3 + i);
			
			gabor1->Init( dPhi , iNu, Sigma, F );

		/* 		IplImage *kernel = NULL;
			kernel = cvCreateImage( cvSize(gabor1->get_mask_width(), gabor1->get_mask_width()), IPL_DEPTH_8U, 1);
			kernel = gabor1->get_image(CV_GABOR_REAL);
			cvNamedWindow("Gabor Kernel", 1);
			cvShowImage("Gabor Kernel", kernel);
			cvWaitKey(0); */

			//IplImage *img = cvLoadImage( "lena.bmp", CV_LOAD_IMAGE_GRAYSCALE );
			
			IplImage *reimg = NULL;
			reimg = cvCreateImage(cvSize(image->width,image->height), IPL_DEPTH_8U, 1);
			
			gabor1->conv_img(image, reimg, CV_GABOR_MAG);
/* 			cvNamedWindow("Magnitude Response", 1);
			cvShowImage("Magnitude Response",reimg);
			cvWaitKey(0);
			cvDestroyWindow("Magnitude Response"); */
			
			Mat image = cvarrToMat(reimg);  
			cvReleaseImage(&reimg);
			// imshow("Mat",image);  
			meanStdDev( image, mean, stddev, Mat() );
			// cvWaitKey(0);
			
			index = (int)(j*2) + (int)(i*12);
			//Gabor descriptor is now formed as a vector of means and standard deviations of filter responses
			GaborDescriptor[index] = mean.val[0];
			GaborDescriptor[index+1] = stddev.val[0];
		}
	}
	
	//normalize vector - values between 0 and 1.
	double norm = 0.0;
   
	for( i = 0; i < gaborbins; i++)
	{
		norm += GaborDescriptor[i]*GaborDescriptor[i];
	}

	norm = sqrt( norm );

	for( i = 0; i < gaborbins; i++)
	{
		GaborDescriptor[i] = GaborDescriptor[i] / norm;
 
	}
	for( int i = 0; i < gaborbins; i++){
		GaborVector[i] 	= GaborDescriptor[i];		
	}

}
