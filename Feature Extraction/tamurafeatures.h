/*
	File:   tamurafeatures.h
 	Author: António Videira
*/
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#ifndef TAMURA_FEATURES_H_
#define TAMURA_FEATURES_H_

/**************************************************************************** 	
Computes the Tamura Features:
There are a number of features of texture that they claim to correspond to
humam visual perception. This function extracts the features describing the
coarseness, contrast and durectionality.The first two (Contrast and Coarseness) 
are represent by single values, while directionality is split into 16 bins.  
This result in a vector of 18 attributes.
*****************************************************************************/

void 	ExtractTamuraFeat	(IplImage * , double *);

double 	Contrast			(IplImage *);

void 	Directionality		(IplImage * , int , int , double *);
double 	CalcDeltaV			(IplImage * , int , int );
double 	CalcDeltaH			(IplImage * , int , int );

double 	Coarseness			(IplImage *);
double 	avgNeigh			(IplImage *, int, int, int, double);

double 	GetPixelVal			(IplImage *, int, int);
int 	sizeLeadDiffValue	(int, int w, double**, int, unsigned int**);

#endif