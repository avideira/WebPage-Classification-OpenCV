/*
	File:   colorhistogram.h
 	Author: António Videira
*/
#include <cv.h>
#include <highgui.h>
#include <stdio.h>

#ifndef COLOR_HISTOGRAM_H_
#define COLOR_HISTOGRAM_H_

/**************************************************************************** 	
Computes a Simple Color Histogram:
The histogram is produced by discretization of the colours in the image 
into 32 bins, and counting the numbers of image pixels in each bin. A
bin correspond to part of the color intensity spectrum. High frequencies
in low bins indicate that the image has a lot of dark colors. High values
in the bins with a higher number indicate images with more light colors. 
****************************************************************************/

void 		CalcPixels			(int , IplImage*, double*);
IplImage* 	RGBtoHSV			(const IplImage*);
IplImage* 	convertImageRGBtoHSV(const IplImage *imageRGB);
void 		ExtractColorHist	(IplImage * , double *);

	
#endif