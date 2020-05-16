#include <cv.h>       // opencv general include file
#include <ml.h>		  // opencv machine learning include file
#include <highgui.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
using namespace std;
using namespace cv;

#define TRAINING_SAMPLES 			60	 
#define TESTING_SAMPLES 			80
#define NUMBER_OF_TOTAL_FEATURES 	166
#define BINARY_CLASS 				2
#define MULTI_CLASS 				8
#define CrossValBin					10
#define CrossValMulti				6

static int BinaryClass[2] = {0, 1};
static int MultiClass4[4]  = {0, 1, 2, 3};
static int MultiClass8[8]  = {0, 1, 2, 3, 4, 5, 6, 7};
static const int Colorbins 	= 32;	
static const int Edgebins 	= 80;	
static const int tamuraF 	= 18;	
static const int gaborF		= 36;
static double **ClassifiersAccuracy;

double 	NaiveBayesClassifier	( const char*, const char*, int, int*, int, int , int );
double 	SVMClassifier			( const char*, const char*, int, int*, int, int , int );			
double 	DecisionTreeClassifier	( const char*, const char*, int, int*, int, int , int );
double 	BoostClassifierBinary	( const char*, const char*, int, int*, int, int , int );	//Binary Classifier
double 	BoostClassifierMulti	( const char*, const char*, int, int*, int, int , int );

int 	AestheticValue			( int , int );
int 	WebPageRecency			( int , int );
int 	WebPageTopic			( int , int );
void 	**CreateTable			( int , int );

int 	WriteinCSV				( double[], int, const char*, int );
int 	read_data_from_csv		( const char*, Mat, Mat, int );
int 	read_features_from_csv	( const char*, Mat, char );
int 	read_features_from_csv  ( const char*, int, int, int );
int 	read_features_from_csv1	( const char*, int, int, int );

void 	sortAscending			(double[] , int );
void 	sortDecreasing			(double[] , int );
void 	ChiSquare				( char, const char*, int, int );
void 	ChiSquare8				( char, const char*, int, int );

void 	AllFeatures 			( int , int , const char* , char );
void 	SpecificFeatures 		( int , int , int , int , const char* , char );

double 	**Training_Features;
double 	**Testing_Features;

Mat training_data, testing_data;

int 	*position;

int main( int argc, char** argv )
{
	int i;
	int TrainingImages = TRAINING_SAMPLES;
	int TestingImages = TESTING_SAMPLES;
	int choice = 0;
	
	cout << "----- Main Menu -----" << endl;
	cout << "Classify Testing Pages \n" 
		 << " '1' to check the Page Aesthetic. \n"
		 << " '2' to check the Page Recency. \n"
		 << " '3' to check the Page Topic. \n";
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

void **CreateTable(int TrainingSize, int TestingSize)
{
	int i;
	Training_Features = (double**)malloc((TrainingSize)*sizeof(double*));
	Testing_Features  = (double**)malloc((TestingSize)*sizeof(double*));
	for ( i = 0; i < TrainingSize ; i++)
	{
		Training_Features[i] = (double *)malloc (NUMBER_OF_TOTAL_FEATURES * sizeof(double));
	}
	for ( i = 0; i < TestingSize ; i++)
	{
		Testing_Features[i] = (double *)malloc (NUMBER_OF_TOTAL_FEATURES * sizeof(double));
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
	int TrainingSize = TrainingImages*MULTI_CLASS;
	int TestingSize	 = TestingImages;
	
	int i;
	const char* Path = "Topic8-10Fold.csv";
	char training 	= 'c';
	char testing 	= 'k';
	const char* csvname   = "Topic";



	ClassifiersAccuracy = (double**)malloc((4)*sizeof(double*));
	for ( i = 0; i < 4 ; i++)
	{
		ClassifiersAccuracy[i] = (double *)malloc (CrossValMulti * sizeof(double));
	}
	
	int sampleTest,numSamples;
		
	for ( i = 0; i < CrossValMulti; i++) 
	{	
	
		cout << "iteration = " << i << endl;
		sampleTest = 10;
		numSamples = sampleTest*i;
		int NumberOfFeats = 166;
		int FeaturesToClassifier = NumberOfFeats;	
	
		CreateTable( TrainingSize, TestingSize );
	
		Mat training_data = Mat(TrainingSize, NUMBER_OF_TOTAL_FEATURES, CV_32FC1);
		Mat testing_data = Mat(TestingSize, NUMBER_OF_TOTAL_FEATURES, CV_32FC1);

		read_features_from_csv1( Path, TrainingImages, numSamples, sampleTest );

		
		AllFeatures ( TrainingSize, MULTI_CLASS, csvname, training );
		//SpecificFeatures ( Colorbins, 0, TrainingSize, MULTI_CLASS, csvname, training );
		//SpecificFeatures ( Edgebins, Colorbins, TrainingSize, MULTI_CLASS, csvname, training );
		//SpecificFeatures ( tamuraF, Colorbins+Edgebins, TrainingSize, MULTI_CLASS, csvname, training );
		//SpecificFeatures ( gaborF, Colorbins+Edgebins+tamuraF, TrainingSize, MULTI_CLASS, csvname, training );
		//ChiSquare(training, csvname, MULTI_CLASS, NumberOfFeats); 
		//ChiSquare8(training, csvname, MULTI_CLASS, NumberOfFeats); 
	
		AllFeatures ( TestingSize, MULTI_CLASS, csvname, testing );
		//SpecificFeatures ( Colorbins, 0, TestingSize, MULTI_CLASS, csvname, testing );
		//SpecificFeatures ( Edgebins, Colorbins, TestingSize, MULTI_CLASS, csvname, testing );
		//SpecificFeatures ( tamuraF, Colorbins+Edgebins, TestingSize, MULTI_CLASS, csvname, testing );
		//SpecificFeatures ( gaborF, Colorbins+Edgebins+tamuraF, TestingSize, MULTI_CLASS, csvname, testing );
		//ChiSquare(testing, csvname, MULTI_CLASS, NumberOfFeats);
		//ChiSquare8(testing, csvname, MULTI_CLASS, NumberOfFeats); 
			
		const char* PathTrain_final = "Training.csv";
		const char* PathTest_final  = "Testing.csv";	
			
		if(MULTI_CLASS == 4){
			ClassifiersAccuracy[0][i] = NaiveBayesClassifier(PathTrain_final, PathTest_final, MULTI_CLASS, MultiClass4, TrainingImages , TestingImages, FeaturesToClassifier );
			ClassifiersAccuracy[1][i] = SVMClassifier(PathTrain_final, PathTest_final, MULTI_CLASS, MultiClass4, TrainingImages , TestingImages, FeaturesToClassifier );
			ClassifiersAccuracy[2][i] = DecisionTreeClassifier(PathTrain_final, PathTest_final, MULTI_CLASS, MultiClass4, TrainingImages , TestingImages, FeaturesToClassifier );		
			ClassifiersAccuracy[3][i] = BoostClassifierMulti(PathTrain_final, PathTest_final, MULTI_CLASS, MultiClass4, TrainingImages, TestingImages, FeaturesToClassifier );
		}else{
			ClassifiersAccuracy[0][i] = NaiveBayesClassifier(PathTrain_final, PathTest_final, MULTI_CLASS, MultiClass8, TrainingImages , TestingImages, FeaturesToClassifier );
			ClassifiersAccuracy[1][i] = SVMClassifier(PathTrain_final, PathTest_final, MULTI_CLASS, MultiClass8, TrainingImages , TestingImages, FeaturesToClassifier );			
			ClassifiersAccuracy[2][i] = DecisionTreeClassifier(PathTrain_final, PathTest_final, MULTI_CLASS, MultiClass8, TrainingImages , TestingImages, FeaturesToClassifier );		
			ClassifiersAccuracy[3][i] = BoostClassifierMulti(PathTrain_final, PathTest_final, MULTI_CLASS, MultiClass8, TrainingImages, TestingImages, FeaturesToClassifier );
		}
	}
			
	double average = 0.0;
	cout << "10 cross-fold validation RESULTS " << endl << endl;
	cout << "NaiveBayes " << endl; 
	for(i= 0 ; i< CrossValMulti; i++){
		average += ClassifiersAccuracy[0][i];
		cout << ClassifiersAccuracy[0][i] << ";  ";
	}
	cout << endl << endl;	
	cout << "Naive Bayes average = " << average/i;
	cout << endl << endl;
	
	average = 0.0;
	cout << "SVM " << endl; 
	for(i= 0 ; i< CrossValMulti; i++){
		average += ClassifiersAccuracy[1][i];
		cout << ClassifiersAccuracy[1][i] << ";  ";
	}
	cout << endl << endl;		
	cout << "SVM average = " << average/i;	
	cout << endl << endl;
	
	average = 0.0;
	cout << "Decision Tree " << endl; 
	for(i= 0 ; i< CrossValMulti; i++){
		average += ClassifiersAccuracy[2][i];
		cout << ClassifiersAccuracy[2][i] << ";  ";
	}
	cout << endl << endl;		
	cout << "DT average = " << average/i;	
	cout << endl << endl;
	
	average = 0.0;
	cout << "AdaBoost " << endl; 
	for(i= 0 ; i< CrossValMulti; i++){
		average += ClassifiersAccuracy[3][i];
		cout << ClassifiersAccuracy[3][i] << ";  ";
	}
	cout << endl << endl;	
	cout << "AdaBoost average = " << average/i;	
	cout << endl << endl;	
}

int WebPageRecency( int TrainingImages, int TestingImages )
{
	/*
		Recency:
			- New Webpages
			- Old WebPages	
	*/
 	int TrainingSize = TrainingImages*BINARY_CLASS;
	int TestingSize	 = TestingImages;

	int i;
	const char* Path = "Recency10Fold.csv";
	char training 	= 'c';
	char testing 	= 'k';
	const char* csvname   = "Recency";

	int NumberOfFeats = 166;

	ClassifiersAccuracy = (double**)malloc((4)*sizeof(double*));
	for ( i = 0; i < 4 ; i++)
	{
		ClassifiersAccuracy[i] = (double *)malloc (CrossValBin * sizeof(double));
	}
	
	int numSamples, sampleTest, limSup;
	
		
	for ( i = 0; i < CrossValBin; i++) 
	{	
		cout << "iteration = " << i << endl;
		sampleTest = TestingImages/2;
		numSamples = sampleTest*i;
		limSup = TrainingSize + 10;
	
		CreateTable( TrainingSize, TestingSize );
	
		Mat training_data = Mat(TrainingSize, NUMBER_OF_TOTAL_FEATURES, CV_32FC1);
		Mat testing_data = Mat(TestingSize, NUMBER_OF_TOTAL_FEATURES, CV_32FC1);

		read_features_from_csv(Path, (TrainingImages-numSamples), (limSup - numSamples) , sampleTest);
		
		AllFeatures ( TrainingSize, BINARY_CLASS, csvname, training );
		//SpecificFeatures ( Colorbins, 0, TrainingSize, BINARY_CLASS, csvname, training );
		//SpecificFeatures ( Edgebins, Colorbins, TrainingSize, BINARY_CLASS, csvname, training );
		//SpecificFeatures ( tamuraF, Colorbins+Edgebins, TrainingSize, BINARY_CLASS, csvname, training );
		//SpecificFeatures ( gaborF, Colorbins+Edgebins+tamuraF-1, TrainingSize, BINARY_CLASS, csvname, training );
		//ChiSquare(training, csvname, BINARY_CLASS, NumberOfFeats); 
		
		AllFeatures ( TestingSize, BINARY_CLASS, csvname, testing );
		//SpecificFeatures ( Colorbins, 0, TestingSize, BINARY_CLASS, csvname, testing );
		//SpecificFeatures ( Edgebins, Colorbins, TestingSize, BINARY_CLASS, csvname, testing );
		//SpecificFeatures ( tamuraF, Colorbins+Edgebins, TestingSize, BINARY_CLASS, csvname, testing );
		//SpecificFeatures ( gaborF, Colorbins+Edgebins+tamuraF-1, TestingSize, BINARY_CLASS, csvname, testing );
		//ChiSquare(testing, csvname, BINARY_CLASS, NumberOfFeats );  

		const char* PathTrain_final = "Training.csv";
		const char* PathTest_final  = "Testing.csv";
		
		ClassifiersAccuracy[0][i] = NaiveBayesClassifier(PathTrain_final, PathTest_final, BINARY_CLASS, BinaryClass, TrainingImages, TestingImages, NumberOfFeats );
		ClassifiersAccuracy[1][i] = SVMClassifier(PathTrain_final, PathTest_final, BINARY_CLASS, BinaryClass, TrainingImages, TestingImages, NumberOfFeats );
		ClassifiersAccuracy[2][i] = DecisionTreeClassifier(PathTrain_final, PathTest_final, BINARY_CLASS, BinaryClass, TrainingImages, TestingImages, NumberOfFeats );
		ClassifiersAccuracy[3][i] = BoostClassifierBinary(PathTrain_final, PathTest_final, BINARY_CLASS, BinaryClass, TrainingImages, TestingImages, NumberOfFeats );
	}
	
	double average = 0.0;
	cout << "10 cross-fold validation RESULTS " << endl << endl;
	cout << "NaiveBayes " << endl; 
	for(i= 0 ; i< 10; i++){
		average += ClassifiersAccuracy[0][i];
		cout << ClassifiersAccuracy[0][i] << ";  ";
	}
	cout << endl << endl;	
	cout << "Naive Bayes average = " << average/i;
	cout << endl << endl;
	
	average = 0.0;
	cout << "SVM " << endl; 
	for(i= 0 ; i< 10; i++){
		average += ClassifiersAccuracy[1][i];
		cout << ClassifiersAccuracy[1][i] << ";  ";
	}
	cout << endl << endl;		
	cout << "SVM average = " << average/i;	
	cout << endl << endl;
	
	average = 0.0;
	cout << "Decision Tree " << endl; 
	for(i= 0 ; i< 10; i++){
		average += ClassifiersAccuracy[2][i];
		cout << ClassifiersAccuracy[2][i] << ";  ";
	}
	cout << endl << endl;		
	cout << "DT average = " << average/i;	
	cout << endl << endl;
	
	average = 0.0;
	cout << "AdaBoost " << endl; 
	for(i= 0 ; i< 10; i++){
		average += ClassifiersAccuracy[3][i];
		cout << ClassifiersAccuracy[3][i] << ";  ";
	}
	cout << endl << endl;	
	cout << "AdaBoost average = " << average/i;	
	cout << endl << endl;	

}

int AestheticValue( int TrainingImages, int TestingImages )
{
	/*
		Aesthetic Value:
			- Ugly Webpages
			- Beautiful WebPages	
	*/
 	int TrainingSize = TrainingImages*BINARY_CLASS;
	int TestingSize	 = TestingImages;
	int i;
	const char* Path = "Aesthetic10Fold.csv";
	char training 	= 'c';
	char testing 	= 'k';
	const char* csvname   = "Aesthetic";

	ClassifiersAccuracy = (double**)malloc((4)*sizeof(double*));
	for ( i = 0; i < 4 ; i++)
	{
		ClassifiersAccuracy[i] = (double *)malloc (CrossValBin * sizeof(double));
	}
	
	int numSamples, sampleTest, limSup;
	
		
	for ( i = 0; i < CrossValBin; i++) 
	{	
		cout << "iteration = " << i << endl;
		sampleTest = TestingImages/2;
		numSamples = sampleTest*i;
		limSup = TrainingSize + 10;
	
		CreateTable( TrainingSize, TestingSize );
	
		Mat training_data = Mat(TrainingSize, NUMBER_OF_TOTAL_FEATURES, CV_32FC1);
		Mat testing_data = Mat(TestingSize, NUMBER_OF_TOTAL_FEATURES, CV_32FC1);

		read_features_from_csv(Path, (TrainingImages-numSamples), (limSup - numSamples) , sampleTest);
	
		int NumberOfFeats = 166;
	
		AllFeatures ( TrainingSize, BINARY_CLASS, csvname, training );
		//SpecificFeatures ( Colorbins, 0, TrainingSize, BINARY_CLASS, csvname, training );
		//SpecificFeatures ( Edgebins, Colorbins, TrainingSize, BINARY_CLASS, csvname, training );
		//SpecificFeatures ( tamuraF, Colorbins+Edgebins, TrainingSize, BINARY_CLASS, csvname, training );
		//SpecificFeatures ( gaborF, Colorbins+Edgebins+tamuraF, TrainingSize, BINARY_CLASS, csvname, training );
		//ChiSquare( training, csvname, BINARY_CLASS, NumberOfFeats ); 
			
		AllFeatures ( TestingSize, BINARY_CLASS, csvname, testing );
		//SpecificFeatures ( Colorbins, 0, TestingSize, BINARY_CLASS, csvname, testing );
		//SpecificFeatures ( Edgebins, Colorbins, TestingSize, BINARY_CLASS, csvname, testing );
		//SpecificFeatures ( tamuraF, Colorbins+Edgebins, TestingSize, BINARY_CLASS, csvname, testing );
		//SpecificFeatures ( gaborF, Colorbins+Edgebins+tamuraF, TestingSize, BINARY_CLASS, csvname, testing );
		//ChiSquare( testing, csvname, BINARY_CLASS, NumberOfFeats ); 
		
		const char* PathTrain_final = "Training.csv";
		const char* PathTest_final  = "Testing.csv";
	
		ClassifiersAccuracy[0][i] = NaiveBayesClassifier(PathTrain_final, PathTest_final, BINARY_CLASS, BinaryClass, TrainingImages, TestingImages, NumberOfFeats );
		ClassifiersAccuracy[1][i] = SVMClassifier(PathTrain_final, PathTest_final, BINARY_CLASS, BinaryClass, TrainingImages, TestingImages, NumberOfFeats );
		ClassifiersAccuracy[2][i] = DecisionTreeClassifier(PathTrain_final, PathTest_final, BINARY_CLASS, BinaryClass, TrainingImages, TestingImages, NumberOfFeats );
		ClassifiersAccuracy[3][i] = BoostClassifierBinary(PathTrain_final, PathTest_final, BINARY_CLASS, BinaryClass, TrainingImages, TestingImages, NumberOfFeats );
	}
	double average = 0.0;
	cout << "10 cross-fold validation RESULTS " << endl << endl;
	cout << "NaiveBayes " << endl; 
	for(i= 0 ; i< 10; i++){
		average += ClassifiersAccuracy[0][i];
		cout << ClassifiersAccuracy[0][i] << ";  ";
	}
	cout << endl << endl;	
	cout << "Naive Bayes average = " << average/i;
	cout << endl << endl;
	
	average = 0.0;
	cout << "SVM " << endl; 
	for(i= 0 ; i< 10; i++){
		average += ClassifiersAccuracy[1][i];
		cout << ClassifiersAccuracy[1][i] << ";  ";
	}
	cout << endl << endl;		
	cout << "SVM average = " << average/i;	
	cout << endl << endl;
	
	average = 0.0;
	cout << "Decision Tree " << endl; 
	for(i= 0 ; i< 10; i++){
		average += ClassifiersAccuracy[2][i];
		cout << ClassifiersAccuracy[2][i] << ";  ";
	}
	cout << endl << endl;		
	cout << "DT average = " << average/i;	
	cout << endl << endl;
	
	average = 0.0;
	cout << "AdaBoost " << endl; 
	for(i= 0 ; i< 10; i++){
		average += ClassifiersAccuracy[3][i];
		cout << ClassifiersAccuracy[3][i] << ";  ";
	}
	cout << endl << endl;	
	cout << "AdaBoost average = " << average/i;	
	cout << endl << endl;	
	
}

void AllFeatures ( int NumberOFSamples, int NUMBER_OF_CLASSES, const char* csvname, char caracter )
{
	int i, h, z;
	int NumberOfSets, size, CheckClass;
	char directory[50];
	double 	SelectedFeat[NUMBER_OF_TOTAL_FEATURES];
	memset(SelectedFeat, 0, NUMBER_OF_TOTAL_FEATURES*sizeof(double));

	if(caracter == 'c') //Training data
	{
		NumberOfSets 	= 	NumberOFSamples;
		size = NUMBER_OF_TOTAL_FEATURES;
		sprintf(directory, "Training.csv");
		ifstream fin(directory);
		if (fin)
		{
			cout << "File found, deleting..." << endl;
			remove(directory);
		}
		CheckClass 	= 	NumberOfSets/NUMBER_OF_CLASSES;
		for(i = 0; i < NumberOfSets; i++){
			for( h = 0; h < NUMBER_OF_TOTAL_FEATURES; h++){
				SelectedFeat[h] = Training_Features[i][h];
			}
			if( i < CheckClass )
			{
				WriteinCSV( SelectedFeat, 0, directory, NUMBER_OF_TOTAL_FEATURES );
			}
			else if( (i >= CheckClass) && (i < CheckClass*2) )
			{
				WriteinCSV( SelectedFeat, 1, directory, NUMBER_OF_TOTAL_FEATURES );
			}
			else if( (i >= CheckClass*2) && (i < CheckClass*3) )
			{
				WriteinCSV( SelectedFeat, 2, directory, NUMBER_OF_TOTAL_FEATURES );
			}
			else if( (i >= CheckClass*3) && (i < CheckClass*4) )
			{
				WriteinCSV( SelectedFeat, 3, directory, NUMBER_OF_TOTAL_FEATURES );
			}
			else if( (i >= CheckClass*4) && (i < CheckClass*5) )
			{
				WriteinCSV( SelectedFeat, 4, directory, NUMBER_OF_TOTAL_FEATURES );
			}
			else if( (i >= CheckClass*5) && (i < CheckClass*6) )
			{
				WriteinCSV( SelectedFeat, 5, directory, NUMBER_OF_TOTAL_FEATURES );
			}
			else if( (i >= CheckClass*6) && (i < CheckClass*7) )
			{
				WriteinCSV( SelectedFeat, 6, directory, NUMBER_OF_TOTAL_FEATURES );
			}
			else if( (i >= CheckClass*7) )
			{
				WriteinCSV( SelectedFeat, 7, directory, NUMBER_OF_TOTAL_FEATURES );
			}
			
		}
		cout << "All features selected. " << endl;
		
		free(Training_Features[0]);
		free(Training_Features);
	}
	else if(caracter == 'k')
	{	
		
		NumberOfSets 	= 	NumberOFSamples;
		CheckClass 		= 	NumberOfSets/NUMBER_OF_CLASSES;
		size 	= 	NUMBER_OF_TOTAL_FEATURES;
		sprintf(directory, "Testing.csv");
		ifstream fin(directory);
		if (fin)
		{
			cout << "File found, deleting..." << endl;
			remove(directory);
		}
		for(i = 0; i < NumberOfSets; i++)
		{
			for(h = 0; h < NUMBER_OF_TOTAL_FEATURES; h++)
			{
				SelectedFeat[h] = Testing_Features[i][h];
			}
			if( i < CheckClass )
			{
				WriteinCSV( SelectedFeat, 0, directory, NUMBER_OF_TOTAL_FEATURES );
			}
			else if( (i >= CheckClass) && (i < CheckClass*2) )
			{
				WriteinCSV( SelectedFeat, 1, directory, NUMBER_OF_TOTAL_FEATURES );
			}
			else if( (i >= CheckClass*2) && (i < CheckClass*3) )
			{
				WriteinCSV( SelectedFeat, 2, directory, NUMBER_OF_TOTAL_FEATURES );
			}
			else if( (i >= CheckClass*3) && (i < CheckClass*4) )
			{
				WriteinCSV( SelectedFeat, 3, directory, NUMBER_OF_TOTAL_FEATURES );
			}
			else if( (i >= CheckClass*4) && (i < CheckClass*5) )
			{
				WriteinCSV( SelectedFeat, 4, directory, NUMBER_OF_TOTAL_FEATURES );
			}
			else if( (i >= CheckClass*5) && (i < CheckClass*6) )
			{
				WriteinCSV( SelectedFeat, 5, directory, NUMBER_OF_TOTAL_FEATURES );
			}
			else if( (i >= CheckClass*6) && (i < CheckClass*7) )
			{
				WriteinCSV( SelectedFeat, 6, directory, NUMBER_OF_TOTAL_FEATURES );
			}
			else if( (i >= CheckClass*7) )
			{
				WriteinCSV( SelectedFeat, 7, directory, NUMBER_OF_TOTAL_FEATURES );
			}
		}
		free(Testing_Features[0]);
		free(Testing_Features);
	}
}

void SpecificFeatures ( int bins, int begin, int NumberOFSamples, int NUMBER_OF_CLASSES, const char* csvname, char caracter )
{
	int i, h, z;
	int NumberOfSets, size, CheckClass;
	char directory[50];
	int pos;
	double 	SelectedFeat[bins];
	memset(SelectedFeat, 0, bins*sizeof(double));
	cout << "Number of Features: " << endl
			<< begin << " - " << bins+begin << endl;

	if(caracter == 'c') //Training data
	{
		NumberOfSets 	= 	NumberOFSamples;
		//size = NUMBER_OF_TOTAL_FEATURES;
		sprintf(directory, "Training.csv");
		ifstream fin(directory);
		if (fin)
		{
			cout << "File found, deleting..." << endl;
			remove(directory);
		}
		
		CheckClass 	= 	NumberOfSets/NUMBER_OF_CLASSES;
		for(i = 0; i < NumberOfSets; i++){
			for( h = 0; h < bins; h++){
				pos = (h+begin);
				SelectedFeat[h] = Training_Features[i][pos];
			}
			if( i < CheckClass )
			{
				WriteinCSV( SelectedFeat, 0, directory, bins );
			}
			else if( (i >= CheckClass) && (i < CheckClass*2) )
			{
				WriteinCSV( SelectedFeat, 1, directory, bins );
			}
			else if( (i >= CheckClass*2) && (i < CheckClass*3) )
			{
				WriteinCSV( SelectedFeat, 2, directory, bins );
			}
			else if( (i >= CheckClass*3) && (i < CheckClass*4) )
			{
				WriteinCSV( SelectedFeat, 3, directory, bins );
			}
			else if( (i >= CheckClass*4) && (i < CheckClass*5) )
			{
				WriteinCSV( SelectedFeat, 4, directory, bins );
			}
			else if( (i >= CheckClass*5) && (i < CheckClass*6) )
			{
				WriteinCSV( SelectedFeat, 5, directory, bins );
			}
			else if( (i >= CheckClass*6) && (i < CheckClass*7) )
			{
				WriteinCSV( SelectedFeat, 6, directory, bins );
			}
			else if( (i >= CheckClass*7) )
			{
				WriteinCSV( SelectedFeat, 7, directory, bins );
			}
		}
		
		free(Training_Features[0]);
		free(Training_Features);
	}
	else if(caracter == 'k')
	{	
		
		NumberOfSets 	= 	NumberOFSamples;
		CheckClass 		= 	NumberOfSets/NUMBER_OF_CLASSES;
		sprintf(directory, "Testing.csv");
		ifstream fin(directory);
		if (fin)
		{
			cout << "File found, deleting..." << endl;
			remove(directory);
		}
		for(i = 0; i < NumberOfSets; i++)
		{
			for(h = 0; h < bins; h++)
			{
				pos = (h+begin);
				SelectedFeat[h] = Testing_Features[i][pos];
			}
			if( i < CheckClass )
			{
				WriteinCSV( SelectedFeat, 0, directory, bins );
			}
			else if( (i >= CheckClass) && (i < CheckClass*2) )
			{
				WriteinCSV( SelectedFeat, 1, directory, bins );
			}
			else if( (i >= CheckClass*2) && (i < CheckClass*3) )
			{
				WriteinCSV( SelectedFeat, 2, directory, bins );
			}
			else if( (i >= CheckClass*3) && (i < CheckClass*4) )
			{
				WriteinCSV( SelectedFeat, 3, directory, bins );
			}
			else if( (i >= CheckClass*4) && (i < CheckClass*5) )
			{
				WriteinCSV( SelectedFeat, 4, directory, bins );
			}
			else if( (i >= CheckClass*5) && (i < CheckClass*6) )
			{
				WriteinCSV( SelectedFeat, 5, directory, bins );
			}
			else if( (i >= CheckClass*6) && (i < CheckClass*7) )
			{
				WriteinCSV( SelectedFeat, 6, directory, bins );
			}
			else if( (i >= CheckClass*7) )
			{
				WriteinCSV( SelectedFeat, 7, directory, bins );
			}
		}
		free(Testing_Features[0]);
		free(Testing_Features);
	}
}

double BoostClassifierBinary(const char* train, const char* test, int NumberOfClasses , int* ClassTable,  int trainingsize, int testingsize, int samplessize )
{
			printf( "\n");
			printf( "........................................................\n ");
			printf( "		BINARY BOOOST CLASSIFIER 	\n ");
			printf( "........................................................\n ");

	int i;
	int NUMBER_OF_CLASSES = NumberOfClasses;
	int ATTRIBUTES_PER_SAMPLE = samplessize;
	int NUMBER_OF_TRAINING_SAMPLES = trainingsize;
	int NUMBER_OF_TESTING_SAMPLES = testingsize;
	//	define training data storage matrices (one for attribute examples, one for classifications)
    Mat training_data = Mat(NUMBER_OF_TRAINING_SAMPLES*NUMBER_OF_CLASSES, ATTRIBUTES_PER_SAMPLE, CV_32F);
    Mat training_classifications = Mat(NUMBER_OF_TRAINING_SAMPLES*NUMBER_OF_CLASSES, 1, CV_32F);
	
    // define testing data storage matrices
    Mat testing_data = Mat(NUMBER_OF_TESTING_SAMPLES, ATTRIBUTES_PER_SAMPLE, CV_32F);
    Mat testing_classifications = Mat(NUMBER_OF_TESTING_SAMPLES, 1, CV_32F);
	
	
	if ((read_data_from_csv(train, training_data, training_classifications, NUMBER_OF_TRAINING_SAMPLES*NUMBER_OF_CLASSES) &&
            read_data_from_csv(test, testing_data, testing_classifications, NUMBER_OF_TESTING_SAMPLES)))
    {
	
	CvMat dataTrain 	= training_data;
	CvMat LabelTrain 	= training_classifications;
	
	printf("Training Boost... ");
	CvBoost boost;
    boost.train(&dataTrain, CV_ROW_SAMPLE, &LabelTrain, 0, 0, 0, 0, CvBoostParams(CvBoost::REAL, 100, 0.95, 1, false, 0), false);
	
	// perform classifier testing and report results
    Mat test_sample;
	CvMat TestingSample;
    int correct_class = 0;
    int wrong_class = 0;
    int false_positives [NUMBER_OF_CLASSES];
    char class_labels[NUMBER_OF_CLASSES];
    float result;
	
	// zero the false positive counters in a simple loop
	int tsample;
    for (i = 0; i < NUMBER_OF_CLASSES; i++)
    {
        false_positives[i] = 0;
    }

    printf( "\nUsing testing database: %s\n\n", test);

    for (tsample = 0; tsample < NUMBER_OF_TESTING_SAMPLES; tsample++)
    {
		// extract a row from the testing matrix
		test_sample = testing_data.row(tsample);
		TestingSample = test_sample;
		
		// run decision tree prediction
		result = boost.predict(&TestingSample, 0, 0, CV_WHOLE_SEQ, false, false);

		printf("Testing Sample %i -> class result (%d)\n", tsample, ClassTable[((int) result)]);

		if (fabs(result - testing_classifications.at<float>(tsample, 0)) != 0)
            {
                // if they differ more than floating point error => wrong class
                wrong_class++;
                false_positives[((int) result)]++;
            }else
            {
                // otherwise correct
                correct_class++;
            }
        }

        printf( "\nResults on the testing database: %s\n"
                "\tCorrect classification: %d (%g%%)\n"
                "\tWrong classifications: %d (%g%%)\n",
                test,
                correct_class, (double) correct_class*100/NUMBER_OF_TESTING_SAMPLES,
                wrong_class, (double) wrong_class*100/NUMBER_OF_TESTING_SAMPLES);

        for (int i = 0; i < NUMBER_OF_CLASSES; i++)
        {
            printf( "\tClass (value %d) false postives 	%d (%g%%)\n", ClassTable[i],
                    false_positives[i],
                    (double) false_positives[i]*100/NUMBER_OF_TESTING_SAMPLES);
        }
		// all matrix memory free by destructors
        // all OK : main returns 0
        return (correct_class*100/NUMBER_OF_TESTING_SAMPLES);
	}	
	// not OK : main returns -1
    return -1;
	
} 

double BoostClassifierMulti(const char* train, const char* test, int NumberOfClasses , int* ClassTable,  int trainingsize, int testingsize, int samplessize )
{
  
   			printf( "\n");
			printf( "........................................................\n ");
			printf( "		MULTICLASS BOOOST CLASSIFIER 	\n ");
			printf( "........................................................\n ");

    const int NUMBER_OF_CLASSES = NumberOfClasses;
	int ATTRIBUTES_PER_SAMPLE = samplessize;
	int NUMBER_OF_TRAINING_SAMPLES = trainingsize;
	int NUMBER_OF_TESTING_SAMPLES = testingsize;
    //CvMat* data = 0;
    //CvMat* responses = 0;
    // CvMat* var_type = 0;
    // CvMat* temp_sample = 0;
    // CvMat* weak_responses = 0;

    Mat training_data = Mat(NUMBER_OF_TRAINING_SAMPLES*NUMBER_OF_CLASSES, ATTRIBUTES_PER_SAMPLE, CV_32F);
    Mat training_classifications = Mat(NUMBER_OF_TRAINING_SAMPLES*NUMBER_OF_CLASSES, 1, CV_32F);
	
    // define testing data storage matrices
    Mat testing_data = Mat(NUMBER_OF_TESTING_SAMPLES, ATTRIBUTES_PER_SAMPLE, CV_32F);
    Mat testing_classifications = Mat(NUMBER_OF_TESTING_SAMPLES, 1, CV_32F);
	
	
    if (read_data_from_csv(train, training_data, training_classifications, NUMBER_OF_TRAINING_SAMPLES*NUMBER_OF_CLASSES) &&
            read_data_from_csv(test, testing_data, testing_classifications, NUMBER_OF_TESTING_SAMPLES))
    {
	
	// CvMat data 	= training_data;
	// CvMat responses 	= training_classifications;
	
	// CvMat testdata 	= testing_data;
	// CvMat testresponses 	= testing_classifications;
	
	Mat new_data = Mat(NUMBER_OF_TRAINING_SAMPLES*NUMBER_OF_CLASSES*NUMBER_OF_CLASSES, ATTRIBUTES_PER_SAMPLE + 1, CV_32F );
    Mat new_responses = Mat(NUMBER_OF_TRAINING_SAMPLES*NUMBER_OF_CLASSES*NUMBER_OF_CLASSES, 1, CV_32S );

    // 1. unroll the training samples

    printf( "\nUnrolling the database...");
	fflush(NULL);
    for(int i = 0; i < NUMBER_OF_TRAINING_SAMPLES*NUMBER_OF_CLASSES; i++ )
	{
		for(int j = 0; j < NUMBER_OF_CLASSES; j++ )
		{
			for(int k = 0; k < ATTRIBUTES_PER_SAMPLE; k++ )
            {
				// copy over the attribute data	
				new_data.at<float>((i * NUMBER_OF_CLASSES) + j, k) = training_data.at<float>(i, k);
            }
			// set the new attribute to the original class
			new_data.at<float>((i * NUMBER_OF_CLASSES) + j, ATTRIBUTES_PER_SAMPLE) = (float) j;
			
			// set the new binary class
			if ( ( (int) training_classifications.at<float>( i, 0)) == j)
			{
				new_responses.at<int>((i * NUMBER_OF_CLASSES) + j, 0) = 1;
			}
			else
			{
				new_responses.at<int>((i * NUMBER_OF_CLASSES) + j, 0) = 0;
            }
        }
	}
	printf( "Done\n");
	
	// 2. Unroll the type mask
    // define all the attributes as numerical
	// alternatives are CV_VAR_CATEGORICAL or CV_VAR_ORDERED(=CV_VAR_NUMERICAL)
	// that can be assigned on a per attribute basis
	
	Mat var_type = Mat(ATTRIBUTES_PER_SAMPLE + 2, 1, CV_8U );
    var_type.setTo(Scalar(CV_VAR_NUMERICAL) ); // all inputs are numerical

        // this is a classification problem (i.e. predict a discrete number of class
        // outputs) so reset the last (+1) output var_type element to CV_VAR_CATEGORICAL
        // *** the last (new) class indicator attribute, as well
        // *** as the new (binary) response (class) are categorical

	var_type.at<uchar>(ATTRIBUTES_PER_SAMPLE, 0) = CV_VAR_CATEGORICAL;
	var_type.at<uchar>(ATTRIBUTES_PER_SAMPLE + 1, 0) = CV_VAR_CATEGORICAL;

	// define the parameters for training the boosted trees
	// weights of each classification for classes
	// N.B. in the "unrolled" data we have an imbalance in the training examples

    float priors[] = {( NUMBER_OF_CLASSES - 1),1};
    //float priors[] = {1,1};

	// set the boost parameters

	CvBoostParams params = CvBoostParams(CvBoost::REAL,  // boosting type
                                         100,			 // number of weak classifiers
                                         0.95,   		 // trim rate
										 
                                             // trim rate is a threshold (0->1)
                                             // used to eliminate samples with
                                             // boosting weight < 1.0 - (trim rate)
                                             // from the next round of boosting
                                             // Used for computational saving only.

                                         25, 	  // max depth of trees
                                         false,  // compute surrogate split, no missing data
                                         priors );

        // as CvBoostParams inherits from CvDTreeParams we can also set generic
        // parameters of decision trees too (otherwise they use the defaults)

	params.max_categories = 15; 	// max number of categories (use sub-optimal algorithm for larger numbers)
    params.min_sample_count = 5; 	// min sample count
    params.cv_folds = 1;					// cross validation folds
	params.use_1se_rule = false; 			// use 1SE rule => smaller tree
	params.truncate_pruned_tree = false; 	// throw away the pruned tree branches
	params.regression_accuracy = 0.0; 		// regression accuracy: N/A here


	// train boosted tree classifier (using training data)

	printf( "\nUsing training database: %s\n\n", train);
	printf( "Training .... (this may take several minutes) .... ");
	fflush(NULL);

	CvBoost* boostTree = new CvBoost;

	boostTree->train( new_data, CV_ROW_SAMPLE, new_responses, Mat(), Mat(), var_type, Mat(), params, false);
	printf( "Done.");

	    // perform classifier testing and report results

	Mat test_sample;
	int correct_class 	= 0;
	int wrong_class 	= 0;
	int false_positives [NUMBER_OF_CLASSES];
	 
	for( int i = 0; i < NUMBER_OF_CLASSES; i++)
	{
		false_positives[i] = 0;
	}
	Mat weak_responses		= Mat( 1, boostTree->get_weak_predictors()->total, CV_32F );
	Mat new_sample		 	= Mat( 1,  ATTRIBUTES_PER_SAMPLE + 1, CV_32F );
	
	int best_class = 0; // best class returned by weak classifier
	double max_sum;	 // highest score for a given class
	
	printf( "\nUsing testing database: %s\n\n", test);

	CvMat weakresponses 	= weak_responses;
	
	
	for (int tsample = 0; tsample < NUMBER_OF_TESTING_SAMPLES; tsample++)
	{

		// extract a row from the testing matrix

		test_sample = testing_data.row(tsample);
		
		// convert it to the new "un-rolled" format of input

		for(int k = 0; k < ATTRIBUTES_PER_SAMPLE; k++ )
		{
			new_sample.at<float>( 0, k) = test_sample.at<float>(0, k);
		}

		// run boosted tree prediction (for N classes and take the
		// maximal response of all the weak classifiers)

		max_sum = INT_MIN; // maximum starts off as Min. Int.
	
		for(int c = 0; c < NUMBER_OF_CLASSES; c++ )
		{
		// set the additional attribute to original class

			new_sample.at<float>(0, ATTRIBUTES_PER_SAMPLE) = (float) c;

			// run prediction (getting also the responses of the weak classifiers)
			// - N.B. here we have to use CvMat() casts and take the address of temporary
			// in order to use the available call that gives us the weak responses
			// For this reason we also have to pass a NULL pointer for the missing data
			CvMat predsample 	= new_sample;
			//boostTree->predict(&CvMat((new_sample)), NULL, &CvMat(weak_responses));
			boostTree->predict(&predsample, NULL, &weakresponses, CV_WHOLE_SEQ, false, false);
			
			// obtain the sum of the responses from the weak classifiers

			Scalar responseSum = sum( weak_responses );

			// record the "best class" - i.e. one with maximal response
			// from weak classifiers

			if( responseSum.val[0] > max_sum)
			{
				max_sum = (double) responseSum.val[0];
				best_class = c;
				}
			}

		printf("Testing Sample %i -> class result (digit %d)\n", tsample, best_class);

		// if the prediction and the (true) testing classification are the same
		// (N.B. openCV uses a floating point decision tree implementation!)

		if (fabs(((float) (best_class)) - testing_classifications.at<float>( tsample, 0)) >= FLT_EPSILON)
		{
			// if they differ more than floating point error => wrong class
			wrong_class++;
			false_positives[best_class]++;

		}else{
			// otherwise correct
			correct_class++;
		}
    }

		printf( "\nResults on the testing database: %s\n"
				"\tCorrect classification: %d (%g%%)\n"
				"\tWrong classifications: %d (%g%%)\n",
				test,
				correct_class, (double) correct_class*100/NUMBER_OF_TESTING_SAMPLES,
				wrong_class, (double) wrong_class*100/NUMBER_OF_TESTING_SAMPLES);

		for (int i = 0; i < NUMBER_OF_CLASSES; i++)
		{
			printf( "\tClass (digit %d) false postives 	%d (%g%%)\n", i,
					false_positives[i],
					(double) false_positives[i]*100/NUMBER_OF_TESTING_SAMPLES);
		}
		// all matrix memory free by destructors
		// all OK : main returns 0

		return (correct_class*100/NUMBER_OF_TESTING_SAMPLES);
    }
    // not OK : main returns -1
    return -1;
}

double DecisionTreeClassifier(const char* train, const char* test, int NumberOfClasses, int* ClassTable,  int trainingsize, int testingsize, int samplessize ) 
{
			printf( "\n");
			printf( "........................................................\n ");
			printf( "		DECISION TREE CLASSIFIER 	\n ");
			printf( "........................................................\n ");

	int i;
	int NUMBER_OF_CLASSES = NumberOfClasses;
	int ATTRIBUTES_PER_SAMPLE = samplessize;
	int NUMBER_OF_TRAINING_SAMPLES = trainingsize;
	int NUMBER_OF_TESTING_SAMPLES = testingsize;
	
	Mat training_data = Mat(NUMBER_OF_TRAINING_SAMPLES*NUMBER_OF_CLASSES, ATTRIBUTES_PER_SAMPLE, CV_32FC1);
    Mat training_classifications = Mat(NUMBER_OF_TRAINING_SAMPLES*NUMBER_OF_CLASSES, 1, CV_32FC1);

    //define testing data storage matrices

    Mat testing_data = Mat(NUMBER_OF_TESTING_SAMPLES, ATTRIBUTES_PER_SAMPLE, CV_32FC1);
    Mat testing_classifications = Mat(NUMBER_OF_TESTING_SAMPLES, 1, CV_32FC1);

    // define all the attributes as numerical
    // alternatives are CV_VAR_CATEGORICAL or CV_VAR_ORDERED(=CV_VAR_NUMERICAL)
    // that can be assigned on a per attribute basis

    Mat var_type = Mat(ATTRIBUTES_PER_SAMPLE + 1, 1, CV_8U );
    var_type.setTo(Scalar(CV_VAR_NUMERICAL) ); // all inputs are numerical

    // this is a classification problem (i.e. predict a discrete number of class
    // outputs) so reset the last (+1) output var_type element to CV_VAR_CATEGORICAL
    var_type.at<uchar>(ATTRIBUTES_PER_SAMPLE, 0) = CV_VAR_CATEGORICAL;
	
    CvDTreeNode* resultNode; // node returned from a prediction

    // load training and testing data sets
    if (read_data_from_csv(train, training_data, training_classifications, NUMBER_OF_TRAINING_SAMPLES*NUMBER_OF_CLASSES) &&
            read_data_from_csv(test, testing_data, testing_classifications, NUMBER_OF_TESTING_SAMPLES))
    {
        // define the parameters for training the decision tree
        float *priors = NULL;  // weights of each classification for classes
        // (all equal as equal samples of each character)

        CvDTreeParams params = CvDTreeParams(  		25, // max depth  
												 5, // min sample count  
												 0, // regression accuracy: N/A here  
											   true, // compute surrogate split,   
													 //   as we have missing data  
												 15, // max number of categories   
													 //   (use sub-optimal algorithm for  
													 //   larger numbers)  
												 10, // cross-validations   
											   true, // use 1SE rule => smaller tree  
											   true, // throw away the pruned tree branches  
											  priors // the array of priors, the bigger   
													 //   p_weight, the more attention  
													 //   to the poisonous mushrooms  
												) ;


        // train decision tree classifier (using training data)

        printf( "\nUsing training database: %s\n\n", train);
        CvDTree* dtree = new CvDTree;

        dtree->train(training_data, CV_ROW_SAMPLE, training_classifications,
                     Mat(), Mat(), var_type, Mat(), params);

        // perform classifier testing and report results

        Mat test_sample;
        int correct_class = 0;
        int wrong_class = 0;
        int false_positives [NUMBER_OF_CLASSES];

        // zero the false positive counters in a simple loop

        for (i = 0; i < NUMBER_OF_CLASSES; i++)
        {
            false_positives[i] = 0;
        }

        printf( "\nUsing testing database: %s\n\n", test);

        for (int tsample = 0; tsample < NUMBER_OF_TESTING_SAMPLES; tsample++)
        {
            // extract a row from the testing matrix
            test_sample = testing_data.row(tsample);

            // run decision tree prediction
            resultNode = dtree->predict(test_sample, Mat(), false);

            printf("Testing Sample %i -> class result (character %d)\n", tsample,
                   ClassTable[((int) (resultNode->value))]);

            // if the prediction and the (true) testing classification are the same
            // (N.B. openCV uses a floating point decision tree implementation!)

            if (fabs(resultNode->value - testing_classifications.at<float>(tsample, 0))
                    >= FLT_EPSILON)
            {
                // if they differ more than floating point error => wrong class
                wrong_class++;
                false_positives[((int) (resultNode->value))]++;
            }
            else
            {
                // otherwise correct
                correct_class++;
            }
        }

        printf( "\nResults on the testing database: %s\n"
                "\tCorrect classification: %d (%g%%)\n"
                "\tWrong classifications: %d (%g%%)\n",
                test,
                correct_class, (double) correct_class*100/NUMBER_OF_TESTING_SAMPLES,
                wrong_class, (double) wrong_class*100/NUMBER_OF_TESTING_SAMPLES);

        for (int i = 0; i < NUMBER_OF_CLASSES; i++)
        {
            printf( "\tClass (value %d) false postives 	%d (%g%%)\n", ClassTable[i],
                    false_positives[i],
                    (double) false_positives[i]*100/NUMBER_OF_TESTING_SAMPLES);
        }
        // all matrix memory free by destructors
        // all OK : main returns 0
        return (correct_class*100/NUMBER_OF_TESTING_SAMPLES);
    }
    // not OK : main returns -1
    return -1;
}

double SVMClassifier(const char* train, const char* test, int NumberOfClasses, int* ClassTable,  int trainingsize, int testingsize, int samplessize )
{	
					printf( "\n");
					printf( "........................................................\n ");
					printf( "		SUPPORT VECTOR MACHINE CLASSIFIER 	\n ");
					printf( "........................................................\n ");

	int i;
	int NUMBER_OF_CLASSES = NumberOfClasses;
	int ATTRIBUTES_PER_SAMPLE = samplessize;
	int NUMBER_OF_TRAINING_SAMPLES = trainingsize;
	int NUMBER_OF_TESTING_SAMPLES = testingsize;
	
    // define training data storage matrices (one for attribute examples, one for classifications)
    Mat training_data = Mat(NUMBER_OF_TRAINING_SAMPLES*NUMBER_OF_CLASSES, ATTRIBUTES_PER_SAMPLE, CV_32FC1);
    Mat training_classifications = Mat(NUMBER_OF_TRAINING_SAMPLES*NUMBER_OF_CLASSES, 1, CV_32FC1);

    //define testing data storage matrices
    Mat testing_data = Mat(NUMBER_OF_TESTING_SAMPLES, ATTRIBUTES_PER_SAMPLE, CV_32FC1);
    Mat testing_classifications = Mat(NUMBER_OF_TESTING_SAMPLES, 1, CV_32FC1);

    // load training and testing data sets
    if (read_data_from_csv( train, training_data, training_classifications, NUMBER_OF_TRAINING_SAMPLES*NUMBER_OF_CLASSES ) &&
            read_data_from_csv( test, testing_data, testing_classifications, NUMBER_OF_TESTING_SAMPLES ))
    {
		// cout << training_data << endl;
		// cout << training_classifications << endl;
		// cout << testing_data << endl;
		// cout << testing_classifications << endl;

        // define the parameters for training the SVM (kernel + SVMtype type used for auto-training,
        // other parameters for manual only)
	
        CvSVMParams params = CvSVMParams(
                                 CvSVM::C_SVC, // Type of SVM, here N classes (see manual)
								 CvSVM::LINEAR, // kernel type (see manual)
                                 0.0,			// kernel parameter (degree) for poly kernel only
								 0.0,			// kernel parameter (gamma) for poly/rbf kernel only
                                 0.0,			// kernel parameter (coef0) for poly/sigmoid kernel only
                                 100,			// SVM optimization parameter C
                                 0.0,				// SVM optimization parameter nu (not used for N classe SVM)
                                 0,				// SVM optimization parameter p (not used for N classe SVM)
                                 NULL,			// class wieghts (or priors)
                                 // Optional weights, assigned to particular classes.
                                 // They are multiplied by C and thus affect the misclassification
                                 // penalty for different classes. The larger weight, the larger penalty
                                 // on misclassification of data from the corresponding class.

                                 // termination criteria for learning algorithm
                                 //cvTermCriteria(CV_TERMCRIT_ITER, 1000, 0.01)
                                 cvTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 1000, FLT_EPSILON)

                             );

        // train SVM classifier (using training data)

        printf( "\nUsing training database: %s", train);
        CvSVM* svm = new CvSVM;

        printf( "\nTraining the SVM (in progress) ..... ");
        fflush(NULL);

 #if (USE_OPENCV_GRID_SEARCH_AUTOTRAIN)

        printf( "(SVM 'grid search' => may take some time!)");
        fflush(NULL);

        // train using auto training parameter grid search if it is available
        // (i.e. OpenCV 2.x) with 10 fold cross valdiation
        // N.B. this does not search kernel choice

        svm->train_auto(training_data, training_classifications, Mat(), Mat(), params, 10);
        params = svm->get_params();
        printf( "\nUsing optimal parameters degree %f, gamma %f, ceof0 %f\n\t C %f, nu %f, p %f\n Training ..",
                params.degree, params.gamma, params.coef0, params.C, params.nu, params.p);
#else
        // otherwise use regular training and use parameters manually specified above
        svm->train(training_data, training_classifications, Mat(), Mat(), params);

#endif
        printf( ".... Done\n");

        // get the number of support vectors used to define the SVM decision boundary
        printf("Number of support vectors for trained SVM = %i\n", svm->get_support_vector_count());

        // perform classifier testing and report results

        Mat test_sample;
        int correct_class = 0;
        int wrong_class = 0;
        int false_positives [NUMBER_OF_CLASSES];
        char class_labels[NUMBER_OF_CLASSES];
        float result;

        // zero the false positive counters in a simple loop
        for (i = 0; i < NUMBER_OF_CLASSES; i++)
        {
            false_positives[i] = 0;
            //class_labels[i] = (char) 65 + i; // ASCII 65 = A
        }

        printf( "\nUsing testing database: %s\n\n", test);
        for (int tsample = 0; tsample < NUMBER_OF_TESTING_SAMPLES; tsample++)
        {

            // extract a row from the testing matrix
            test_sample = testing_data.row(tsample);

            // run SVM classifier
            result = svm->predict(test_sample);

            printf("Testing Sample %i -> class result (character %d)\n", tsample,  ClassTable[((int) result)]);
            // if the prediction and the (true) testing classification are the same
            // (N.B. openCV uses a floating point decision tree implementation!)

            if (fabs(result - testing_classifications.at<float>(tsample, 0))
                    >= FLT_EPSILON)
            {
                // if they differ more than floating point error => wrong class
                wrong_class++;
                false_positives[(int) (testing_classifications.at<float>(tsample, 0))]++;
            }
            else
            {
                // otherwise correct
                correct_class++;
            }
        }

        printf( "\nResults on the testing database: %s\n"
                "\tCorrect classification: %d (%g%%)\n"
                "\tWrong classifications: %d (%g%%)\n",
                test,
                correct_class, (double) correct_class*100/NUMBER_OF_TESTING_SAMPLES,
                wrong_class, (double) wrong_class*100/NUMBER_OF_TESTING_SAMPLES);

        for (unsigned char i = 0; i < NUMBER_OF_CLASSES; i++)
        {
            printf( "\tClass (value %d) false positives 	%d (%g%%)\n",ClassTable[(int) i],
                    false_positives[(int) i],
                    (double) false_positives[i]*100/NUMBER_OF_TESTING_SAMPLES);
        }

        // all matrix memory free by destructors
        // all OK : main returns 0
        return (correct_class*100/NUMBER_OF_TESTING_SAMPLES);
    }

    // not OK : main returns -1
    return -1;
}

double NaiveBayesClassifier(const char* train, const char* test, int NumberOfClasses, int *ClassTable, int trainingsize, int testingsize, int samplessize )
{
			printf( "\n");
			printf( "........................................................\n ");
			printf( "		NAIVE BAYES CLASSIFIER 		\n ");
			printf( ".........................................................\n ");
	int i;
	int NUMBER_OF_CLASSES = NumberOfClasses;
	int ATTRIBUTES_PER_SAMPLE = samplessize;
	int NUMBER_OF_TRAINING_SAMPLES = trainingsize;
	int NUMBER_OF_TESTING_SAMPLES = testingsize;
	//	define training data storage matrices (one for attribute examples, one for classifications)
    Mat training_data = Mat(NUMBER_OF_TRAINING_SAMPLES*NUMBER_OF_CLASSES, ATTRIBUTES_PER_SAMPLE, CV_32F);
    Mat training_classifications = Mat(NUMBER_OF_TRAINING_SAMPLES*NUMBER_OF_CLASSES, 1, CV_32F);
	
    // define testing data storage matrices
    Mat testing_data = Mat(NUMBER_OF_TESTING_SAMPLES, ATTRIBUTES_PER_SAMPLE, CV_32F);
    Mat testing_classifications = Mat(NUMBER_OF_TESTING_SAMPLES, 1, CV_32F);
	
	if ((read_data_from_csv( train, training_data, training_classifications, NUMBER_OF_TRAINING_SAMPLES*NUMBER_OF_CLASSES ) &&
            read_data_from_csv( test, testing_data, testing_classifications, NUMBER_OF_TESTING_SAMPLES )))
    {
/* 		plot_binary(training_data, training_classifications, "Training Data", NUMBER_OF_TRAINING_SAMPLES*NUMBER_OF_CLASSES);
		waitKey(0);
		cvDestroyWindow("Training Data");
		plot_binary(testing_data, testing_classifications, "Test Data", NUMBER_OF_TESTING_SAMPLES);	
		waitKey(0);
		cvDestroyWindow("Test Data"); */
		//cout << "Training data = " << "  " << training_data << endl;
		//cout << "Testing data = " << "  " << testing_data << endl;
		//cout << "Testing classifications = " << "  " << testing_classifications << endl;
		// train bayesian classifier (using training data)
        printf( "\nUsing training database: %s\n", train);
		CvNormalBayesClassifier* bayes = new CvNormalBayesClassifier();		
		
		bayes->train(training_data,training_classifications,Mat(),Mat(),false); 
		
		//CvNormalBayesClassifier bayes(training_data, training_classifications);
		// perform classifier testing and report results
        Mat test_sample;
        int correct_class = 0;
        int wrong_class = 0;
        int false_positives [NUMBER_OF_CLASSES];
        char class_labels[NUMBER_OF_CLASSES];
        float result;
		
		// zero the false positive counters in a simple loop
		int tsample;
        for (i = 0; i < NUMBER_OF_CLASSES; i++)
        {
            false_positives[i] = 0;
        }

        printf( "\nUsing testing database: %s\n\n", test);
		//Mat predicted(testing_classifications.rows, 1, CV_32F);
        for (tsample = 0; tsample < NUMBER_OF_TESTING_SAMPLES; tsample++)
        {
			// extract a row from the testing matrix
            test_sample = testing_data.row(tsample);

            // run decision tree prediction
            result = bayes->predict(test_sample);
			printf("Testing Sample %i -> class result (%d)\n", tsample, ClassTable[((int) result)]);
			// if the prediction and the (true) testing classification are the same
            // (N.B. openCV uses a floating point decision tree implementation!)

            /* if (fabs(result - testing_classifications.at<float>(tsample, 0))
                    >= FLT_EPSILON)
            { */
			if (fabs(result - testing_classifications.at<float>(tsample, 0))
                    != 0)
            {
                // if they differ more than floating point error => wrong class
                wrong_class++;
                false_positives[((int) result)]++;
            }else
            {
                // otherwise correct
                correct_class++;
            }
        }
		
		// plot_binary(testing_data, predicted, "Predictions Bayes", NUMBER_OF_TESTING_SAMPLES);
		// waitKey(0);
		// cvDestroyWindow("Predictions Bayes");

        printf( "\nResults on the testing database: %s\n"
                "\tCorrect classification: %d (%g%%)\n"
                "\tWrong classifications: %d (%g%%)\n",
                test,
                correct_class, (double) correct_class*100/NUMBER_OF_TESTING_SAMPLES,
                wrong_class, (double) wrong_class*100/NUMBER_OF_TESTING_SAMPLES);

        for (int i = 0; i < NUMBER_OF_CLASSES; i++)
        {
            printf( "\tClass (value %d) false postives 	%d (%g%%)\n", ClassTable[i],
                    false_positives[i],
                    (double) false_positives[i]*100/NUMBER_OF_TESTING_SAMPLES);
        }
		// all matrix memory free by destructors
        // all OK : main returns 0
        return (correct_class*100/NUMBER_OF_TESTING_SAMPLES);
	}
	
	// not OK : main returns -1
    return -1;
}

int read_features_from_csv(const char* filename , int limInf, int limSup, int TestSample )
{
    string line;
    int linenum = 0;
	float value;
	
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

			if (linenum < limInf ){
				Training_Features[linenum][itemnum] = value;
				//cout << "erro1 - " << linenum << endl;
			}else if( (linenum >= limInf) && (linenum < (limInf+TestSample)) ){ 
				Testing_Features[linenum-limInf][itemnum] = value;
				//cout << "erro2 - " << (linenum-limInf) << endl;

			}else if( linenum >= (limInf+TestSample) && linenum < limSup ){
				Training_Features[linenum-TestSample][itemnum] = value;
				//cout << "erro3 - " << (linenum-TestSample) << endl;

			}else if( (linenum >= limSup) && (linenum < (limSup+TestSample)) ){ 
				Testing_Features[linenum-limSup+TestSample][itemnum] = value;
				//cout << "erro4 - " << (linenum-limSup+TestSample) << endl;

			}else if (linenum >= (limSup+TestSample)) {
				Training_Features[linenum-TestSample-TestSample][itemnum] = value;
				//cout << "erro5 - " << (linenum-TestSample-TestSample) << endl;
			}
			itemnum++;
        }
		linenum++;
	}
	
	inFile.close();		
	//fflush(NULL);
	return 1;
}

int read_features_from_csv1(const char* filename , int lim, int nSam, int TestSample )
{
    string line;
    int linenum = 0;
	float value;
	int i = 1;
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

			if (linenum < (lim - nSam)){
				Training_Features[linenum][itemnum] = value;
			
			}else if( (linenum >= (lim - nSam) ) && (linenum < (lim + TestSample - nSam)) ){ 
				Testing_Features[linenum-lim+nSam][itemnum] = value;
			
			}else if( linenum >= (((lim*i) - lim) + (TestSample*(i-1)) - nSam ) && linenum < lim*i + (TestSample*(i-1)) - nSam){
				Training_Features[linenum-(TestSample*(i-1))][itemnum] = value;
			
			}else if( (linenum >= ((lim*i) + (TestSample*(i-1)) - nSam)) && (linenum < (lim*i) + (TestSample*i) - nSam)){ 
				Testing_Features[linenum-(lim*i)+nSam][itemnum] = value;
			}
			itemnum++;
        }
		cout << linenum << "------" << i << endl;
		linenum++;
		if( linenum == ((lim*i) + (TestSample*i))) i++;
	}
	
	inFile.close();		
	//fflush(NULL);
	return 1;
}

int read_features_from_csv(const char* filename , Mat data, char caracter )
{
    string line;
    int linenum = 0;
	float value;
	
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
			//cout << value;
			if(caracter == 'c') //Training data
			{ 
				Training_Features[linenum][itemnum] = value;
				//cout << Training_Features[i][j];
			}
			else if(caracter == 'k') 
			{
				Testing_Features[linenum][itemnum] = value;
			}

			itemnum++;
        }
		//cout << endl;
		//cout << "row = " << linenum << " and cols = " << itemnum << endl;
		linenum++;
		
	}
	
	inFile.close();		
	//fflush(NULL);
	return 1;
}

int read_data_from_csv(const char* filename, Mat data, Mat classes, int n_samples )
{

	ifstream inFile (filename);
    string line;
    int linenum = 0;
	float value;
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
			//cout << "Item #" << itemnum << ": " << value << endl;
			if(itemnum == 0){
				if( value == 0){
					classes.at<float>(linenum,0) = 0;
                }else if ( value == 1){ 
					classes.at<float>(linenum,0) = 1;
				}else if ( value == 2){ 
					classes.at<float>(linenum,0) = 2;
				}else if ( value == 3){ 
					classes.at<float>(linenum,0) = 3;
				}else if ( value == 4){
					classes.at<float>(linenum,0) = 4;
                }else if ( value == 5){ 
					classes.at<float>(linenum,0) = 5;
				}else if ( value == 6){ 
					classes.at<float>(linenum,0) = 6;
				}else if ( value == 7){ 
					classes.at<float>(linenum,0) = 7;
                }else{
                    printf("ERROR: unexpected class in file %s\n",  filename);
				}
			}else{
				data.at<float>(linenum, (itemnum-1)) = value;
			}
			itemnum++;
        }
		linenum++;
    }
	inFile.close();	
	fflush(NULL);
	return 1;
}

void ChiSquare( char caracter, const char* csvname, int NUMBER_OF_CLASSES, int tamanho )
{
	int 	i,k,h,z;
	int size;
	double 	ExpectedFreq = 0;
	int 	NumberOfSets;
	int 	NumberOfFeats = NUMBER_OF_TOTAL_FEATURES;
	int 	ATTRIBUTES_PER_SAMPLE = tamanho;
	
	if(caracter == 'c') //Training data
	{ 
		ExpectedFreq 	= 	TRAINING_SAMPLES;
		NumberOfSets 	= 	TRAINING_SAMPLES*NUMBER_OF_CLASSES;
		printf("\n Creating	Training Samples.\n"
				" Number Of Sets  =  %d.\n", 	NumberOfSets);	
	}
	else if(caracter == 'k') //Testing data
	{ 
		NumberOfSets 	= 	TESTING_SAMPLES;
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
		
		double 	Chi2[NumberOfFeats]; //X^2
		double 	Chi2temp[NumberOfFeats]; 
		memset(Chi2, 0, NumberOfFeats*sizeof(double));
		memset(Chi2temp, 0, NumberOfFeats*sizeof(double));
		int sizeInf, sizeSup;
		
		position = (int*)calloc(ATTRIBUTES_PER_SAMPLE,sizeof(int*));
		
		for(k = 0; k < NumberOfFeats; k++){
	
			p = 0;
			z = 0;
			
			double temp[NUMBER_OF_CLASSES];
			double ObservedFreq[NUMBER_OF_CLASSES];
			memset(temp, 0, NUMBER_OF_CLASSES*sizeof(double));
			memset(ObservedFreq, 0, NUMBER_OF_CLASSES*sizeof(double));
		
			
			sizeInf = NumberOfSets/NUMBER_OF_CLASSES;
			sizeSup = sizeInf + TRAINING_SAMPLES;
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
				else if( ( i > (sizeInf + TRAINING_SAMPLES) ) && ( i < (sizeSup + TRAINING_SAMPLES)) )
				{
					if( (Training_Features[i][k] > val[z+1][k]) && (Training_Features[i][k] <= val[z+2][k]) )
					{
						ObservedFreq[p+2] += 1;
					}
				}
				else if(  i > ( sizeSup + TRAINING_SAMPLES + TRAINING_SAMPLES) )
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
		
		sortAscending(Chi2, NumberOfFeats);
		
		/*for(k = 0; k < NumberOfFeats; k++){
				printf ("Chi-Square value %d : %f      .  %f\n " , k, Chi2[k], Chi2temp[k]);
		}  */
		vector<int> lastpos;
		for( h = 0; h < ATTRIBUTES_PER_SAMPLE; h++ )
		{
			for( z = 0; z < NumberOfFeats; z++ )
			{
				if( fabs(Chi2[h] - Chi2temp[z]) == 0.0)
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
						Chi2temp[z] = 0;
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
	double 	SelectedFeat[ATTRIBUTES_PER_SAMPLE];
	memset(SelectedFeat, 0, ATTRIBUTES_PER_SAMPLE*sizeof(double));
	
	if (caracter == 'c')
	{
 
		size 	= 	NumberOfSets/NUMBER_OF_CLASSES;
		sprintf(directory, "Training.csv");
		ifstream fin(directory);
		if (fin)
		{
			cout << "File found, deleting..." << endl;
			remove(directory);
		}
		for(k = 0; k < NumberOfSets; k++)
		{
			for(h = 0; h < ATTRIBUTES_PER_SAMPLE; h++)
			{
				pos = position[h];
				SelectedFeat[h] = Training_Features[k][pos];
			}
			if( k < size )
			{
				WriteinCSV( SelectedFeat, 0, directory, ATTRIBUTES_PER_SAMPLE );
			}
			else if( (k >= size) && (k < size*2) )
			{
				WriteinCSV( SelectedFeat, 1, directory, ATTRIBUTES_PER_SAMPLE );
			}
			else if( (k >= size*2) && (k < size*3) )
			{
				WriteinCSV( SelectedFeat, 2, directory, ATTRIBUTES_PER_SAMPLE );
			}
			else if( (k >= size*3) )
			{
				WriteinCSV( SelectedFeat, 3, directory, ATTRIBUTES_PER_SAMPLE );
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
		sprintf(directory, "Testing.csv");
		ifstream fin(directory);
		if (fin)
		{
			cout << "File found, deleting..." << endl;
			remove(directory);
		}
		for(k = 0; k < NumberOfSets; k++)
		{
			for(h = 0; h < ATTRIBUTES_PER_SAMPLE; h++)
			{
				pos = position[h];
				SelectedFeat[h] = Testing_Features[k][pos];
			}
			if( k < size )
			{
				WriteinCSV( SelectedFeat, 0, directory, ATTRIBUTES_PER_SAMPLE );
			}
			else if( (k >= size) && (k < size*2) )
			{
				WriteinCSV( SelectedFeat, 1, directory, ATTRIBUTES_PER_SAMPLE );
			}
			else if( (k >= size*2) && (k < size*3) )
			{
				WriteinCSV( SelectedFeat, 2, directory, ATTRIBUTES_PER_SAMPLE );
			}
			else if( (k >= size*3) )
			{
				WriteinCSV( SelectedFeat, 3, directory,ATTRIBUTES_PER_SAMPLE );
			}
		}
		free(position);
		free(Testing_Features[0]);
		free(Testing_Features);
	}
}

void ChiSquare8( char caracter, const char* csvname, int NUMBER_OF_CLASSES, int tamanho )
{
	int 	i,k,h,z;
	int size;
	double 	ExpectedFreq = 0;
	int 	NumberOfSets;
	int 	NumberOfFeats = NUMBER_OF_TOTAL_FEATURES;
	int 	ATTRIBUTES_PER_SAMPLE = tamanho;
	
	if(caracter == 'c') //Training data
	{ 
		ExpectedFreq 	= 	TRAINING_SAMPLES;
		NumberOfSets 	= 	TRAINING_SAMPLES*NUMBER_OF_CLASSES;
		printf("\n Creating	Training Samples.\n"
				" Number Of Sets  =  %d.\n", 	NumberOfSets);	
	}
	else if(caracter == 'k') //Testing data
	{ 
		NumberOfSets 	= 	TESTING_SAMPLES;
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
					else if( (k >= size*3) && (k < size*4) )
					{
						mean[p+3][i] += Training_Features[k][i];
					}
					else if( (k >= size*4) && (k < size*5) )
					{
						mean[p+4][i] += Training_Features[k][i];
					}
					else if( (k >= size*5) && (k < size*6) )
					{
						mean[p+5][i] += Training_Features[k][i];
					}
					else if( (k >= size*6) && (k < size*7) )
					{
						mean[p+6][i] += Training_Features[k][i];
					}
					else if( (k >= size*7) )
					{
						mean[p+7][i] += Training_Features[k][i];
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
		
		double 	Chi2[NumberOfFeats]; //X^2
		double 	Chi2temp[NumberOfFeats]; 
		memset(Chi2, 0, NumberOfFeats*sizeof(double));
		memset(Chi2temp, 0, NumberOfFeats*sizeof(double));
		int sizeInf, sizeSup;
		
		position = (int*)malloc((ATTRIBUTES_PER_SAMPLE)*sizeof(int*));
		
		for(k = 0; k < NumberOfFeats; k++){
	
			p = 0;
			z = 0;
			
			double temp[NUMBER_OF_CLASSES];
			double ObservedFreq[NUMBER_OF_CLASSES];
			memset(temp, 0, NUMBER_OF_CLASSES*sizeof(double));
			memset(ObservedFreq, 0, NUMBER_OF_CLASSES*sizeof(double));
		
			
			sizeInf = NumberOfSets/NUMBER_OF_CLASSES;
			sizeSup = sizeInf + TRAINING_SAMPLES;
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
				else if( ( i > (sizeInf + TRAINING_SAMPLES) ) && ( i < (sizeSup + TRAINING_SAMPLES)) )
				{
					if( (Training_Features[i][k] > val[z+1][k]) && (Training_Features[i][k] <= val[z+2][k]) )
					{
						ObservedFreq[p+2] += 1;
					}
				}
				else if( ( i > (sizeSup + TRAINING_SAMPLES) ) && ( i < (sizeSup + TRAINING_SAMPLES + TRAINING_SAMPLES )) )
				{
					if( (Training_Features[i][k] > val[z+2][k]) && (Training_Features[i][k] <= val[z+3][k]) )
					{
						ObservedFreq[p+3] += 1;
					}
				}
				else if( ( i > (sizeSup + TRAINING_SAMPLES + TRAINING_SAMPLES) ) && ( i < (sizeSup + TRAINING_SAMPLES + TRAINING_SAMPLES + TRAINING_SAMPLES)) )
				{
					if( (Training_Features[i][k] > val[z+3][k]) && (Training_Features[i][k] <= val[z+4][k]) )
					{
						ObservedFreq[p+4] += 1;
					}
				}
				else if( ( i > (sizeSup + TRAINING_SAMPLES + TRAINING_SAMPLES + TRAINING_SAMPLES) ) && ( i < (sizeSup + TRAINING_SAMPLES + TRAINING_SAMPLES + TRAINING_SAMPLES + TRAINING_SAMPLES)) )
				{
					if( (Training_Features[i][k] > val[z+4][k]) && (Training_Features[i][k] <= val[z+5][k]) )
					{
						ObservedFreq[p+5] += 1;
					}
				}
				else if( ( i > (sizeSup + TRAINING_SAMPLES + TRAINING_SAMPLES + TRAINING_SAMPLES + TRAINING_SAMPLES) ) && ( i < (sizeSup + TRAINING_SAMPLES + TRAINING_SAMPLES + TRAINING_SAMPLES + TRAINING_SAMPLES + TRAINING_SAMPLES)) )
				{
					if( (Training_Features[i][k] > val[z+5][k]) && (Training_Features[i][k] <= val[z+6][k]) )
					{
						ObservedFreq[p+6] += 1;
					}
				}
				else if(  i > (sizeSup + TRAINING_SAMPLES + TRAINING_SAMPLES + TRAINING_SAMPLES + TRAINING_SAMPLES + TRAINING_SAMPLES) )
				{
					if( (Training_Features[i][k] > val[z+6][k]) )
					{
						ObservedFreq[p+7] += 1;
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
		
		sortAscending(Chi2, NumberOfFeats);
		
		/*for(k = 0; k < NumberOfFeats; k++){
				printf ("Chi-Square value %d : %f      .  %f\n " , k, Chi2[k], Chi2temp[k]);
		}  */
		vector<int> lastpos;
		for( h = 0; h < ATTRIBUTES_PER_SAMPLE; h++ )
		{
			for( z = 0; z < NumberOfFeats; z++ )
			{
				if( fabs(Chi2[h] - Chi2temp[z]) == 0.0)
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
						Chi2temp[z] = 0;
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
	double 	SelectedFeat[ATTRIBUTES_PER_SAMPLE];
	memset(SelectedFeat, 0, ATTRIBUTES_PER_SAMPLE*sizeof(double));
	
	if (caracter == 'c')
	{
 
		size 	= 	NumberOfSets/NUMBER_OF_CLASSES;
		sprintf(directory, "Training.csv");
		ifstream fin(directory);
		if (fin)
		{
			cout << "File found, deleting..." << endl;
			remove(directory);
		}
		for(k = 0; k < NumberOfSets; k++)
		{
			for(h = 0; h < ATTRIBUTES_PER_SAMPLE; h++)
			{
				pos = position[h];
				SelectedFeat[h] = Training_Features[k][pos];
			}
			if( k < size )
			{
				WriteinCSV( SelectedFeat, 0, directory, ATTRIBUTES_PER_SAMPLE );
			}
			else if( (k >= size) && (k < size*2) )
			{
				WriteinCSV( SelectedFeat, 1, directory, ATTRIBUTES_PER_SAMPLE );
			}
			else if( (k >= size*2) && (k < size*3) )
			{
				WriteinCSV( SelectedFeat, 2, directory, ATTRIBUTES_PER_SAMPLE );
			}
			else if( (k >= size*3) && (k < size*4) )
			{
				WriteinCSV( SelectedFeat, 3, directory, ATTRIBUTES_PER_SAMPLE );
			}
			else if( (k >= size*4) && (k < size*5) )
			{
				WriteinCSV( SelectedFeat, 4, directory, ATTRIBUTES_PER_SAMPLE );
			}
			else if( (k >= size*5) && (k < size*6) )
			{
				WriteinCSV( SelectedFeat, 5, directory, ATTRIBUTES_PER_SAMPLE );
			}
			else if( (k >= size*6) && (k < size*7) )
			{
				WriteinCSV( SelectedFeat, 6, directory, ATTRIBUTES_PER_SAMPLE );
			}
			else if( (k >= size*7) )
			{
				WriteinCSV( SelectedFeat, 7, directory, ATTRIBUTES_PER_SAMPLE );
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
		sprintf(directory, "Testing.csv");
		ifstream fin(directory);
		if (fin)
		{
			cout << "File found, deleting..." << endl;
			remove(directory);
		}
		for(k = 0; k < NumberOfSets; k++)
		{
			for(h = 0; h < ATTRIBUTES_PER_SAMPLE; h++)
			{
				pos = position[h];
				SelectedFeat[h] = Testing_Features[k][pos];
			}
			if( k < size )
			{
				WriteinCSV( SelectedFeat, 0, directory, ATTRIBUTES_PER_SAMPLE );
			}
			else if( (k >= size) && (k < size*2) )
			{
				WriteinCSV( SelectedFeat, 1, directory, ATTRIBUTES_PER_SAMPLE );
			}
			else if( (k >= size*2) && (k < size*3) )
			{
				WriteinCSV( SelectedFeat, 2, directory, ATTRIBUTES_PER_SAMPLE );
			}
			else if( (k >= size*3) && (k < size*4) )
			{
				WriteinCSV( SelectedFeat, 3, directory, ATTRIBUTES_PER_SAMPLE );
			}
			else if( (k >= size*4) && (k < size*5) )
			{
				WriteinCSV( SelectedFeat, 4, directory, ATTRIBUTES_PER_SAMPLE );
			}
			else if( (k >= size*5) && (k < size*6) )
			{
				WriteinCSV( SelectedFeat, 5, directory, ATTRIBUTES_PER_SAMPLE );
			}
			else if( (k >= size*6) && (k < size*7) )
			{
				WriteinCSV( SelectedFeat, 6, directory, ATTRIBUTES_PER_SAMPLE );
			}
			else if( (k >= size*7) )
			{
				WriteinCSV( SelectedFeat, 7, directory, ATTRIBUTES_PER_SAMPLE );
			}
		}
		free(position);
		free(Testing_Features[0]);
		free(Testing_Features);
	}
}
	
int WriteinCSV(double SelectedFeatures[], int flag, const char* directory, int size)
{
	/* 	
		Save the variables that characterize the image in a csv file, separable by a 
		comma FLAG (single value) - regards to the type of the class of each image.
	*/
	int 		i = 0;
	ofstream 	myfile;

	myfile.open (directory, ios::app);
	myfile << flag << ",";

	
	while(i < (size-1))
	{
		myfile << SelectedFeatures[i] << ",";
		i++;
	}
	myfile << SelectedFeatures[i] << endl;
	myfile.close();
	fflush(NULL);
	return 0; 
}

void sortAscending(double arr[], int size)
{
		int i, j;
        double temp;

        for( i = 0; i < size; i++ )
        {
                for( j = 0;j < size - 1; j++ )
                {
                        if( arr[j] > arr[j+1] )
                        {
                                //we need to swap
                                temp = arr[j];
                                arr[j] = arr[j+1];
                                arr[j+1] = temp;
                        }
                }
        }
}		

void sortDecreasing(double arr[], int size)
{
		int i, j;
        double temp;

        for( i = 0; i < size; i++ )
        {
                for( j = 0;j < size - 1; j++ )
                {
                        if( arr[j] < arr[j+1] )
                        {
                                //we need to swap
                                temp = arr[j];
                                arr[j] = arr[j+1];
                                arr[j+1] = temp;
                        }
                }
        }
}		