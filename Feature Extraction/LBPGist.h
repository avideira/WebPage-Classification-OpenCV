#include <cv.h>
#include <stdio.h>
#include <stdlib.h>
#include <highgui.h>

#ifndef LBP_GIST_H
#define LBP_GIST_H

CvScalar 	GetAvgImage			( IplImage*, int, int, int, int ); 
void 		ExtractGISTFeatures	( IplImage*, int, int, char );
void 		BubbleSort			( float[], int );
void 		ChiSquare			( int  , int , int , int , int , const char*, char );
int 		WriteinCSV			( double[], int, const char* , int );
int 		WriteinCSV			( float[], int, const char*, int );
void 		**CreateTable		( int, int, int, int );
void 		doPCA				( int  , int , int , int , int , const char*, char );

void sort(float[], const int );
void swap(float *, float *);
int compare(float , float );





#endif