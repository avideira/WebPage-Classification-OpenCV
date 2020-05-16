/*
	File:   gabor.c
 	Author: António Videira
	Created on 19 February, 2013
	Function that extract the Gabor features from
	an image. 
	
*/
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <math.h>
#include <opencv2/opencv.hpp>
#include "/home/antonio/OpenCV-2.4.0/modules/imgproc/src/gabor.cpp"
using namespace cv;
using namespace std;

//Mat getGaborKernel(Size ksize, double sigma, double theta, double lambd, double gamma, double psi, int ktype);
Mat dest;
Mat gaborKernel;

int process(){
	double sigma = 3.0;  //defining the size of the Gaussian envelope 
	double gamma = 10; // Aspect ratio of the Gaussian curves
	double psi = CV_PI*0.5; //Phase
	double theta = 0;
	double lambd = CV_PI;
	int ktype = CV_64F;
	CvSize ksize = cvSize(9,9);
	printf("Vai calcular\n");
	gaborKernel = getGaborKernel(ksize,sigma,theta, lambd,gamma,psi,ktype);
	
	
	return 0;
	
}

int main( int argc, char** argv ) 
{
	Mat imageRGB, gray;
	imageRGB  = imread("lena.bmp");
	//double theta = 0, lambd = CV_PI/2;
	cvtColor(imageRGB, gray, CV_BGR2GRAY);
	//imshow("imageRGB", imageRGB);
	imshow("imageGray", gray);

	process();
	imshow("kernel", gaborKernel);
		cvResizeWindow("kernel",400, 400);

	filter2D(gray,dest,CV_32F, gaborKernel);
	imshow("process window", dest);

	//cvNamedWindow("Gray Image", 1);
	//cvShowImage( "Gray Image", gray);
	cvWaitKey(0);
 	//cvDestroyWindow("Gray Image");  
}
