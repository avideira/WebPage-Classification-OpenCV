/*
 	Author: António Videira
*/
#include "colorhistogram.h"
#include "edgehistogram.h"
#include "gaborfeatures.h"
#include "tamurafeatures.h"
#include "ExtractFeatures.h"
#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <vector>
#include <string>
using namespace std;

#define NUMBER_OF_TOTAL_FEATURES 	166 

static const int Colorbins 	= 32;	
static const int Edgebins 	= 80;	
static const int tamuraF 	= 18;	
static const int gaborF		= 36;

double  *Features;

void InitTable()
{
	Features = (double*)malloc((NUMBER_OF_TOTAL_FEATURES)*sizeof(double*));
}

void ExtractFeatures( IplImage* imageRGB, const char* directory, int coars) 
{
	
	double 		RGBdescriptor[Colorbins] 	= {0};
	double 		EdgeH[Edgebins] 			= {0};
	double 		TamuraFeat[tamuraF]			= {0};
	double 		GaborFeat[gaborF] 			= {0};
	
	IplImage *imageGray;
	imageGray 	= 	cvCreateImage(cvGetSize(imageRGB), 8, 1);
	cvCvtColor(imageRGB, imageGray, CV_BGR2GRAY );
	
	  /***********************************************************/
	 /* 				SIMPLE COLOR HISTOGRAM					*/
	/***********************************************************/
 	ExtractColorHist(imageRGB , RGBdescriptor);

	  /***********************************************************/
	 /* 						EDGE HISTOGRAM					*/
	/***********************************************************/
	ExtractEdgeHist(EdgeH , imageGray);

	  /***********************************************************/
	 /* 					TAMURA FEATURES						*/
	/***********************************************************/
	ExtractTamuraFeat(imageGray , TamuraFeat, coars);

	  /***********************************************************/
	 /* 						GABOR FEATURES					*/
	/***********************************************************/
	ExtractGaborFeatures(imageGray, GaborFeat);

	for(int i=0; i<NUMBER_OF_TOTAL_FEATURES; i++){
	
		if( i < Colorbins ){	
			Features[i] = RGBdescriptor[i];
		}
		if( i >= Colorbins && i < (Colorbins+Edgebins) ){	
			Features[i] = EdgeH[i-Colorbins];
		}
		if( i >= (Colorbins+Edgebins) && i < (Colorbins+Edgebins+tamuraF) ){		
			Features[i] = TamuraFeat[i-(Colorbins+Edgebins)];
		}
		if( i >= (Colorbins+Edgebins+tamuraF) && i < NUMBER_OF_TOTAL_FEATURES ){			
			Features[i] = GaborFeat[i-(Colorbins+Edgebins+tamuraF)];
		}
	}
	
	SaveFeatures (directory);
	cvReleaseImage(&imageGray);
	return;
}

/* 	
	Save the variables that characterize the image in a csv file, separable by a comma. 
	FLAG (single value) - regards to the type of the class of each image.
*/	
int SaveFeatures( const char* file )
{
	int i = 0;
	ofstream myfile;
	
	myfile.open (file, ios::app);
	//myfile << flag << ",";
	
	while( i < (NUMBER_OF_TOTAL_FEATURES-1) )
	{
		myfile << Features[i] << ",";
		i++;
	}
	myfile << Features[i] << endl;
	myfile.close();
	fflush(NULL);
	return 0; 
}

int SaveFeatures2( const char* file, double *feats, int numberoffeats )
{
	int i = 0;
	ofstream myfile;
	
	myfile.open (file, ios::app);
	//myfile << flag << ",";
	
	while( i < (numberoffeats-1) )
	{
		myfile << feats[i] << ",";
		i++;
	}
	myfile << feats[i] << endl;
	myfile.close();
	fflush(NULL);
	return 0; 
}
