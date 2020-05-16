#include "cvGabor.h"
#include "cvGabor.cpp"
#include <cv.h>
#include <highgui.h>
#include <opencv2/core/core.hpp>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <vector>
#include <string>     
#include "LBPGist.h"

#define ImageGrid 16

using namespace std;
using namespace cv;

float 	**Training_Gist;
float 	**Testing_Gist;
//float **FinalData;
static int	*position;
//static int position[NUMBER_OF_SELECTED_FEATURES];
static int idx = 0;

void ChiSquare( int dim_reduction, int NumberOFSamples, int scales, int orientations, int NUMBER_OF_CLASSES, const char* csvname, char caracter )
{
	int 	i,k,h,z;
	int 	size;
	double 	ExpectedFreq;
	int 	NumberOfSets;
	int 	NumberOfFeats = scales*orientations*ImageGrid;
	int 	NUMBER_OF_SELECTED_FEATURES = dim_reduction;

	if(caracter == 'c') //Training data
	{
		ExpectedFreq 	= 	NumberOFSamples/NUMBER_OF_CLASSES;
		NumberOfSets 	= 	NumberOFSamples;
		printf("\n Creating	Training Samples.\n"
				" Number Of Samples  =  %d.\n", 	NumberOfSets);	
	
		size 		= 	NumberOfSets/NUMBER_OF_CLASSES;
		
		double 	mean[NUMBER_OF_CLASSES][NumberOfFeats];
		memset(mean, 0, NUMBER_OF_CLASSES*NumberOfFeats*sizeof(double));
		double 	val[NUMBER_OF_CLASSES-1][NumberOfFeats];
		memset(val, 0, (NUMBER_OF_CLASSES-1)*NumberOfFeats*sizeof(double));
		
		int p = 0;
	
		for( i = 0; i < NumberOfFeats; i++ ){
			for( k = 0; k < NumberOfSets ; k++ ){
					if( k < size )
					{
						mean[p][i] += Training_Gist[k][i];
					}
					else if( (k >= size) && (k < size*2) )
					{
						mean[p+1][i] += Training_Gist[k][i];
					}
					else if( (k >= size*2) && (k < size*3) )
					{
						mean[p+2][i] += Training_Gist[k][i];
					}
					else if( (k >= size*3) )
					{
						mean[p+3][i] += Training_Gist[k][i];
					}
			}

			for( h = 0; h < NUMBER_OF_CLASSES; h++ )
			{
				mean[h][i] 	/= 	ExpectedFreq;
				//printf ( "mean %d,%d = %f \n" , h, i ,mean[h][i]);
			}
			
			for( z = 0; z < NUMBER_OF_CLASSES-1; z++ )
			{
				val[z][i]  	= mean[z][i] + mean[z+1][i];
				val[z][i] 	= val[z][i]/2;
				//printf ( "val %d = %f \n" , z ,val[z][i]);
			}
		}
		
		float 	Chi2[NumberOfFeats]; //X^2
		float 	Chi2temp[NumberOfFeats]; 
		memset(Chi2, 0, NumberOfFeats*sizeof(float));
		memset(Chi2temp, 0, NumberOfFeats*sizeof(float));
		int sizeInf, sizeSup;
	
		for(k = 0; k < NumberOfFeats; k++){
	
			p = 0;
			z = 0;
			
			double temp[NUMBER_OF_CLASSES];
			double ObservedFreq[NUMBER_OF_CLASSES];
			memset(temp, 0, NUMBER_OF_CLASSES*sizeof(double));
			memset(ObservedFreq, 0, NUMBER_OF_CLASSES*sizeof(double));
		
			
			sizeInf = NumberOfSets/NUMBER_OF_CLASSES;
			sizeSup = sizeInf + NumberOfSets;
			for( i = 0; i < NumberOfSets; i++ )
			{
				if( (i < sizeInf))
				{
					if( (Training_Gist[i][k] <= val[z][k]) )
					{
							ObservedFreq[p] += 1;
					}
				}
				else if( (i > sizeInf) && (i < sizeSup) )
				{
					if( (Training_Gist[i][k] > val[z][k]) && (Training_Gist[i][k] <= val[z+1][k]) )
					{
						ObservedFreq[p+1] += 1;
					}					
				}
				else if( ( i > (sizeInf + NumberOfSets) ) && ( i < (sizeSup + NumberOfSets)) )
				{
					if( (Training_Gist[i][k] > val[z+1][k]) && (Training_Gist[i][k] <= val[z+2][k]) )
					{
						ObservedFreq[p+2] += 1;
					}
				}
				else if(  i > ( sizeSup + NumberOfSets + NumberOfSets) )
				{
					if( (Training_Gist[i][k] > val[z+2][k]) )
					{
						ObservedFreq[p+3] += 1;
					}
				}
			}
			
			for( h = 0; h < NUMBER_OF_CLASSES; h++ )
			{
				temp[h] = ObservedFreq[h] - ExpectedFreq;
				temp[h] = pow(temp[h],2);
				Chi2[k] += temp[h];
			}
		
			Chi2[k] = Chi2[k]/NumberOfSets;
			Chi2temp[k] = Chi2[k];
			//printf ("Chi-Square value %d = %f \n " , k, Chi2[k]);
		}
		BubbleSort(Chi2, NumberOfFeats);
		
/* 		for(k = 0; k < NumberOfFeats; k++){
				printf ("Chi-Square value %d : %f      .  %f\n " , k, Chi2[k], Chi2temp[k]);
		} */
		position = (int*)malloc((NUMBER_OF_SELECTED_FEATURES)*sizeof(int*));
		vector<int> lastpos;
		
		for( h = 0; h < NUMBER_OF_SELECTED_FEATURES; h++ )
		{
			for( z = 0; z < NumberOfFeats; z++ )
			{
				if( fabs(Chi2[h] - Chi2temp[z]) == 0)
				{
					if(lastpos.empty()){
						lastpos.push_back(z);
						position[h] = z;
						z = NumberOfFeats;
					}else if(find(lastpos.begin(), lastpos.end(), z)!= lastpos.end()){
						z++;
					}else{
						lastpos.push_back(z);
						position[h] = z;
						z = NumberOfFeats;					
						//EigenValtemp[z] = 0;
					}
				}
			}
			cout << "Position " << h << " = " << position[h] << endl;
		}
	}
	
	char 	directory[50];
	int 	pos;
	double 	SelectedFeat[NUMBER_OF_SELECTED_FEATURES];
	memset(SelectedFeat, 0, NUMBER_OF_SELECTED_FEATURES*sizeof(double));
	
	if (caracter == 'c')
	{
		size 	= 	NumberOfSets/NUMBER_OF_CLASSES;
		sprintf(directory, "%sTraining.csv", csvname);
		for(k = 0; k < NumberOfSets; k++)
		{
			for(h = 0; h < NUMBER_OF_SELECTED_FEATURES; h++)
			{
				pos = position[h];
				SelectedFeat[h] = Training_Gist[k][pos];
			}
			if( k < size )
			{
				WriteinCSV(SelectedFeat, 0, directory, NUMBER_OF_SELECTED_FEATURES);
			}
			else if( (k >= size) && (k < size*2) )
			{
				WriteinCSV(SelectedFeat, 1, directory, NUMBER_OF_SELECTED_FEATURES);
			}
			else if( (k >= size*2) && (k < size*3) )
			{
				WriteinCSV(SelectedFeat, 2, directory, NUMBER_OF_SELECTED_FEATURES);
			}
			else if( (k >= size*3) )
			{
				WriteinCSV(SelectedFeat, 3, directory, NUMBER_OF_SELECTED_FEATURES);
			}
			/*if(k < NumberOfSets/2) 	WriteinCSV(caracter, 0, SelectedFeat, directory);
			else 					WriteinCSV(caracter, 1, SelectedFeat, directory); */
		}
		free(Training_Gist[0]);
		free(Training_Gist);
	}
	
	else if(caracter == 'k')
	{
		ExpectedFreq 	= 	NumberOFSamples/NUMBER_OF_CLASSES;
		NumberOfSets 	= 	NumberOFSamples;
		printf("\n Creating	Testing Samples.\n"
				" Number Of Samples  =  %d.\n",	NumberOfSets);
				
		size 	= 	NumberOfSets/NUMBER_OF_CLASSES;
		sprintf(directory, "%sTesting.csv", csvname);
		for(k = 0; k < NumberOfSets; k++)
		{
			for(h = 0; h < NUMBER_OF_SELECTED_FEATURES; h++)
			{
				pos = position[h];
				SelectedFeat[h] = Testing_Gist[k][pos];
			}
			if( k < size )
			{
				WriteinCSV(SelectedFeat, 0, directory, NUMBER_OF_SELECTED_FEATURES);
			}
			else if( (k >= size) && (k < size*2) )
			{
				WriteinCSV(SelectedFeat, 1, directory, NUMBER_OF_SELECTED_FEATURES);
			}
			else if( (k >= size*2) && (k < size*3) )
			{
				WriteinCSV(SelectedFeat, 2, directory, NUMBER_OF_SELECTED_FEATURES);
			}
			else if( (k >= size*3) )
			{
				WriteinCSV(SelectedFeat, 3, directory, NUMBER_OF_SELECTED_FEATURES);
			}
			/* if(k < NumberOfSets/2) 	WriteinCSV(caracter, 0, SelectedFeat, directory);
			else 					WriteinCSV(caracter, 1, SelectedFeat, directory); */
		}
		free(position);
		free(Testing_Gist[0]);
		free(Testing_Gist);
	}
	idx = 0;
}

void doPCA( int dim_reduction, int NumberOFSamples, int scales, int orientations, int NUMBER_OF_CLASSES, const char* csvname, char caracter )
{
	printf(" Applying PCA Reduction. \n");
	int i, j, h, z;
	int NumberOfSets, size, CheckClass;
	char directory[50];
	float 	SelectedFeat[dim_reduction];
	memset(SelectedFeat, 0, dim_reduction*sizeof(float));
	
	if(caracter == 'c') //Training data
	{
		NumberOfSets 	= 	NumberOFSamples;
		size = scales*orientations*ImageGrid;
		Mat features = Mat(NumberOfSets, size, CV_32FC1);
		for( i = 0; i < NumberOfSets; i++ ) {
			for( j = 0; j < size; j++ ) {
				features.at<float>(i,j) = Training_Gist[i][j];
				//cout << features.at<float>(i,j) << "  ";
			}
			//cout << endl;
		} 
		//cout << endl;
		Mat cov, mu;
		calcCovarMatrix(features, cov, mu, CV_COVAR_NORMAL | CV_COVAR_ROWS);
		
		SVD svdA(cov, SVD::FULL_UV);
		float EigenValtemp[size];
		
		cout << "Calculating EigenValues of Training Features. " << endl;
		
		for( j = 0; j < size; j++ ){
				EigenValtemp[j] = svdA.w.at<float>(j);
				cout << svdA.w.at<float>(j) << "  " ;
		}
		cout << "Number of Eigenvalues = " << j << endl;
		
		sort(EigenValtemp, size); // Sort the singular values in descendent order.
 /* 		for( j = 0; j < size; j++ ){
			cout << "Value " << j << " = " << svdA.w.at<float>(j) << " - "  << EigenValtemp[j] << endl;
		}
		cout << endl << endl;
		 */
		position = (int*)malloc((dim_reduction)*sizeof(int*));

		vector<int> lastpos;
		
		for( h = 0; h < dim_reduction; h++)
		{
			for(z = 0; z < size; z++)
			{
				if(fabs( EigenValtemp[h] - svdA.w.at<float>(z) )  == 0)
				{
					if(lastpos.empty()){
						lastpos.push_back(z);
						position[h] = z;
						z = size;
					}else if(find(lastpos.begin(), lastpos.end(), z)!= lastpos.end()){
						z++;
					}else{
						lastpos.push_back(z);
						position[h] = z;
						z = size;					
						//EigenValtemp[z] = 0;
					}
				}
			}
			cout << "Position " << h << " = " << position[h] << endl;
		}
		
		Mat FinalData = Mat(NumberOfSets, dim_reduction, CV_32FC1);
		for(i = 0; i< dim_reduction; i++){
			features.col(position[i]).copyTo(FinalData.col(i));
		}
		
		sprintf(directory, "%sTraining.csv", csvname);
		
		CheckClass 	= 	NumberOfSets/NUMBER_OF_CLASSES;
		for(i = 0; i < NumberOfSets; i++){
			for( j = 0; j < dim_reduction; j++){
				SelectedFeat[j] = FinalData.at<float>(i,j);
			}
			if( i < CheckClass )
			{
				WriteinCSV(SelectedFeat, 0, directory, dim_reduction);
			}
			else if( (i >= CheckClass) && (i < CheckClass*2) )
			{
				WriteinCSV(SelectedFeat, 1, directory, dim_reduction);
			}
			else if( (i >= CheckClass*2) && (i < CheckClass*3) )
			{
				WriteinCSV(SelectedFeat, 2, directory, dim_reduction);
			}
			else if( (i >= CheckClass*3) )
			{
				WriteinCSV(SelectedFeat, 3, directory, dim_reduction);
			}
		}
		free(Training_Gist[0]);
		free(Training_Gist);
	}
	else if(caracter == 'k')
	{	
		int pos;
		NumberOfSets 	= 	NumberOFSamples;
		CheckClass 		= 	NumberOfSets/NUMBER_OF_CLASSES;
		size 	= 	scales*orientations*ImageGrid;
		sprintf(directory, "%sTesting.csv", csvname);
		for(i = 0; i < NumberOfSets; i++)
		{
			for(j = 0; j < dim_reduction; j++)
			{
				pos = position[j];
				SelectedFeat[j] = Testing_Gist[i][pos];
			}
			if( i < CheckClass )
			{
				WriteinCSV(SelectedFeat, 0, directory, dim_reduction);
			}
			else if( (i >= CheckClass) && (i < CheckClass*2) )
			{
				WriteinCSV(SelectedFeat, 1, directory, dim_reduction);
			}
			else if( (i >= CheckClass*2) && (i < CheckClass*3) )
			{
				WriteinCSV(SelectedFeat, 2, directory, dim_reduction);
			}
			else if( (i >= CheckClass*3) )
			{
				WriteinCSV(SelectedFeat, 3, directory, dim_reduction);
			}
			/* if(k < NumberOfSets/2) 	WriteinCSV(caracter, 0, SelectedFeat, directory);
			else 					WriteinCSV(caracter, 1, SelectedFeat, directory); */
		}
		free(Testing_Gist[0]);
		free(Testing_Gist);
	}
	idx = 0;
}

void **CreateTable(int TrainingSize, int TestingSize, int scales, int orientations)
{
	int i;
	int size = scales*orientations*ImageGrid;

	Training_Gist 	= (float**)malloc((TrainingSize)*sizeof(float*));
	Testing_Gist 	= (float**)malloc((TestingSize)*sizeof(float*));
	
	for ( i = 0; i < TrainingSize ; i++)
		Training_Gist[i] = (float *)malloc (size * sizeof(float));
	
	for ( i = 0; i < TestingSize ; i++)
		Testing_Gist[i] = (float *)malloc (size * sizeof(float));		
}

void ExtractGISTFeatures( IplImage *image, int scales, int orientations, char type)
{
	if(type == 'c')	printf(" Extracting Gist Training Features. \n");
	else if(type == 'k') printf(" Extracting Gist Testing Features. \n");
	int i,j,k,z;
	double Sigma = 2*PI;  
	double F = sqrt(2.0);  
	double norm = 0.0;
	CvGabor *gabor1 = new CvGabor;
	int pos = 0;
	for( i = 0; i < scales;  i++){ //scales
		for( j = 0; j < orientations; j++){ //orientations
		
			double dPhi = (PI/(j+1));
			int iNu = (1 + i);
			
			gabor1->Init( dPhi , iNu, Sigma, F );

			IplImage *kernel = cvCreateImage( cvSize(gabor1->get_mask_width(), gabor1->get_mask_width()), IPL_DEPTH_8U, 1);
			kernel = gabor1->get_image(CV_GABOR_REAL);
 			/* cvNamedWindow("Gabor Kernel", CV_WINDOW_AUTOSIZE);
			cvShowImage("Gabor Kernel", kernel);
			cvWaitKey(0);
	 */
			IplImage *reimg = cvCreateImage(cvSize(image->width,image->height), IPL_DEPTH_8U, 1);
			
			gabor1->conv_img(image, reimg, CV_GABOR_MAG);
			
 		/* 	cvNamedWindow("Magnitude Response", CV_WINDOW_AUTOSIZE);
			cvShowImage("Magnitude Response",reimg);
			cvWaitKey(0);
			 */
			//4X4 GRID
			//printf( "height = %d, width = %d\n", reimg->height, reimg->width );
			int hinc = (int)(reimg->height/4);
			int winc = (int)(reimg->width/4);
			//printf( "height inc = %d, width inc = %d\n", hinc, winc);
			
			CvScalar valor;
			
			for( k = 0; k < reimg->height-3;  k += hinc)	 // For each scale and orientation for each 
			{    
				for( z = 0; z < reimg->width-3; z += winc)  // sub window of a 4x4 grid the avg value is computed.	
				{ 
					//printf( "height = %d, width = %d\n", k, z);
					valor = GetAvgImage(reimg, k, z, hinc, winc );
					if(type == 'c'){ 
						Training_Gist[idx][pos] = (float) valor.val[0];
						norm += Training_Gist[idx][pos]*Training_Gist[idx][pos];
					}
					else if(type == 'k'){ 
						Testing_Gist[idx][pos] = (float) valor.val[0];
						norm += Testing_Gist[idx][pos]*Testing_Gist[idx][pos];
					}
					++pos;
				}
			}
			// cvDestroyWindow("Gabor Kernel");
			// cvDestroyWindow("Magnitude Response");
			cvReleaseImage(&reimg);
		}
	}
	//normalize vector - values between 0 and 1.
	norm = (float)sqrt(norm);
	
/* 	for( i = 0; i < pos; i++) {
		if(type == 'c') 		printf(" %f , " , Training_Gist[idx][i]);
		else if(type == 'k')	printf(" %f , " , Testing_Gist[idx][i]);
	}
	printf("\n"); */
	for( i = 0; i < pos; i++) {
		if(type == 'c') Training_Gist[idx][i] = ((Training_Gist[idx][i])/norm);
		else if(type == 'k') Testing_Gist[idx][i] = ((Testing_Gist[idx][i])/norm);
	} 
	++idx;
}

CvScalar GetAvgImage(IplImage* source, int x, int y, int heightincr ,int widthincr) 
{
	CvRect old_roi = cvGetImageROI(source);
	cvSetImageROI(source, cvRect(y, x, (widthincr),(heightincr)) );
	/* cvNamedWindow("ROI Image", CV_WINDOW_AUTOSIZE);
	cvShowImage("ROI Image", source);
	cvWaitKey(0);
	cvDestroyWindow("ROI Image"); */
	CvScalar c = cvAvg(source);
	cvSetImageROI(source,old_roi); // reset old roi
	return c;
}

int WriteinCSV( double SelectedFeatures[], int flag, const char* directory, int totalfeats )
{
	/* 	
		Save the variables that characterize the image in a csv file, separable by a 
		comma FLAG (single value) - regards to the type of the class of each image.
	*/
	int 		i = 0;
	ofstream 	myfile;
	
	myfile.open (directory, ios::app);
	myfile << flag << ",";
	
	while(i < (totalfeats-1))
	{
		myfile << SelectedFeatures[i] << ",";
		i++;
	}
	myfile << SelectedFeatures[i] << endl;
	myfile.close();
	fflush(NULL);
	return 0; 
}

int WriteinCSV( float SelectedFeatures[], int flag, const char* directory, int totalfeats )
{
	/* 	
		Save the variables that characterize the image in a csv file, separable by a 
		comma FLAG (single value) - regards to the type of the class of each image.
	*/
	int 		i = 0;
	ofstream 	myfile;
	
	myfile.open (directory, ios::app);
	myfile << flag << ",";
	
	while(i < (totalfeats-1))
	{
		myfile << SelectedFeatures[i] << ",";
		i++;
	}
	myfile << SelectedFeatures[i] << endl;
	myfile.close();
	fflush(NULL);
	return 0; 
}

void BubbleSort(float a[], int array_size)
{
     int i, c;
	 float temp;
	 int p;
     for (i = 0; i < (array_size - 1); i++)
     {
          for (c = 0; c < array_size - 1 - i; c++ )
          {
               if (a[c] < a[c+1])
               {
                    temp = a[c+1];
                    a[c+1] = a[c];
                    a[c] = temp;
               }
          }
     }

}

int compare(float x, float y)
{
     return(x < y);
}

void swap(float *x, float *y)
{
     float temp;
     temp = *x;
     *x = *y;
     *y = temp;
}

void sort(float table[], const int n)
{
     for(int i = 0; i < n; i++)
     {
          for(int j = 0; j < n-1; j++)
          {
               if(compare(table[j], table[j+1]))
                    swap(&table[j], &table[j+1]);
          }
     }
}
