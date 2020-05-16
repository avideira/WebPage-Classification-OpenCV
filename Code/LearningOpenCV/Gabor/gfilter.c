/*
	File:   gabor.c
 	Author: António Videira
	Created on 19 February, 2013
	Function that extract the Gabor features from
	an image. 
	
*/
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>
#include <math.h>
#include <opencv2/opencv.hpp>
#include "/home/antonio/OpenCV-2.4.0/modules/imgproc/src/gabor.cpp"
using namespace cv;
using namespace std;

int ks=21;
int ktype = CV_32F;
Mat dest;
Mat kernel;
Mat gray;
Mat imageRGB;
Mat src_f;

void calc()
{	
	CvSize dim = cvSize(ks,ks);
	double theta = 0; //The rotation angle of the kernel.
    double psi = CV_PI/2; //shift, in radians of the sinusoidal from center.
					//0 means that the max value in in center (symetrical, max positive)
    double lmbd = CV_PI; //It controls the harmonic's frequency
    double sigma = 3; 	//defining the size of the Gaussian envelope
						// Controls the spread (radius)of the kernel
	double gm = 10;	// Aspect ratio of the Gaussian curves
					// how elongated the filter is in the lateral direction
	kernel = getGaborKernel(dim, sigma, theta, lmbd, gm, psi, ktype);
	filter2D(gray, dest, ktype, kernel);
	printf("kernel size (%d, %d)", kernel.rows, kernel.cols); 
	Mat Lkernel(ks*20, ks*20, CV_32F);
	resize(kernel, Lkernel, Lkernel.size());
    Lkernel /= 2.;
    Lkernel += 0.5;
	namedWindow("Kernel", 1);
	imshow("Kernel", Lkernel);
	imshow("Process window", dest);
}

int main(int argc, char** argv)
{
	const char *imagename = argv[1];
	imageRGB  = imread(imagename,1);
	cvtColor(imageRGB, gray, CV_BGR2GRAY);
	imshow("imageGray", gray);
	if (!ks%2)
    {
        ks+=1;
    }
	namedWindow("Process window", 1);
    //createTrackbar("Sigma", "Process window", &pos_sigma, ks, calc);
    //createTrackbar("Lambda", "Process window", &pos_lm, 100, calc);
    //createTrackbar("Theta", "Process window", &pos_th, 180, calc);
    //createTrackbar("Psi", "Process window", &pos_psi, 360, calc);
    calc();
	
	waitKey(0);
	cvDestroyWindow( "imageGray" );
	cvDestroyWindow( "Process window" );
	cvDestroyWindow( "Kernel" );
	
    return 0;
}