#include "colorhistogram.h"
#include "edgehistogram.h"
#include "gaborfeatures.h"
#include "tamurafeatures.h"
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
#define NUMBER_OF_TRAINING_SAMPLES 60 //(30, 60 or 90)
#define NUMBER_OF_TESTING_SAMPLES 80 //(20 or 40)

static const int Colorbins 	= 32;	
static const int Edgebins 	= 80;	
static const int tamuraF 	= 18;	
static const int gaborF		= 36;

double  *Features;
void InitTable();
void ExtractFeatures( IplImage* , const char* ) ;
int SaveFeatures( const char* );
IplImage* img_resize(IplImage* , int , int );
void Extract( char* , char* );
void ProcessImages(const char* , char* , int );
int WebPageTopic( int , int );
int WebPageRecency( int , int );
int AestheticValue( int , int );


int main(int argc, char ** argv) 
{
	int TrainingImages = NUMBER_OF_TRAINING_SAMPLES;
	int TestingImages = NUMBER_OF_TESTING_SAMPLES;
	int choice = 0;
	cout << "----- Main Menu -----" << endl;
	cout << "Extract Features \n" 
		 << " '1' Aesthetic. \n"
		 << " '2' Recency. \n"
		 << " '3' Page Topic. \n";

	cout << "Enter a choice: ";
	cin >> choice;
	cout << '\n';
	
	switch ( choice )
	{
		case 1:
			AestheticValue(TrainingImages, TestingImages);
			break;
		case 2:
			WebPageRecency(TrainingImages, TestingImages);
			break;
		case 3:
			WebPageTopic(TrainingImages, TestingImages);
			break;
		default:
			cout << "You have entered an invalid choice.\n"
				 << "Please try again.\n\n";
			break;
	}
}

void InitTable()
{
	Features = (double*)malloc((NUMBER_OF_TOTAL_FEATURES)*sizeof(double*));
}

void ExtractFeatures( IplImage* imageRGB, const char* directory ) 
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
	ExtractTamuraFeat(imageGray , TamuraFeat);

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

IplImage* img_resize(IplImage* src_img, int new_width,int new_height)
{
    IplImage* des_img;
    des_img=cvCreateImage(cvSize(new_width,new_height),src_img->depth,src_img->nChannels);
    cvResize(src_img,des_img,CV_INTER_LINEAR);
    return des_img;
} 

void Extract( char* filePath, char* csvfile)
{
	int i, j;
	
	IplImage *imageRGB = NULL; 
	imageRGB = cvLoadImage(filePath, CV_LOAD_IMAGE_COLOR);
	if(!imageRGB){ 
		printf("Error loading image\n");
	}
	int width = 1024;
	int height = 1024;
	IplImage *newImage = NULL;
	newImage = img_resize(imageRGB, width, height);
	
/* 	cvNamedWindow("image", CV_WINDOW_AUTOSIZE);
	cvShowImage("image", newImage);
	cvWaitKey();
	cvDestroyWindow("image"); */
	
	/* FUNCTION TO EXTRACT FEATURES*/
	ExtractFeatures( newImage, csvfile );	
	cvReleaseImage(&newImage);
	return;
}

void ProcessImages(const char* filePath, char* csvfile, int nImages)
{
	int 	i;
	char 	directory[50];
	for( i = 1; i <= nImages; i++ )
	{
		sprintf(directory, "%s/%1d.png", filePath, i);
		printf(" Processing image %d in %s.		\n", i, directory);
		Extract( directory, csvfile);
	}
}

int WebPageTopic( int TrainingImages, int TestingImages )
{
	/*
		Topic:
			- Newspaper class    	0
			- Conference class	 	1
			- Celebrity class		2
			- Hotel class			3
	*/
	
	InitTable();
	char 	TrainingDir[50];
	char 	TestingDir[50];
	
	sprintf(TrainingDir, "TopicTraining-%1d-%1d.csv", TrainingImages, TestingImages);
	sprintf(TestingDir, "TopicTesting-%1d.csv", TestingImages);
	const char* filePath1 	= "WebPages/PageTopic/Newspapers";
	const char* filePath2 	= "WebPages/PageTopic/Conference";
	const char* filePath3 	= "WebPages/PageTopic/Celebrity";
	const char* filePath4 	= "WebPages/PageTopic/Hotel";
	const char* filePath5 	= "WebPages/PageTopic/ClassifiedSites";
	const char* filePath6 	= "WebPages/PageTopic/Gaming";
	const char* filePath7 	= "WebPages/PageTopic/SocialNetworks";
	const char* filePath8 	= "WebPages/PageTopic/Video-Sharing";

	const char* filePath9 	= "WebPages/PageTopic/testing8";
	const char* filePath10 	= "WebPages/PageTopic/Testing1";

	const char* csvname 	= "PageTopic";

	ProcessImages(filePath1, TrainingDir, TrainingImages);
	ProcessImages(filePath2, TrainingDir, TrainingImages);
	ProcessImages(filePath3, TrainingDir, TrainingImages);
	ProcessImages(filePath4, TrainingDir, TrainingImages);
	ProcessImages(filePath5, TrainingDir, TrainingImages);
	ProcessImages(filePath6, TrainingDir, TrainingImages);
	ProcessImages(filePath7, TrainingDir, TrainingImages);
	ProcessImages(filePath8, TrainingDir, TrainingImages);
	ProcessImages(filePath9, TestingDir, TestingImages);
	//ProcessImages(filePath10, TestingDir, TestingImages);

}

int WebPageRecency( int TrainingImages, int TestingImages )
{
	/*
		Recency:
			- New Webpages
			- Old WebPages	
	*/

	InitTable();
	char 	TrainingDir[50];
	char 	TestingDir[50];
	sprintf(TrainingDir, "RecencyTraining-%1d-%1d.csv", TrainingImages, TestingImages);
	sprintf(TestingDir, "RecencyTesting-%1d-%1d.csv", TrainingImages, TestingImages);
	const char* filePath1 	= "WebPages/Recency/Old";
	const char* filePath2 	= "WebPages/Recency/New";
	const char* filePath3 	= "WebPages/Recency/Testing10";
	const char* csvname 	= "Recency";
	
	ProcessImages(filePath1, TrainingDir, TrainingImages);
	ProcessImages(filePath2, TrainingDir, TrainingImages);
	ProcessImages(filePath3, TestingDir, TestingImages);
}

int AestheticValue( int TrainingImages, int TestingImages )
{
	
	/*
		Aesthetic Value:
			- Ugly Webpages
			- Beautiful WebPages	
	*/
	InitTable();
	char 	TrainingDir[50];
	char 	TestingDir[50];
	sprintf(TrainingDir, "AestheticTraining-%1d-%1d.csv", TrainingImages, TestingImages);
	sprintf(TestingDir, "AestheticTesting-%1d-%1d.csv", TrainingImages, TestingImages);
	
	const char* filePath1 	= "WebPages/Aesthetic/Ugly";
	const char* filePath2 	= "WebPages/Aesthetic/Beautiful";
	const char* filePath3 	= "WebPages/Aesthetic/Testing";
	const char* csvname 	= "Aesthetic";	
	ProcessImages( filePath1, TrainingDir, TrainingImages);
	ProcessImages( filePath2, TrainingDir, TrainingImages);
	ProcessImages( filePath3, TestingDir, TestingImages); 
}
