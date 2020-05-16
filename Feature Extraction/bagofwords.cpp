#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cv.h>       // opencv general include file
#include <ml.h>		  // opencv machine learning include file
#include <highgui.h>

#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#define NUMBER_OF_TRAINING_SAMPLES 180
#define NUMBER_OF_TESTING_SAMPLES 20
//#define DICTIONARY_BUILD 1// set DICTIONARY_BUILD to 1 for Step 1. 0 for step 2 

const char* filePath = "";
const char* filePath1 = "";
const char* filePath2 = "";
const char* filePath3 = "";
const char* filePath4 = "";
const char* filePath5 = "";
const char* filePath6 = "";
const char* filePath7 = "";
const char* filePath8 = "";
const char* filePath9 = "";

//See article on BoW model for details
Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("FlannBased");
Ptr<DescriptorExtractor> extractor = DescriptorExtractor::create("SIFT");
//Ptr<FeatureDetector> detector = FeatureDetector::create("SURF");


//See article on BoW model for details
int dictionarySize = 500;
TermCriteria tc(CV_TERMCRIT_ITER, 10, 0.001);
int retries = 1;
int flags = KMEANS_PP_CENTERS;

//See article on BoW model for details
BOWKMeansTrainer bowTrainer(dictionarySize, tc, retries, flags);
//See article on BoW model for details
BOWImgDescriptorExtractor bowDE(extractor, matcher);

static int BinaryClass[2] = {0, 1};
static int MultiClass[4]  = {0, 1, 2, 3};
static int MultiClass8[8]  = {0, 1, 2, 3, 4, 5, 6, 7};


/**
 * \brief Recursively traverses a folder hierarchy. Extracts features from the training images and adds them to the bowTrainer.
 */
void extractTrainingVocabulary(const char* filePath1, const char* filePath2,  const char* filePath3, const char* filePath4, int numberOfImages, int numberOfClasses) {
    SiftFeatureDetector detector;
	int 	i;
	char 	directory[50];
	int size = numberOfImages/numberOfClasses;
	
	for( i = 1; i <= numberOfImages; i++ )
	{
		if(i <= size){
			sprintf(directory, "%s/%1d.png", filePath1, i);
			printf(" Processing image %d in %s.		\n", i, directory);
			float label = 0;
		}else if( i > (size) && i <= (size*2) ){
			sprintf(directory, "%s/%1d.png", filePath2, (i - size));
			printf(" Processing image %d in %s.		\n", (i - size) , directory);
			float label = 1;
		}else if( i > (size*2) && i <= (size*3) ){
			sprintf(directory, "%s/%1d.png", filePath3, (i - (size*2) ));
			printf(" Processing image %d in %s.		\n", (i - (size*2) ) , directory);
			float label = 2;
		}else if( i > (size*3)){
			sprintf(directory, "%s/%1d.png", filePath4, (i - (size*3) ));
			printf(" Processing image %d in %s.		\n", (i - (size*3) ) , directory);
			float label = 3;
		}
		
		Mat img = imread(directory);
		if (!img.empty()) {
			vector<KeyPoint> keypoints;
			detector.detect(img, keypoints);
			
			if (keypoints.empty()) {
				cerr << "Warning: Could not find key points in image: "
								<< i << endl;
			} else {
						Mat features;						
						extractor->compute(img, keypoints, features);
						bowTrainer.add(features);
					}
		} else {
			cerr << "Warning: Could not read image: " << i << endl;
		}
		img.release();
		
	}
}


void extractTrainingVocabulary8(const char* filePath1, const char* filePath2,  const char* filePath3, const char* filePath4, const char* filePath5, 
								const char* filePath6, const char* filePath7, const char* filePath8, int numberOfImages, int numberOfClasses) {
    SiftFeatureDetector detector;
	int 	i;
	char 	directory[50];
	int size = numberOfImages/numberOfClasses;
	
	for( i = 1; i <= numberOfImages; i++ )
	{
		if(i <= size){
			sprintf(directory, "%s/%1d.png", filePath1, i);
			printf(" Processing image %d in %s.		\n", i, directory);
			float label = 0;
		}else if( i > (size) && i <= (size*2) ){
			sprintf(directory, "%s/%1d.png", filePath2, (i - size));
			printf(" Processing image %d in %s.		\n", (i - size) , directory);
			float label = 1;
		}else if( i > (size*2) && i <= (size*3) ){
			sprintf(directory, "%s/%1d.png", filePath3, (i - (size*2) ));
			printf(" Processing image %d in %s.		\n", (i - (size*2) ) , directory);
			float label = 2;
		}else if( i > (size*3) && i <= (size*4) ){
			sprintf(directory, "%s/%1d.png", filePath4, (i - (size*3) ));
			printf(" Processing image %d in %s.		\n", (i - (size*3) ) , directory);
			float label = 3;
		}else if( i > (size*4) && i <= (size*5) ){
			sprintf(directory, "%s/%1d.png", filePath5, (i - (size*4) ));
			printf(" Processing image %d in %s.		\n", (i - (size*4) ) , directory);
			float label = 4;
		}else if( i > (size*5) && i <= (size*6) ){
			sprintf(directory, "%s/%1d.png", filePath6, (i - (size*5) ));
			printf(" Processing image %d in %s.		\n", (i - (size*3) ) , directory);
			float label = 5;
		}else if( i > (size*6) && i <= (size*7) ){
			sprintf(directory, "%s/%1d.png", filePath7, (i - (size*6) ));
			printf(" Processing image %d in %s.		\n", (i - (size*6) ) , directory);
			float label = 6;
		}else if( i > (size*7)){
			sprintf(directory, "%s/%1d.png", filePath8, (i - (size*7) ));
			printf(" Processing image %d in %s.		\n", (i - (size*7) ) , directory);
			float label = 7;
		}
		
		Mat img = imread(directory);
		if (!img.empty()) {
			vector<KeyPoint> keypoints;
			detector.detect(img, keypoints);
			
			if (keypoints.empty()) {
				cerr << "Warning: Could not find key points in image: "
								<< i << endl;
					} else {
						Mat features;						
						extractor->compute(img, keypoints, features);
						bowTrainer.add(features);
					}
				} else {
					cerr << "Warning: Could not read image: "
							<< i << endl;
				}
		
	}
}

/**
 * \brief Recursively traverses a folder hierarchy. Creates a BoW descriptor for each image encountered.
 */
void extractBOWDescriptor(const char* filePath1, const char* filePath2, const char* filePath3, const char* filePath4, Mat& descriptors, Mat& labels, int numberOfImages, int numberOfClasses) {
    SiftFeatureDetector detector;
	int 	i;
	float label;
	char 	directory[50];
	const char* path;
	int size = numberOfImages/numberOfClasses;

	for( i = 1; i <= numberOfImages; i++ )
	{
		if(strcmp(filePath1, filePath2) == 0)
		{
			sprintf(directory, "%s/%1d.png", filePath1, i);
			printf( " Processing image %d in %s.	\n", i, directory);
			if(i <= size)
			{
				label = 0.0;
			}
			else if( i > (size) && i <= (size*2) )
			{
				label = 1.0;
			}
			else if( i > (size*2) && i<  (size*3) ) 
			{
				label = 2.0;
			}
			else if( i > (size*3) )
			{
				label = 3.0;
			}
			
		}else{
			if( i <= size )
			{
				sprintf(directory, "%s/%1d.png", filePath1, i);
				printf(" Processing image %d in %s.		\n", i, directory);
				label = 0.0;
			}
			else if( i > (size) && i <= (size*2) ){
				sprintf(directory, "%s/%1d.png", filePath2, (i - size));
				printf(" Processing image %d in %s.		\n", (i - size) , directory);
				label = 1.0;
			}
			else if( i > (size*2) && i <= (size*3) )
			{
				sprintf(directory, "%s/%1d.png", filePath3, (i - (size*2) ));
				printf(" Processing image %d in %s.		\n", (i - (size*2) ) , directory);
				label = 2.0;
			}
			else if( i > (size*3) )
			{
				sprintf(directory, "%s/%1d.png", filePath4, (i - (size*3) ));
				printf(" Processing image %d in %s.		\n", (i - (size*3) ) , directory);
				label = 3.0;
			}
		}
		
		Mat img = imread(directory);
		if ( !img.empty() ) 
		{
			vector<KeyPoint> keypoints;
			detector.detect(img, keypoints);
			if ( keypoints.empty() ) 
			{
				cerr << "Warning: Could not find key points in image: "
						<< i << endl;
			}else{

				Mat bowDescriptor;
				bowDE.compute(img, keypoints, bowDescriptor);
				descriptors.push_back(bowDescriptor);
                cout << "Label : " << label << endl;
				labels.push_back(label);
			}
		} else {
			cerr << "Warning: Could not read image: "
					<< i << endl;
		}
	}
}

void extractBOWDescriptor8(const char* filePath1, const char* filePath2, const char* filePath3, const char* filePath4, const char* filePath5, 
								const char* filePath6, const char* filePath7, const char* filePath8, Mat& descriptors, Mat& labels, int numberOfImages, int numberOfClasses) {
    SiftFeatureDetector detector;
	int 	i;
	float label;
	char 	directory[50];
	const char* path;
	int size = numberOfImages/numberOfClasses;
	cout << "size = " << size << endl;
	for( i = 1; i <= numberOfImages; i++ )
	{
		if(strcmp(filePath1, filePath2) == 0)
		{
			sprintf(directory, "%s/%1d.png", filePath1, i);
			printf( " Processing image %d in %s.	\n", i, directory);
			if(i <= size)
			{
				label = 0.0;
			}
			else if( i > (size) && i <= (size*2) )
			{
				label = 1.0;
			}
			else if( i > (size*2) && i<=  (size*3) ) 
			{
				label = 2.0;
			}
			else if( i > (size*3) && i <= (size*4) ) 
			{
				label = 3.0;
			}
			else if( i > (size*4) && i <= (size*5) ) 
			{
				label = 4.0;
			}
			else if( i > (size*5) && i <= (size*6) ) 
			{
				label = 5.0;
			}
			else if( i > (size*6) && i <= (size*7) ) 
			{
				label = 6.0;
			}
			else if( i > (size*7) )
			{
				label = 7.0;
			}
			
		}else{
			if( i <= size ){
				sprintf(directory, "%s/%1d.png", filePath1, i);
				printf(" Processing image %d in %s.		\n", i, directory);
				label = 0.0;
			}else if( i > (size) && i <= (size*2) ){
				sprintf(directory, "%s/%1d.png", filePath2, (i - size));
				printf(" Processing image %d in %s.		\n", (i - size) , directory);
				label = 1.0;
			}else if( i > (size*2) && i <= (size*3) ){
				sprintf(directory, "%s/%1d.png", filePath3, (i - (size*2) ));
				printf(" Processing image %d in %s.		\n", (i - (size*2) ) , directory);
				label = 2.0;
			}else if( i > (size*3) && i <= (size*4) ){
				sprintf(directory, "%s/%1d.png", filePath4, (i - (size*3) ));
				printf(" Processing image %d in %s.		\n", (i - (size*3) ) , directory);
				label = 3.0;
			}else if( i > (size*4) && i <= (size*5) ){
				sprintf(directory, "%s/%1d.png", filePath5, (i - (size*4) ));
				printf(" Processing image %d in %s.		\n", (i - (size*4) ) , directory);
				label = 4.0;
			}else if( i > (size*5) && i <= (size*6) ){
				sprintf(directory, "%s/%1d.png", filePath6, (i - (size*5) ));
				printf(" Processing image %d in %s.		\n", (i - (size*5) ) , directory);
				label = 5.0;
			}else if( i > (size*6) && i <= (size*7) ){
				sprintf(directory, "%s/%1d.png", filePath7, (i - (size*6) ));
				printf(" Processing image %d in %s.		\n", (i - (size*6) ) , directory);
				label = 6.0;
			}else if( i > (size*7)){
				sprintf(directory, "%s/%1d.png", filePath8, (i - (size*7) ));
				printf(" Processing image %d in %s.		\n", (i - (size*7) ) , directory);
				label = 7.0;
			}
		}
		
		Mat img = imread(directory);
		if ( !img.empty() ) 
		{
			vector<KeyPoint> keypoints;
			detector.detect(img, keypoints);
			if ( keypoints.empty() ) 
			{
				cerr << "Warning: Could not find key points in image: "
						<< i << endl;
			}else{

				Mat bowDescriptor;
				bowDE.compute(img, keypoints, bowDescriptor);
				descriptors.push_back(bowDescriptor);
				
                cout << "Label : " << label << endl;
				labels.push_back(label);

			}
		} else {
			cerr << "Warning: Could not read image: "
					<< i << endl;
		}
	}
}


int NaiveBayesClassifier(Mat trainingData, Mat labels, Mat evalData, Mat groundTruth, int NumberOfClasses, int *ClassTable )
{
			printf( "\n\n........................................................\n ");
			printf( "		NAIVE BAYES CLASSIFIER 		\n ");
			printf( ".........................................................\n ");
			
	int i;
	int NUMBER_OF_CLASSES = NumberOfClasses;

    printf( "\nUsing training database: \n");
	CvNormalBayesClassifier* bayes = new CvNormalBayesClassifier();
	bayes->train(trainingData,labels,Mat(),Mat(),false);//MAT()
	while(1){	
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

		printf( "\nUsing testing database: \n");

		for (tsample = 0; tsample < NUMBER_OF_TESTING_SAMPLES; tsample++)
		{
			// extract a row from the testing matrix
			test_sample = evalData.row(tsample);

			// run decision tree prediction
			result = bayes->predict(test_sample);
			printf("Testing Sample %i -> class result (%d)\n", tsample, ClassTable[((int) result)]);
			// if the prediction and the (true) testing classification are the same
			// (N.B. openCV uses a floating point decision tree implementation!)

			/* if (fabs(result - testing_classifications.at<float>(tsample, 0))
						>= FLT_EPSILON)
				{ */
			if (fabs(result - groundTruth.at<float>(tsample, 0)) != 0)
			{
					// if they differ more than floating point error => wrong class
				wrong_class++;
				false_positives[((int) result)]++;
			}else{
				// otherwise correct
				correct_class++;
			}
		}

		printf( "\nResults on the testing database:\n"
                "\tCorrect classification: %d (%g%%)\n"
                "\tWrong classifications: %d (%g%%)\n",
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
        return 0;
	}	
	// not OK : main returns -1
    return -1;
}
	
int BoostClassifier(Mat trainingData, Mat labels, Mat evalData, Mat groundTruth, int NumberOfClasses , int* ClassTable )
{
			printf( "\n\n........................................................\n ");
			printf( "		BOOOST CLASSIFIER 	\n ");
			printf( "........................................................\n ");

	int i;
	int NUMBER_OF_CLASSES = NumberOfClasses;
	int ATTRIBUTES_PER_SAMPLE = dictionarySize;
	Mat training_data = trainingData;
    Mat training_classifications = labels;
	
    // define testing data storage matrices
    Mat testing_data = evalData;
    Mat testing_classifications = groundTruth;
	
	//CvMat dataTrain 	= trainingData;
	//CvMat LabelTrain 	= labels;
	while(1) {
	// CvMat data 	= training_data;
	// CvMat responses 	= training_classifications;
	
	// CvMat testdata 	= testing_data;
	// CvMat testresponses 	= testing_classifications;
	
	Mat new_data = Mat(NUMBER_OF_TRAINING_SAMPLES*NUMBER_OF_CLASSES, ATTRIBUTES_PER_SAMPLE + 1, CV_32F );
    Mat new_responses = Mat(NUMBER_OF_TRAINING_SAMPLES*NUMBER_OF_CLASSES, 1, CV_32S );

    // 1. unroll the training samples

    printf( "\nUnrolling the database...");
	fflush(NULL);
    for(int i = 0; i < NUMBER_OF_TRAINING_SAMPLES; i++ )
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

	printf( "\nUsing training database:\n\n");
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
	
	printf( "\nUsing testing database:\n\n");

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

		printf( "\nResults on the testing database:\n"
				"\tCorrect classification: %d (%g%%)\n"
				"\tWrong classifications: %d (%g%%)\n",
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

		return 0;
    }
	return 1;
} 

int SVMClassifier(Mat trainingData, Mat labels, Mat evalData, Mat groundTruth, int NumberOfClasses , int* ClassTable )
{	
					printf( "\n\n........................................................\n ");
					printf( "		SUPPORT VECTOR MACHINE CLASSIFIER 	\n ");
					printf( "........................................................\n ");

	int i;
	int NUMBER_OF_CLASSES = NumberOfClasses;
	
	while(1)
    {
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

        printf( "\nUsing training database: " );
        CvSVM* svm = new CvSVM;

        printf( "\nTraining the SVM (in progress) ..... ");
        fflush(NULL);

 #if (USE_OPENCV_GRID_SEARCH_AUTOTRAIN)

        printf( "(SVM 'grid search' => may take some time!)");
        fflush(NULL);

        // train using auto training parameter grid search if it is available
        // (i.e. OpenCV 2.x) with 10 fold cross valdiation
        // N.B. this does not search kernel choice
		
        svm->train_auto(trainingData, labels, Mat(), Mat(), params, 10);
        params = svm->get_params();
        printf( "\nUsing optimal parameters degree %f, gamma %f, ceof0 %f\n\t C %f, nu %f, p %f\n Training ..",
                params.degree, params.gamma, params.coef0, params.C, params.nu, params.p);
#else
        // otherwise use regular training and use parameters manually specified above
        svm->train(trainingData, labels, Mat(), Mat(), params);

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

        printf( "\nUsing testing database: \n");
        for (int tsample = 0; tsample < NUMBER_OF_TESTING_SAMPLES; tsample++)
        {
			
            // extract a row from the testing matrix
            test_sample = evalData.row(tsample);

            // run SVM classifier
            result = svm->predict(test_sample);

            printf("Testing Sample %i -> class result (character %d)\n", tsample,  ClassTable[((int) result)]);
            // if the prediction and the (true) testing classification are the same
            // (N.B. openCV uses a floating point decision tree implementation!)

            if (fabs(result - groundTruth.at<float>(tsample, 0))
                    >= FLT_EPSILON)
            {
                // if they differ more than floating point error => wrong class
                wrong_class++;
                false_positives[(int) (groundTruth.at<float>(tsample, 0))]++;
            }
            else
            {
                // otherwise correct
                correct_class++;
            }
        }

        printf( "\nResults on the testing database: \n"
                "\tCorrect classification: %d (%g%%)\n"
                "\tWrong classifications: %d (%g%%)\n",
                correct_class, (double) correct_class*100/NUMBER_OF_TESTING_SAMPLES,
                wrong_class, (double) wrong_class*100/NUMBER_OF_TESTING_SAMPLES);

        for (unsigned char i = 0; i < NUMBER_OF_CLASSES; i++)
        {
            printf( "\tClass (character %d) false positives 	%d (%g%%)\n",ClassTable[(int) i],
                    false_positives[(int) i],
                    (double) false_positives[i]*100/NUMBER_OF_TESTING_SAMPLES);
        }

        // all matrix memory free by destructors
        // all OK : main returns 0
        return 0;
    }

    // not OK : main returns -1
    return -1;
}	

int DecisionTreeClassifier( Mat trainingData, Mat labels, Mat evalData, Mat groundTruth, int NumberOfClasses , int* ClassTable) 
{

			printf( "\n\n........................................................\n ");
			printf( "		DECISION TREE CLASSIFIER 	\n ");
			printf( "........................................................\n ");

	int i;
	int NUMBER_OF_CLASSES = NumberOfClasses;
	
    // define all the attributes as numerical
    // alternatives are CV_VAR_CATEGORICAL or CV_VAR_ORDERED(=CV_VAR_NUMERICAL)
    // that can be assigned on a per attribute basis
    Mat var_type = Mat(dictionarySize + 1, 1, CV_8U );
    var_type.setTo(Scalar(CV_VAR_NUMERICAL) ); // all inputs are numerical

    // this is a classification problem (i.e. predict a discrete number of class
    // outputs) so reset the last (+1) output var_type element to CV_VAR_CATEGORICAL
    var_type.at<uchar>(dictionarySize, 0) = CV_VAR_CATEGORICAL;
	
    CvDTreeNode* resultNode; // node returned from a prediction

    // load training and testing data sets
    while(1)
    {
        // define the parameters for training the decision tree
        float *priors = NULL;  // weights of each classification for classes
        // (all equal as equal samples of each character)

        CvDTreeParams params = CvDTreeParams(25, // max depth
                                             5, // min sample count
                                             0, // regression accuracy: N/A here
                                             false, // compute surrogate split, no missing data
                                             15, // max number of categories (use sub-optimal algorithm for larger numbers)
                                             10, // the number of cross-validation folds
                                             false, // use 1SE rule => smaller tree
                                             false, // throw away the pruned tree branches
                                             priors // the array of priors
                                            );


        // train decision tree classifier (using training data)

        printf( "\nUsing training database: \n" );
        CvDTree* dtree = new CvDTree;

        dtree->train(trainingData, CV_ROW_SAMPLE, labels,
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

        printf( "\nUsing testing database: \n");

        for (int tsample = 0; tsample < NUMBER_OF_TESTING_SAMPLES; tsample++)
        {
            // extract a row from the testing matrix
            test_sample = evalData.row(tsample);

            // run decision tree prediction
            resultNode = dtree->predict(test_sample, Mat(), false);

            printf("Testing Sample %i -> class result (character %d)\n", tsample,
                   ClassTable[((int) (resultNode->value))]);

            // if the prediction and the (true) testing classification are the same
            // (N.B. openCV uses a floating point decision tree implementation!)

            if (fabs(resultNode->value - groundTruth.at<float>(tsample, 0))
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

        printf( "\nResults on the testing database: \n"
                "\tCorrect classification: %d (%g%%)\n"
                "\tWrong classifications: %d (%g%%)\n",
                correct_class, (double) correct_class*100/NUMBER_OF_TESTING_SAMPLES,
                wrong_class, (double) wrong_class*100/NUMBER_OF_TESTING_SAMPLES);

        for (int i = 0; i < NUMBER_OF_CLASSES; i++)
        {
            printf( "\tClass (character %d) false postives 	%d (%g%%)\n", ClassTable[i],
                    false_positives[i],
                    (double) false_positives[i]*100/NUMBER_OF_TESTING_SAMPLES);
        }
        // all matrix memory free by destructors
        // all OK : main returns 0
        return 0;
    }
    // not OK : main returns -1
    return -1;
}
 
int main(int argc, char ** argv)
{

	int Training_Images = NUMBER_OF_TRAINING_SAMPLES;
	int Testing_Images  = NUMBER_OF_TESTING_SAMPLES;
	int choice = 0;
	
	int numberOfClasses;
	
	cout << "----- Main Menu -----" << endl;
	cout << "Classify Testing Pages \n" 
		 << " '1' to check the Page Aesthetic. \n"
		 << " '2' to check the Page Recency. \n"
		 << " '3' to check the Page Topic (4 classes). \n"
	     << " '4' to check the Page Topic (8 classes). \n";
		
	cout << "Enter a choice: ";
	cin >> choice;
	cout << '\n';

	if ( choice == 1)
	{
		/*			Aesthetic	 			*/
		filePath1 	= "WebPages/Aesthetic/Ugly";
		filePath2 	= "WebPages/Aesthetic/Beautiful";
		filePath3 	= "WebPages/Aesthetic/Ugly";
		filePath4 	= "WebPages/Aesthetic/Beautiful";
		filePath 	= "WebPages/Aesthetic/Testing";
		numberOfClasses = 2;
	}
	else if ( choice == 2 )
	{
		/*			Recency	 			*/
		filePath1 	= "WebPages/Recency/Old";
		filePath2 	= "WebPages/Recency/New";
		filePath3 	= "WebPages/Recency/Old";
		filePath4 	= "WebPages/Recency/New";
		filePath 	= "WebPages/Recency/Testing";
		numberOfClasses = 2;
	}
	else if ( choice == 3 )
	{
		/*			WebPageTopic		*/
		filePath 	= "WebPages/PageTopic/Testing";
		filePath1 	= "WebPages/PageTopic/Newspapers";
		filePath2 	= "WebPages/PageTopic/Conference";
		filePath3 	= "WebPages/PageTopic/Celebrity";
		filePath4 	= "WebPages/PageTopic/Hotel";
		numberOfClasses = 4;
	}	
	else if ( choice == 4 )
	{
		/*			WebPageTopic		*/
		filePath 	= "WebPages/PageTopic/testing8";
		filePath1 	= "WebPages/PageTopic/Newspapers";
		filePath2 	= "WebPages/PageTopic/Conference";
		filePath3 	= "WebPages/PageTopic/Celebrity";
		filePath4 	= "WebPages/PageTopic/Hotel";
		filePath5 	= "WebPages/PageTopic/ClassifiedSites";
		filePath6 	= "WebPages/PageTopic/Gaming";
		filePath7 	= "WebPages/PageTopic/SocialNetworks";
		filePath8 	= "WebPages/PageTopic/Video-Sharing";
		numberOfClasses = 8;
	}
	
	//#if DICTIONARY_BUILD == 1
	vector<Mat> descriptors;
	if(numberOfClasses == 2 || numberOfClasses == 4)
	{
		cout << " Number Of Classes = " << numberOfClasses << endl;
		cout << " Creating dictionary..." << endl;
		extractTrainingVocabulary(filePath1, filePath2, filePath3, filePath4, Training_Images, numberOfClasses);
		descriptors = bowTrainer.getDescriptors();
	}
	else if ( numberOfClasses == 8)
	{
		cout << " Number Of Classes = " << numberOfClasses << endl;
		cout << " Creating dictionary..." << endl;
		extractTrainingVocabulary8(filePath1, filePath2, filePath3, filePath4, filePath5, filePath6, filePath7, filePath8, Training_Images, numberOfClasses);
		descriptors = bowTrainer.getDescriptors();
	}
	
	int count=0;
	for( vector<Mat>::iterator iter = descriptors.begin(); iter != descriptors.end(); iter++ ){
		count+=iter->rows;
	}
	cout << "Clustering " << count << " features" << endl;
	Mat dictionary = bowTrainer.cluster();
	
	/***************************************************/
	// Parte do codigo para gravar o dicionario
	//
	/***************************************************/
	//FileStorage fs("dictionary.yml", FileStorage::WRITE);
	//fs << "vocabulary" << dictionary;
	//fs.release();
	
	//#else
    //Step 2 - Obtain the BoF descriptor for given images. 

    //prepare BOW descriptor extractor from the dictionary    
    /*Mat dictionary; 
    FileStorage fs("dictionary500-8-60.yml", FileStorage::READ);
    fs["vocabulary"] >> dictionary;
    fs.release();*/    
	
	//Set the dictionary with the vocabulary we created in the first step */ */
	bowDE.setVocabulary(dictionary);
	
	cout << "Processing training data..." << endl;
	Mat trainingData(0, dictionarySize, CV_32FC1);
	Mat labels(0, 1, CV_32FC1);
	
	if(numberOfClasses == 2 || numberOfClasses == 4)
		extractBOWDescriptor(filePath1, filePath2, filePath3, filePath4, trainingData, labels, Training_Images, numberOfClasses);
	else if ( numberOfClasses == 8)
		extractBOWDescriptor8(filePath1, filePath2, filePath3, filePath4, filePath5, filePath6, filePath7, filePath8, trainingData, labels, Training_Images, numberOfClasses);

		
	cout << "cols = " << trainingData.cols << " and rows = " << trainingData.rows << endl;
    cout << "Processing evaluation data..." << endl;
	
	Mat evalData(0, dictionarySize, CV_32FC1);
	Mat groundTruth(0, 1, CV_32FC1);
	if(numberOfClasses == 2 || numberOfClasses == 4)
		extractBOWDescriptor(filePath, filePath, filePath3, filePath4, evalData, groundTruth, Testing_Images, numberOfClasses);
	else if ( numberOfClasses == 8)
		extractBOWDescriptor8(filePath, filePath, filePath3, filePath4, filePath5, filePath6, filePath7, filePath8, evalData, groundTruth, Testing_Images, numberOfClasses);

	choice = 0;
	while(1){
		cout << endl << endl;
		cout << "Choose the classifier" << endl;
		cout << " '1' Naive Bayes Classifier. \n"
			 << " '2' Support Vector Machine Classifier. \n"
			 << " '3' Decision Tree Classifier. \n"
			 << " '4' Boost Classifier. \n";

		cout << "Enter a choice: ";
		cin >> choice;
		cout << '\n';
	if(numberOfClasses == 2 || numberOfClasses == 4){
		switch ( choice )
		{
		case 1:
			NaiveBayesClassifier( trainingData, labels, evalData, groundTruth, numberOfClasses, MultiClass );
			break;
		case 2:
			SVMClassifier( trainingData, labels, evalData, groundTruth, numberOfClasses, MultiClass );
			break;
		case 3:
			DecisionTreeClassifier( trainingData, labels, evalData, groundTruth, numberOfClasses, MultiClass );
			break;
		case 4:
			BoostClassifier( trainingData, labels, evalData, groundTruth, numberOfClasses, MultiClass );
			break;
		default:
			cout << "You have entered an invalid choice.\n"
				 << "Please try again.\n\n";
			return 0;
		}
	}else if ( numberOfClasses == 8){
				
		switch ( choice )
		{
		case 1:
			NaiveBayesClassifier( trainingData, labels, evalData, groundTruth, numberOfClasses, MultiClass8 );
			break;
		case 2:
			SVMClassifier( trainingData, labels, evalData, groundTruth, numberOfClasses, MultiClass8 );
			break;
		case 3:
			DecisionTreeClassifier( trainingData, labels, evalData, groundTruth, numberOfClasses, MultiClass8 );
			break;
		case 4:
			BoostClassifier( trainingData, labels, evalData, groundTruth, numberOfClasses, MultiClass8 );
			break;
		default:
			cout << "You have entered an invalid choice.\n"
				 << "Please try again.\n\n";
			return 0;
		}
	}
	}
	//#endif
}
