/*
	File:   edgehistogram.h
 	Author: António Videira
*/
#include <cv.h>
#include <highgui.h>
#include <stdio.h>

#ifndef EDGE_HISTOGRAM_H_
#define EDGE_HISTOGRAM_H_

/**************************************************************************** 	
Computes the Edge Histogram:
Edge Histogram descriptor represents the spatial distribution of five types
of edges(four directional edges and one non-directional). The image space is 
divided into 16 (4x4) non-overlapping sub-images and for each sub-image a
histogram with five edge bins is generated. This result in a descriptor with
80 attributes.
*****************************************************************************/

void 		ExtractEdgeHist		(double* , IplImage*);
double 		FirstBlock			(int , int , int , IplImage*);
double 		SecondBlock			(int , int , int , IplImage*);
double 		ThirdBlock			(int , int , int , IplImage*);
double 		FourthBlock			(int , int , int , IplImage*);
int			GetBlockSize		(unsigned int , unsigned int);
int 		GetEdgeFeature		(int , int , unsigned short int, IplImage*);
void 		EdgeCalculation		(unsigned int , unsigned int , unsigned short int , double* , IplImage*);

#endif