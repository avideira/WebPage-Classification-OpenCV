/*
	File:   gaborfeatures.h
 	Author: António Videira
*/
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <stdlib.h>
using namespace cv;

#ifndef GABOR_FEATURES_H_
#define GABOR_FEATURES_H_

/**************************************************************************** 	
Computes the Gabor Features:
Gabor filters have been used extensively as a model of texture for image
interpretation tasks. This function result in 36 attributes.
*****************************************************************************/

void 	ExtractGaborFeatures	( IplImage * , double* );

#endif