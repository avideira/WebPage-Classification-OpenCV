#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

IplImage * Img = 0;

int main(int argc, char** argv)
{

	Img = cvLoadImage(argv[1], 1);
	cvNamedWindow("Imagem",0);
	cvRectangle(Img,cvPoint(0,0),cvPoint(350,350),CV_RGB(255,0,0),-1,0,0);
	cvLine(Img,cvPoint(100,100), cvPoint(500,200), CV_RGB(0,0,0),0,0,0);
	cvShowImage("Imagem",Img);
	cvWaitKey(0);
	cvReleaseImage(&Img);
	return 0;
}