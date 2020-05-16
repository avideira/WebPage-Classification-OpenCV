/*
 	Author: António Videira
*/
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
using namespace cv;
#define NUMBER_OF_TOTAL_FEATURES 	166 
#define NUMBER_OF_SELECTED_FEATURES 20 	
#define NUMBER_OF_TRAINING_SAMPLES 	90 
#define NUMBER_OF_TESTING_SAMPLES 	20

static int j = 0;

float 			**Training_Features;
float 			**Testing_Features;
static int 		position[NUMBER_OF_SELECTED_FEATURES];

void **CreateTable(int TrainingSize, int TestingSize)
{
	int i;
	Training_Features = (float**)malloc((TrainingSize)*sizeof(float*));
	Testing_Features  = (float**)malloc((TestingSize)*sizeof(float*));
	for ( i = 0; i < TrainingSize ; i++)
	{
		Training_Features[i] = (float *)malloc (NUMBER_OF_TOTAL_FEATURES * sizeof(float));
	}
	for ( i = 0; i < TestingSize ; i++)
	{
		Testing_Features[i] = (float *)malloc (NUMBER_OF_TOTAL_FEATURES * sizeof(float));
	}
	
}

/* void ExtractFeatures( IplImage* imageRGB, char type ) 
{
	int i;
	double 		RGBdescriptor[Colorbins] 	= {0};
	double 		EdgeH[Edgebins] 			= {0};
	double 		TamuraFeat[tamuraF]			= {0};
	double 		GaborFeat[gaborF] 			= {0};
	
	if(type == 'c')	printf(" Extracting Training Features. \n");
	else if(type == 'k') printf(" Extracting Testing Features. \n");
	
	IplImage *imageGray = NULL;
	imageGray 	= 	cvCreateImage(cvGetSize(imageRGB), 8, 1);
	cvCvtColor(imageRGB, imageGray, CV_BGR2GRAY );
	
 	ExtractColorHist(imageRGB , RGBdescriptor);

	ExtractEdgeHist(EdgeH , imageGray);

	ExtractTamuraFeat(imageGray , TamuraFeat);

	ExtractGaborFeatures(imageGray, GaborFeat);

	
	for(int i=0; i<NUMBER_OF_TOTAL_FEATURES; i++){
	
		if(i<32 && type == 'c'){	
			Training_Features[j][i] = RGBdescriptor[i];
		}else if(i<32 && type == 'k'){
			Testing_Features[j][i] = RGBdescriptor[i];
		}
		if(i>=32 && i <112 && type == 'c'){	
			Training_Features[j][i] = EdgeH[i-32];
		}else if(i>=32 && i <112 && type == 'k'){ 
			Testing_Features[j][i] = EdgeH[i-32];
		}
		if(i>=112 && i <130 && type == 'c'){		
			Training_Features[j][i] = TamuraFeat[i-112];
		}else if(i>=112 && i <130 && type == 'k'){	
			Testing_Features[j][i] = TamuraFeat[i-112];
		}
		if(i>=130 && i <166 && type == 'c'){			
			Training_Features[j][i] = GaborFeat[i-130];
		}else if(i>=130 && i <166 && type == 'k'){
			Testing_Features[j][i] = GaborFeat[i-130];
		}
	}
	
	j++;
	cvReleaseImage(&imageGray);
	return;
} */

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

int WriteinCSV( double SelectedFeatures[], const char* directory, int totalfeats )
{
	/* 	
		Save the variables that characterize the image in a csv file, separable by a 
		comma FLAG (single value) - regards to the type of the class of each image.
	*/
	int 		i = 0;
	ofstream 	myfile;
	
	myfile.open (directory, ios::app);
	
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
			
int WriteinCSV(char caracter, int flag, double SelectedFeatures[], const char* directory)
{
	/* 	
		Save the variables that characterize the image in a csv file, separable by a 
		comma FLAG (single value) - regards to the type of the class of each image.
	*/
	int 		i = 0;
	ofstream 	myfile;

	myfile.open (directory, ios::app);
	myfile << flag << ",";

	
	while(i < (NUMBER_OF_SELECTED_FEATURES-1))
	{
		myfile << SelectedFeatures[i] << ",";
		i++;
	}
	myfile << SelectedFeatures[i] << endl;
	myfile.close();
	fflush(NULL);
	return 0; 
}

int read_features_from_csv(const char* filename, char caracter)
{
    string line;
    int linenum = 0;
	float value;
	while(1) {
	ifstream inFile (filename);
	
    while (getline (inFile, line))
    {  
        //cout << "\nLine #" << linenum << ":" << endl;
        istringstream linestream(line);
        string item;
        int itemnum = 0;
        while (getline (linestream, item, ','))
        {
			istringstream buffer(item);
			buffer >> value;
			
			if(caracter == 'c')	Training_Features[linenum][itemnum] = value;
			else if(caracter == 'k')Testing_Features[linenum][itemnum] = value;
			
			itemnum++;
        }
		linenum++;
    }
	inFile.close();	
	return 0;
	}
	
	
	//fflush(NULL);
	return 1;
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
               if (a[c] > a[c+1])
               {
                    temp = a[c+1];
                    a[c+1] = a[c];
                    a[c] = temp;
               }
          }
     }

}

void BubbleSort(double a[], int array_size)
{
     int i, c;
	 float temp;
	 int p;
     for (i = 0; i < (array_size - 1); i++)
     {
          for (c = 0; c < array_size - 1 - i; c++ )
          {
               if (a[c] > a[c+1])
               {
                    temp = a[c+1];
                    a[c+1] = a[c];
                    a[c] = temp;
               }
          }
     }

}

void doPCA( int dim_reduction, int NumberOFSamples, int NUMBER_OF_CLASSES, const char* csvname, char caracter )
{
	printf(" Applying PCA Reduction. \n");
	int i, h, z;
	int NumberOfSets, size, CheckClass;
	char directory[50];
	double 	SelectedFeat[dim_reduction];
	memset(SelectedFeat, 0, dim_reduction*sizeof(double));
	
	if(caracter == 'c') //Training data
	{
		NumberOfSets 	= 	NumberOFSamples;
		size = NUMBER_OF_TOTAL_FEATURES;
		Mat features = Mat(NumberOfSets, size, CV_32FC1);
		for( i = 0; i < NumberOfSets; i++ ) {
			for( h = 0; h < size; h++ ) {
				features.at<float>(i,h) = (float)Training_Features[i][h];
				//cout << features.at<float>(i,h) << "  ";
			}
			//cout << endl;
		} 
		//cout << endl;
		Mat cov, mu;
		calcCovarMatrix(features, cov, mu, CV_COVAR_NORMAL | CV_COVAR_ROWS);
		SVD svdA(cov, SVD::FULL_UV);
		float EigenValtemp[size];
		cout << "Calculating EigenValues of Training Features. " << endl;
		for( h = 0; h < size; h++ ){
				EigenValtemp[h] = svdA.w.at<float>(h);
		}
		//cout << endl << endl;
		cout << "Number of Eigenvalues = " << h << endl;
		
		sort(EigenValtemp, size); // Sort the singular values in descendent order.
 		for( h = 0; h < size; h++ ){
				cout <<"value " << h << " = " << EigenValtemp[h] << "  -  " << svdA.w.at<float>(h) << "  " << endl;
		} 
		cout << endl;
		
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
						//z++;
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
		
		sprintf(directory, "%s-%d-%d-Training.csv", csvname, NUMBER_OF_TRAINING_SAMPLES, NUMBER_OF_SELECTED_FEATURES);
		
		CheckClass 	= 	NumberOfSets/NUMBER_OF_CLASSES;
		for(i = 0; i < NumberOfSets; i++){
			for( h = 0; h < dim_reduction; h++){
				SelectedFeat[h] = FinalData.at<float>(i,h);
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
		j = 0;
		free(Training_Features[0]);
		free(Training_Features);
	}
	else if(caracter == 'k')
	{	
		int pos;
		NumberOfSets 	= 	NumberOFSamples;
		CheckClass 		= 	NumberOfSets/NUMBER_OF_CLASSES;
		size 	= 	NUMBER_OF_TOTAL_FEATURES;
		sprintf(directory, "%s-%d-%d-Testing.csv", csvname, NUMBER_OF_TRAINING_SAMPLES, NUMBER_OF_SELECTED_FEATURES);
		for(i = 0; i < NumberOfSets; i++)
		{
			for(h = 0; h < dim_reduction; h++)
			{
				pos = position[h];
				SelectedFeat[h] = Testing_Features[i][pos];
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
		free(Testing_Features[0]);
		free(Testing_Features);
	}
	
}

void ChiSquare( char caracter, const char* csvname, int NUMBER_OF_CLASSES )
{
	int 	i,k,h,z;
	int 	size;
	double 	ExpectedFreq = 0;
	int 	NumberOfSets;
	int 	NumberOfFeats = NUMBER_OF_TOTAL_FEATURES;
	
	if(caracter == 'c') //Training data
	{ 
		ExpectedFreq 	= 	NUMBER_OF_TRAINING_SAMPLES;
		NumberOfSets 	= 	NUMBER_OF_TRAINING_SAMPLES*NUMBER_OF_CLASSES;
		printf("\n Creating	Training Samples.\n"
				" Number Of Sets  =  %d.\n", 	NumberOfSets);	
	}
	else if(caracter == 'k') //Testing data
	{ 
		NumberOfSets 	= 	NUMBER_OF_TESTING_SAMPLES;
		printf("\n Creating	Testing Samples.\n"
				" Number Of Sets  =  %d.\n",	NumberOfSets);
	}
	
	if(caracter == 'c') //Training data
	{
		size 		= 	NumberOfSets/NUMBER_OF_CLASSES;
		
		double 	mean[NUMBER_OF_CLASSES][NumberOfFeats];
		memset(mean, 0, NUMBER_OF_CLASSES*NumberOfFeats*sizeof(double));
		double 	val[NUMBER_OF_CLASSES-1][NumberOfFeats];
		memset(val, 0, (NUMBER_OF_CLASSES-1)*NumberOfFeats*sizeof(double));
		
		int p = 0;
	
		for( i = 0; i<NumberOfFeats; i++){
			for( k = 0; k<NumberOfSets ; k++){
					if( k < size )
					{
						mean[p][i] += Training_Features[k][i];
					}
					else if( (k >= size) && (k < size*2) )
					{
						mean[p+1][i] += Training_Features[k][i];
					}
					else if( (k >= size*2) && (k < size*3) )
					{
						mean[p+2][i] += Training_Features[k][i];
					}
					else if( (k >= size*3) )
					{
						mean[p+3][i] += Training_Features[k][i];
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
			sizeSup = sizeInf + NUMBER_OF_TRAINING_SAMPLES;
			for( i = 0; i < NumberOfSets; i++ )
			{
				if( (i < sizeInf))
				{
					if( (Training_Features[i][k] <= val[z][k]) )
					{
							ObservedFreq[p] += 1;
					}
				}
				else if( (i > sizeInf) && (i < sizeSup) )
				{
					if( (Training_Features[i][k] > val[z][k]) && (Training_Features[i][k] <= val[z+1][k]) )
					{
						ObservedFreq[p+1] += 1;
					}					
				}
				else if( ( i > (sizeInf + NUMBER_OF_TRAINING_SAMPLES) ) && ( i < (sizeSup + NUMBER_OF_TRAINING_SAMPLES)) )
				{
					if( (Training_Features[i][k] > val[z+1][k]) && (Training_Features[i][k] <= val[z+2][k]) )
					{
						ObservedFreq[p+2] += 1;
					}
				}
				else if(  i > ( sizeSup + NUMBER_OF_TRAINING_SAMPLES + NUMBER_OF_TRAINING_SAMPLES) )
				{
					if( (Training_Features[i][k] > val[z+2][k]) )
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
		
		/* for(k = 0; k < NumberOfFeats; k++){
				printf ("Chi-Square value %d : %f      .  %f\n " , k, Chi2[k], Chi2temp[k]);
		} */
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
		sprintf(directory, "%s-%d-%d-Training.csv", csvname,NUMBER_OF_TRAINING_SAMPLES, NUMBER_OF_SELECTED_FEATURES);
		for(k = 0; k < NumberOfSets; k++)
		{
			for(h = 0; h < NUMBER_OF_SELECTED_FEATURES; h++)
			{
				pos = position[h];
				SelectedFeat[h] = Training_Features[k][pos];
			}
			if( k < size )
			{
				WriteinCSV(caracter, 0, SelectedFeat, directory);
			}
			else if( (k >= size) && (k < size*2) )
			{
				WriteinCSV(caracter, 1, SelectedFeat, directory);
			}
			else if( (k >= size*2) && (k < size*3) )
			{
				WriteinCSV(caracter, 2, SelectedFeat, directory);
			}
			else if( (k >= size*3) )
			{
				WriteinCSV(caracter, 3, SelectedFeat, directory);
			}
			/*if(k < NumberOfSets/2) 	WriteinCSV(caracter, 0, SelectedFeat, directory);
			else 					WriteinCSV(caracter, 1, SelectedFeat, directory); */
		}
		free(Training_Features[0]);
		free(Training_Features);
	}
	
	else if(caracter == 'k')
	{
		size 	= 	NumberOfSets/NUMBER_OF_CLASSES;
		sprintf(directory, "%s-%d-%d-Testing.csv", csvname, NUMBER_OF_TRAINING_SAMPLES, NUMBER_OF_SELECTED_FEATURES);
		for(k = 0; k < NumberOfSets; k++)
		{
			for(h = 0; h < NUMBER_OF_SELECTED_FEATURES; h++)
			{
				pos = position[h];
				SelectedFeat[h] = Testing_Features[k][pos];
			}
			if( k < size )
			{
				WriteinCSV(caracter, 0, SelectedFeat, directory);
			}
			else if( (k >= size) && (k < size*2) )
			{
				WriteinCSV(caracter, 1, SelectedFeat, directory);
			}
			else if( (k >= size*2) && (k < size*3) )
			{
				WriteinCSV(caracter, 2, SelectedFeat, directory);
			}
			else if( (k >= size*3) )
			{
				WriteinCSV(caracter, 3, SelectedFeat, directory);
			}
		}
		free(Testing_Features[0]);
		free(Testing_Features);
	}
	j = 0;
}
		
