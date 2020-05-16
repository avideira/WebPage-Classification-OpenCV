#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef EXTRACT_FEATURES_H
#define EXTRACT_FEATURES_H


void 	ExtractFeatures	( IplImage*, const char*, int );
int 	SaveFeatures	( const char* );
int SaveFeatures2( const char* , double *, int  );
void 	InitTable		( );


#endif