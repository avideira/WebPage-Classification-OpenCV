#include <cv.h>
#include <stdio.h>
#include <stdlib.h>
#include <highgui.h>

#ifndef EXTRACT_FEATURES_H
#define EXTRACT_FEATURES_H


//void 	ExtractFeatures	( IplImage*, char );
void 	BubbleSort		( float[], int );
void 	BubbleSort		( double a[], int );
void 	ChiSquare		( char, const char*, int);
int 	WriteinCSV		( char, int, double[], const char*);
void 	**CreateTable	( int , int );
void 	doPCA			( int , int , int , const char* , char );
int 	WriteinCSV		( double[], int, const char*, int );
int     WriteinCSV		( double[], const char* , int  );

int 	read_features_from_csv(const char*, char );

int compare(float x, float y);
void swap(float *x, float *y);
void sort(float table[], const int n);

#endif