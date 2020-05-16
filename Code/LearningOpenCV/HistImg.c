/*
 	File:   HistImg.c
 	Author: António Videira
	Created on 12 February, 2013
	Resume: Plot the gray histogram (32 bins) of an RGB image.
		Calculate the number of pixels in each bin.
		An the density value of each pixels.
*/


#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <stdlib.h>

IplImage* src = 0;
IplImage* imgHistogram = 0;
IplImage* gray = 0;
CvHistogram* hist;

int main( int argc, char** argv ) {

	if( argc == 2 && (src=cvLoadImage(argv[1], 1))!= 0) {
		
   		int bins = 32;				//size of the histogram -1D histogram
    		int hsize[] = {bins};
    		float max_value = 0, min_value = 0;   	//max and min value of the histogram
    		float xranges[] = {0, 256}; 		//ranges - grayscale 0 to 256
    		float* ranges[] = {xranges};
		
		gray = cvCreateImage( cvGetSize(src), IPL_DEPTH_8U, 1); //create an 8 bit single channel image to hold a
		cvCvtColor(src, gray, CV_BGR2GRAY);			//grayscale version of the original picture

		//Create 3 windows to show the results
		cvNamedWindow("original", 1);
		cvNamedWindow("gray", 1);
		cvNamedWindow("histogram", 1);

		IplImage * planes[] = {gray}; 		//planes to obtain the histogram, in this case just one
		hist = cvCreateHist(1, hsize, CV_HIST_ARRAY, ranges, 1); //get the histogram and some info about it
		cvCalcHist(planes, hist, 0, NULL);
		cvGetMinMaxHistValue(hist, &min_value, &max_value);
		printf("min: %f, max: %f\n", min_value, max_value);
		
    		imgHistogram = cvCreateImage(cvSize(bins, 200), 8, 1); //create an 8 bits single channel image to hold the histogram
    		cvRectangle(imgHistogram, cvPoint(0,0), cvPoint(256,200), CV_RGB(255, 255, 255), -1);
    		int value=0;
		int normalized=0;
    		for (int i = 0; i < bins; i++) {	//draw the histogram
        		value = cvQueryHistValue_1D(hist, i);
        		normalized = cvRound(value * 200 / max_value);
       	 		cvLine(imgHistogram, cvPoint(i, 200), cvPoint(i, 200 - normalized), CV_RGB(0, 0, 0));
        		printf("%d\n", normalized);
    		}
		
		//show the image results
    		cvShowImage("original", src);
    		cvShowImage("gray", gray);
    		cvShowImage("histogram", imgHistogram);
    		cvWaitKey();
    		return (EXIT_SUCCESS);

	}
}
