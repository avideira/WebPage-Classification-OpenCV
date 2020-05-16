#include <cv.h>       
#include <ml.h>		
#include <highgui.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include "LBPGist.h"
#include "lbp.hpp"

using namespace std;
using namespace cv;

//#define USE_OPENCV_GRID_SEARCH_AUTOTRAIN 1  // set to 0 to set SVM parameters manually
#define BINARY_CLASS 2
#define MULTI_CLASS 4
#define FEATS_REDUCTION 5

static int BinaryClass[2] = {0, 1};
static int MultiClass[4]  = {0, 1, 2, 3};

int orientations = 8;
int scales = 4;

int 	BoostClassifier			( const char* , const char* , int , int* , int , int , int );
int 	DecisionTreeClassifier	( const char* , const char* , int , int* , int , int , int );
int 	SVMClassifier			( const char* , const char* , int , int* , int , int , int );
int 	NaiveBayesClassifier	( const char* , const char* , int , int* , int , int , int );
int 	read_data_from_csv		( const char* , Mat , Mat , int );
void 	ProcessImages			( const char* , int , char );
int 	AestheticValue			( int , int );
int 	WebPageRecency			( int , int );
void 	Extract					( char* , char );

IplImage *imageRGB, *gray;

int main( int argc, char** argv )
{
	int i;
	int TrainingImages = 30;
	int TestingImages = 10;
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
		//case 3:
			// WebPageTopic(TrainingImages, (TestingImages+10));
			// break;
		default:
			cout << "You have entered an invalid choice.\n"
				 << "Please try again.\n\n";
			break;
	}
}

int AestheticValue( int TrainingImages, int TestingImages )
{
	/*
		Aesthetic Value:
			- Ugly Webpages
			- Beautiful WebPages	
	*/
 	int Training_Samples = TrainingImages*BINARY_CLASS;
	int Testing_Samples = TestingImages;
	
	const char* filePath1 	= "WebPages/Aesthetic/Ugly";
	const char* filePath2 	= "WebPages/Aesthetic/Beautiful";
	const char* filePath3 	= "WebPages/Aesthetic/Testing";
	const char* csvname 	= "AestheticGist";
 	char training 	= 'c';
	char testing 	= 'k';
	
 	CreateTable( Training_Samples, Testing_Samples, scales, orientations );
	ProcessImages( filePath1, TrainingImages, training );
	ProcessImages( filePath2, TrainingImages, training );
	//doPCA(FEATS_REDUCTION, Training_Samples, scales, orientations, BINARY_CLASS, csvname, training );
	ChiSquare( FEATS_REDUCTION, Training_Samples, scales, orientations, BINARY_CLASS, csvname, training );
	
	ProcessImages( filePath3, TestingImages, testing );
	ChiSquare( FEATS_REDUCTION, /*Testing_Samples*/2, scales, orientations, BINARY_CLASS, csvname, testing  );
	//doPCA(FEATS_REDUCTION, Testing_Samples, scales, orientations, BINARY_CLASS, csvname, testing ); 
	
	const char* PathTrain = "AestheticGistTraining.csv";
	const char* PathTest  = "AestheticGistTesting.csv";
	
 	int choice = 0;
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

		switch ( choice )
		{
		case 1:
			NaiveBayesClassifier( PathTrain, PathTest, BINARY_CLASS, BinaryClass, FEATS_REDUCTION, TrainingImages, TestingImages );
			break;
 		case 2:
			SVMClassifier( PathTrain, PathTest, BINARY_CLASS, BinaryClass, FEATS_REDUCTION, TrainingImages, TestingImages );
			break;
		case 3:
			DecisionTreeClassifier( PathTrain, PathTest, BINARY_CLASS, BinaryClass, FEATS_REDUCTION, TrainingImages, TestingImages );
			break;
		case 4:
			BoostClassifier( PathTrain, PathTest, BINARY_CLASS, BinaryClass, FEATS_REDUCTION, TrainingImages, TestingImages );
			break;
		default:
			cout << "You have entered an invalid choice.\n"
				 << "Please try again.\n\n";
			return 0;
		}
	} 
	
}	

int WebPageRecency( int TrainingImages, int TestingImages )
{
	/*
		Recency:
			- New Webpages
			- Old WebPages	
	*/
	int Training_Samples = TrainingImages*BINARY_CLASS;
	int Testing_Samples = TestingImages;
	
	const char* filePath1 	= "WebPages/Recency/Old";
	const char* filePath2 	= "WebPages/Recency/New";
	const char* filePath3 	= "WebPages/Recency/Testing";
	const char* csvname 	= "RecencyGist";
	char training = 'c';
	char testing = 'k';
	
 	CreateTable( Training_Samples, Testing_Samples, scales, orientations );
	ProcessImages( filePath1, TrainingImages, training );
	ProcessImages( filePath2, TrainingImages, training );
	//doPCA(FEATS_REDUCTION, Training_Samples, scales, orientations, BINARY_CLASS, csvname, training );
	ChiSquare( FEATS_REDUCTION, Training_Samples, scales, orientations, BINARY_CLASS, csvname, training );
	ProcessImages( filePath3, TestingImages, testing );
	ChiSquare( FEATS_REDUCTION, /*Testing_Samples*/2, scales, orientations, BINARY_CLASS, csvname, testing  );
	//doPCA(FEATS_REDUCTION, Testing_Samples, scales, orientations, BINARY_CLASS, csvname, testing ); 

	const char* PathTrain = "RecencyGistTraining.csv";
	const char* PathTest  = "RecencyGistTesting.csv";
	
	int choice = 0;
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

		switch ( choice )
		{
		case 1:
			NaiveBayesClassifier( PathTrain, PathTest, BINARY_CLASS, BinaryClass, FEATS_REDUCTION, TrainingImages, TestingImages );
			break;
 		case 2:
			SVMClassifier( PathTrain, PathTest, BINARY_CLASS, BinaryClass, FEATS_REDUCTION, TrainingImages, TestingImages );
			break;
		case 3:
			DecisionTreeClassifier( PathTrain, PathTest, BINARY_CLASS, BinaryClass, FEATS_REDUCTION, TrainingImages, TestingImages );
			break;
		case 4:
			BoostClassifier( PathTrain, PathTest, BINARY_CLASS, BinaryClass, FEATS_REDUCTION, TrainingImages, TestingImages );
			break;
		default:
			cout << "You have entered an invalid choice.\n"
				 << "Please try again.\n\n";
			return 0;
		}
	}
	
}

int NaiveBayesClassifier( const char* train, const char* test, int NumberOfClasses, int *ClassTable, int NumberofFeats, 
							int NUMBER_OF_TRAINING_SAMPLES, int NUMBER_OF_TESTING_SAMPLES )
{
			printf( "\n");
			printf( "........................................................\n ");
			printf( "		NAIVE BAYES CLASSIFIER 		\n ");
			printf( ".........................................................\n ");
	int i;
	int NUMBER_OF_CLASSES = NumberOfClasses;
	//	define training data storage matrices (one for attribute examples, one for classifications)
    Mat training_data = Mat(NUMBER_OF_TRAINING_SAMPLES*NUMBER_OF_CLASSES, NumberofFeats, CV_32F);
    Mat training_classifications = Mat(NUMBER_OF_TRAINING_SAMPLES*NUMBER_OF_CLASSES, 1, CV_32F);
	
    // define testing data storage matrices
    Mat testing_data = Mat(NUMBER_OF_TESTING_SAMPLES, NumberofFeats, CV_32F);
    Mat testing_classifications = Mat(NUMBER_OF_TESTING_SAMPLES, 1, CV_32F);
	
	if ((read_data_from_csv(train, training_data, training_classifications, NUMBER_OF_TRAINING_SAMPLES*NUMBER_OF_CLASSES) &&
            read_data_from_csv(test, testing_data, testing_classifications, NUMBER_OF_TESTING_SAMPLES)))
    {
		//cout << "Training data = " << "  " << training_data << endl;
		//cout << "Testing data = " << "  " << testing_data << endl;
		//cout << "Testing classifications = " << "  " << testing_classifications << endl;
		// train bayesian classifier (using training data)
        printf( "\nUsing training database: %s\n", train);
		CvNormalBayesClassifier* bayes = new CvNormalBayesClassifier();
		bayes->train(training_data,training_classifications,Mat(),Mat(),false);//MAT()
		
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
        return 0;
	}	
	// not OK : main returns -1
    return -1;
}

int BoostClassifier( const char* train, const char* test, int NumberOfClasses , int* ClassTable, int NumberofFeats, 
							int NUMBER_OF_TRAINING_SAMPLES, int NUMBER_OF_TESTING_SAMPLES )
{
			printf( "\n");
			printf( "........................................................\n ");
			printf( "		BOOOST CLASSIFIER 	\n ");
			printf( "........................................................\n ");

	int i;
	int NUMBER_OF_CLASSES = NumberOfClasses;
	//	define training data storage matrices (one for attribute examples, one for classifications)
    Mat training_data = Mat(NUMBER_OF_TRAINING_SAMPLES*NUMBER_OF_CLASSES, NumberofFeats, CV_32F);
    Mat training_classifications = Mat(NUMBER_OF_TRAINING_SAMPLES*NUMBER_OF_CLASSES, 1, CV_32F);
	
    // define testing data storage matrices
    Mat testing_data = Mat(NUMBER_OF_TESTING_SAMPLES, NumberofFeats, CV_32F);
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
        return 0;
	}	
	// not OK : main returns -1
    return -1;
	
} 

int DecisionTreeClassifier( const char* train, const char* test, int NumberOfClasses, int* ClassTable, int NumberofFeats, 
							int NUMBER_OF_TRAINING_SAMPLES, int NUMBER_OF_TESTING_SAMPLES ) 
{
			printf( "\n");
			printf( "........................................................\n ");
			printf( "		DECISION TREE CLASSIFIER 	\n ");
			printf( "........................................................\n ");

	int i;
	int NUMBER_OF_CLASSES = NumberOfClasses;
	Mat training_data = Mat(NUMBER_OF_TRAINING_SAMPLES*NUMBER_OF_CLASSES, NumberofFeats, CV_32FC1);
    Mat training_classifications = Mat(NUMBER_OF_TRAINING_SAMPLES*NUMBER_OF_CLASSES, 1, CV_32FC1);

    //define testing data storage matrices

    Mat testing_data = Mat(NUMBER_OF_TESTING_SAMPLES, NumberofFeats, CV_32FC1);
    Mat testing_classifications = Mat(NUMBER_OF_TESTING_SAMPLES, 1, CV_32FC1);

    // define all the attributes as numerical
    // alternatives are CV_VAR_CATEGORICAL or CV_VAR_ORDERED(=CV_VAR_NUMERICAL)
    // that can be assigned on a per attribute basis

    Mat var_type = Mat(NumberofFeats + 1, 1, CV_8U );
    var_type.setTo(Scalar(CV_VAR_NUMERICAL) ); // all inputs are numerical

    // this is a classification problem (i.e. predict a discrete number of class
    // outputs) so reset the last (+1) output var_type element to CV_VAR_CATEGORICAL
    var_type.at<uchar>(NumberofFeats, 0) = CV_VAR_CATEGORICAL;
	
    CvDTreeNode* resultNode; // node returned from a prediction

    // load training and testing data sets
    if (read_data_from_csv(train, training_data, training_classifications, NUMBER_OF_TRAINING_SAMPLES*NUMBER_OF_CLASSES) &&
            read_data_from_csv(test, testing_data, testing_classifications, NUMBER_OF_TESTING_SAMPLES))
    {
        // define the parameters for training the decision tree
        float *priors = NULL;  // weights of each classification for classes
        // (all equal as equal samples of each character)

        CvDTreeParams params = CvDTreeParams(  		8, // max depth  
												 10, // min sample count  
												  0, // regression accuracy: N/A here  
											   true, // compute surrogate split,   
													 //   as we have missing data  
												 15, // max number of categories   
													 //   (use sub-optimal algorithm for  
													 //   larger numbers)  
												 5, // cross-validations   
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
        return 0;
    }
    // not OK : main returns -1
    return -1;
}

int SVMClassifier( const char* train, const char* test, int NumberOfClasses, int* ClassTable, int NumberofFeats, 
							int NUMBER_OF_TRAINING_SAMPLES, int NUMBER_OF_TESTING_SAMPLES )
{	
					printf( "\n");
					printf( "........................................................\n ");
					printf( "		SUPPORT VECTOR MACHINE CLASSIFIER 	\n ");
					printf( "........................................................\n ");

	int i;
	int NUMBER_OF_CLASSES = NumberOfClasses;
    // define training data storage matrices (one for attribute examples, one for classifications)
    Mat training_data = Mat(NUMBER_OF_TRAINING_SAMPLES*NUMBER_OF_CLASSES, NumberofFeats, CV_32FC1);
    Mat training_classifications = Mat(NUMBER_OF_TRAINING_SAMPLES*NUMBER_OF_CLASSES, 1, CV_32FC1);

    //define testing data storage matrices
    Mat testing_data = Mat(NUMBER_OF_TESTING_SAMPLES, NumberofFeats, CV_32FC1);
    Mat testing_classifications = Mat(NUMBER_OF_TESTING_SAMPLES, 1, CV_32FC1);

    // load training and testing data sets
    if (read_data_from_csv(train, training_data, training_classifications, NUMBER_OF_TRAINING_SAMPLES*NUMBER_OF_CLASSES) &&
            read_data_from_csv(test, testing_data, testing_classifications, NUMBER_OF_TESTING_SAMPLES))
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
        return 0;
    }

    // not OK : main returns -1
    return -1;
}

void ProcessImages( const char* filePath, int nImages, char flag )
{
	int 	i;
	char 	directory[50];
	for( i = 1; i <= nImages; i++ )
	{
		sprintf(directory, "%s/%1d.png", filePath, i);
		printf(" Processing image %d in %s.		\n", i, directory);
		Extract( directory, flag );
	}
	
}

void Extract( char* filePath, char type )
{
	int i, j;
	
	imageRGB  = cvLoadImage(filePath, 1);
	if(!imageRGB){ 
		cout << "Error opening image file named: " << filePath << "\n";
		assert(imageRGB);
	}
	
	gray = cvCreateImage(cvGetSize(imageRGB), IPL_DEPTH_8U, 1);
	cvCvtColor(imageRGB, gray, CV_BGR2GRAY );
	Mat Ogray(gray);
	Mat LBPimage;
	
	printf("Applying the LBP operator to image %s.\n", filePath);
	int radius = 1;
	int neighbors = 8;
	lbp::ELBP(Ogray, LBPimage, radius, neighbors);
	// now to show the patterns a normalization is necessary
    // a simple min-max norm will do the job...
    normalize(LBPimage, LBPimage, 0, 255, NORM_MINMAX, CV_8UC1);
		
	IplImage copy = LBPimage;
	IplImage* dest = &copy;
	
/* 	cvNamedWindow("EXTENDED LBP", CV_WINDOW_AUTOSIZE);
	cvShowImage("EXTENDED LBP", dest);
	cvWaitKey(0);
	cvDestroyWindow("EXTENDED LBP"); */
	
	ExtractGISTFeatures( dest, scales, orientations, type );
	
	cvReleaseImage(&imageRGB);
	cvReleaseImageHeader(&gray);
	//cvReleaseImageHeader(&dest);

}

int read_data_from_csv( const char* filename, Mat data, Mat classes, int n_samples )
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
					classes.at<float>(linenum,0) = 0.0;
                }else if ( value == 1){ 
					classes.at<float>(linenum,0) = 1.0;
				}else if ( value == 2){ 
					classes.at<float>(linenum,0) = 2.0;
				}else if ( value == 3){ 
					classes.at<float>(linenum,0) = 3.0;
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