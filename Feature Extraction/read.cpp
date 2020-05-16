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
#include "ExtractFeatures.h"
using namespace std;
using namespace cv;

//#define mSet(a,i,j) CV_MAT_ELEM((a), float, i, j)

//#define USE_OPENCV_GRID_SEARCH_AUTOTRAIN 1  // set to 0 to set SVM parameters manually
#define NUMBER_OF_TRAINING_SAMPLES 90 
#define ATTRIBUTES_PER_SAMPLE 20 //The first one is the class.
#define NUMBER_OF_TESTING_SAMPLES 20
#define BINARY_CLASS 2
#define MULTI_CLASS 4
#define NUMBER_OF_TOTAL_FEATURES 166

static int BinaryClass[2] = {0, 1};
static int MultiClass[4]  = {0, 1, 2, 3};
static int totalFeat = 166;

int 	read_data_from_csv(const char*, Mat, Mat, int );
void 	Extract(char*, char);
int 	NaiveBayesClassifier( const char*, const char*, int, int* );
int 	SVMClassifier(const char*, const char*, int, int* );			
int 	DecisionTreeClassifier( const char*, const char*, int, int* );
int 	BoostClassifier( const char*, const char*, int, int* );	//Binary Classifier
void 	ProcessImages( const char*, int, char );
int 	AestheticValue( int , int );
int 	WebPageRecency( int , int );
int 	WebPageTopic( int, int );

void plot_binary( Mat&, Mat&, string, int);
void teste();


int main( int argc, char** argv )
{
	int i;
	int TrainingImages = NUMBER_OF_TRAINING_SAMPLES;
	int TestingImages = NUMBER_OF_TESTING_SAMPLES;
	int choice = 0;
	
	cout << "----- Main Menu -----" << endl;
	cout << "Classify Testing Pages \n" 
		 << " '1' to check the Page Aesthetic. \n"
		 << " '2' to check the Page Recency. \n"
		 << " '3' to check the Page Topic. \n"
		 << " '4' teste . \n ";

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
		case 4:
			teste();
		default:
			cout << "You have entered an invalid choice.\n"
				 << "Please try again.\n\n";
			break;
	}
	
}

void teste(){
	
	char 	directory[50];
	char training = 'c';
	char testing = 'k';
	CreateTable( 1, 1);
	const char* filePath 	= "WebPages/PageTopic/Newspapers";
	
	sprintf(directory, "%s/%1d.png", filePath, 2);
	printf(" Processing image %d in %s.		\n", 2, directory);
	Extract( directory, training );
	
	sprintf(directory, "%s/%1d.png", filePath, 4);
	printf(" Processing image %d in %s.		\n", 4, directory);
	Extract( directory, testing );

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
	int TrainingSize = NUMBER_OF_TRAINING_SAMPLES*MULTI_CLASS;
	int TestingSize	 = NUMBER_OF_TESTING_SAMPLES;
	
	const char* filePath1 	= "WebPages/PageTopic/Newspapers";
	const char* filePath2 	= "WebPages/PageTopic/Conference";
	const char* filePath3 	= "WebPages/PageTopic/Celebrity";
	const char* filePath4 	= "WebPages/PageTopic/Hotel";
	const char* filePath5 	= "WebPages/PageTopic/Testing";
	const char* csvname 	= "PageTopic";
	
	const char* PathTrain = "PageTopicTraining.csv";
	const char* PathTest  = "PageTopicTesting.csv";
	
	char training = 'c';
	char testing = 'k';
		
 	CreateTable( TrainingSize, TestingSize );
 	ProcessImages(filePath1, TrainingImages, training);
	ProcessImages(filePath2, TrainingImages, training);
	ProcessImages(filePath3, TrainingImages, training);
	ProcessImages(filePath4, TrainingImages, training);
	doPCA( ATTRIBUTES_PER_SAMPLE, TrainingSize, MULTI_CLASS, csvname, training );
	//ChiSquare(training, csvname, MULTI_CLASS); 
	ProcessImages(filePath5, TestingImages, testing);
	doPCA( ATTRIBUTES_PER_SAMPLE, TestingSize, MULTI_CLASS, csvname, testing );
	//ChiSquare(testing, csvname, MULTI_CLASS);
		int choice = 0;
	while(1){
		cout << endl << endl;
		cout << "Choose the classifier" << endl;
		cout << " '1' Naive Bayes Classifier. \n"
			 << " '2' Support Vector Machine Classifier. \n"
			 << " '3' Decision Tree Classifier. \n";

		cout << "Enter a choice: ";
		cin >> choice;
		cout << '\n';

		switch ( choice )
		{
		case 1:
			NaiveBayesClassifier(PathTrain, PathTest, MULTI_CLASS, MultiClass);
			break;
		case 2:
			SVMClassifier(PathTrain, PathTest, MULTI_CLASS, MultiClass);			
			break;
		case 3:
			DecisionTreeClassifier(PathTrain, PathTest, MULTI_CLASS, MultiClass);		
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
 	int TrainingSize = NUMBER_OF_TRAINING_SAMPLES*BINARY_CLASS;
	int TestingSize	 = NUMBER_OF_TESTING_SAMPLES;
	
	const char* filePath1 	= "WebPages/Recency/Old";
	const char* filePath2 	= "WebPages/Recency/New";
	const char* filePath3 	= "WebPages/Recency/Testing10";
	const char* csvname 	= "Recency";
	char training = 'c';
	char testing = 'k';
	
	CreateTable( TrainingSize, TestingSize );
	ProcessImages(filePath1, TrainingImages, training);
	ProcessImages(filePath2, TrainingImages, training);
	//doPCA( ATTRIBUTES_PER_SAMPLE, TrainingSize, BINARY_CLASS, csvname, training );
	ChiSquare(training, csvname, BINARY_CLASS); 
	ProcessImages(filePath3, TestingImages, testing);
	//doPCA( ATTRIBUTES_PER_SAMPLE, TestingSize, BINARY_CLASS, csvname, testing );
	ChiSquare(testing, csvname, BINARY_CLASS);  

	const char* PathTrain = "RecencyTraining.csv";
	const char* PathTest  = "RecencyTesting.csv";
	
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
			NaiveBayesClassifier(PathTrain, PathTest, BINARY_CLASS, BinaryClass);
			break;
		case 2:
			SVMClassifier(PathTrain, PathTest, BINARY_CLASS, BinaryClass);
			break;
		case 3:
			DecisionTreeClassifier(PathTrain, PathTest, BINARY_CLASS, BinaryClass);
			break;
		case 4:
			BoostClassifier(PathTrain, PathTest, BINARY_CLASS, BinaryClass);
			break;
		default:
			cout << "You have entered an invalid choice.\n"
				 << "Please try again.\n\n";
			return 0;
		}
	}
	
}

int AestheticValue( int TrainingImages, int TestingImages )
{
	
	/*
		Aesthetic Value:
			- Ugly Webpages
			- Beautiful WebPages	
	*/
 	int TrainingSize = NUMBER_OF_TRAINING_SAMPLES*BINARY_CLASS;
	int TestingSize	 = NUMBER_OF_TESTING_SAMPLES;
	
	const char* filePath1 	= "WebPages/Aesthetic/Ugly";
	const char* filePath2 	= "WebPages/Aesthetic/Beautiful";
	const char* filePath3 	= "WebPages/Aesthetic/Testing";
	const char* csvname 	= "Aesthetic";
 	char training 	= 'c';
	char testing 	= 'k';
	
 	CreateTable( TrainingSize, TestingSize );
	ProcessImages( filePath1, TrainingImages, training );
	ProcessImages( filePath2, TrainingImages, training );
	doPCA( ATTRIBUTES_PER_SAMPLE, TrainingSize, BINARY_CLASS, csvname, training );
	//ChiSquare( training, csvname, BINARY_CLASS ); 
	ProcessImages( filePath3, TestingImages, testing ); 
	//ChiSquare( testing, csvname, BINARY_CLASS ); 
	doPCA( ATTRIBUTES_PER_SAMPLE, TestingSize, BINARY_CLASS, csvname, testing );	
	
	const char* PathTrain = "AestheticTraining.csv";
	const char* PathTest  = "AestheticTesting.csv";
	
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
			NaiveBayesClassifier(PathTrain, PathTest, BINARY_CLASS, BinaryClass);
			break;
		case 2:
			SVMClassifier(PathTrain, PathTest, BINARY_CLASS, BinaryClass);
			break;
		case 3:
			DecisionTreeClassifier(PathTrain, PathTest, BINARY_CLASS, BinaryClass);
			break;
		case 4:
			BoostClassifier(PathTrain, PathTest, BINARY_CLASS, BinaryClass);
			break;
		default:
			cout << "You have entered an invalid choice.\n"
				 << "Please try again.\n\n";
			return 0;
		}
	}
	
}

void ProcessImages(const char* filePath, int nImages, char flag )
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

int BoostClassifier(const char* train, const char* test, int NumberOfClasses , int* ClassTable )
{
			printf( "\n");
			printf( "........................................................\n ");
			printf( "		BOOOST CLASSIFIER 	\n ");
			printf( "........................................................\n ");

	int i;
	int NUMBER_OF_CLASSES = NumberOfClasses;
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
        return 0;
	}	
	// not OK : main returns -1
    return -1;
	
} 

int DecisionTreeClassifier(const char* train, const char* test, int NumberOfClasses, int* ClassTable ) 
{
			printf( "\n");
			printf( "........................................................\n ");
			printf( "		DECISION TREE CLASSIFIER 	\n ");
			printf( "........................................................\n ");

	int i;
	int NUMBER_OF_CLASSES = NumberOfClasses;
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

int SVMClassifier(const char* train, const char* test, int NumberOfClasses, int* ClassTable )
{	
					printf( "\n");
					printf( "........................................................\n ");
					printf( "		SUPPORT VECTOR MACHINE CLASSIFIER 	\n ");
					printf( "........................................................\n ");

	int i;
	int NUMBER_OF_CLASSES = NumberOfClasses;
    // define training data storage matrices (one for attribute examples, one for classifications)
    Mat training_data = Mat(NUMBER_OF_TRAINING_SAMPLES*NUMBER_OF_CLASSES, ATTRIBUTES_PER_SAMPLE, CV_32FC1);
    Mat training_classifications = Mat(NUMBER_OF_TRAINING_SAMPLES*NUMBER_OF_CLASSES, 1, CV_32FC1);

    //define testing data storage matrices
    Mat testing_data = Mat(NUMBER_OF_TESTING_SAMPLES, ATTRIBUTES_PER_SAMPLE, CV_32FC1);
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

int NaiveBayesClassifier(const char* train, const char* test, int NumberOfClasses, int *ClassTable )
{
			printf( "\n");
			printf( "........................................................\n ");
			printf( "		NAIVE BAYES CLASSIFIER 		\n ");
			printf( ".........................................................\n ");
	int i;
	int NUMBER_OF_CLASSES = NumberOfClasses;
	//	define training data storage matrices (one for attribute examples, one for classifications)
    Mat training_data = Mat(NUMBER_OF_TRAINING_SAMPLES*NUMBER_OF_CLASSES, ATTRIBUTES_PER_SAMPLE, CV_32F);
    Mat training_classifications = Mat(NUMBER_OF_TRAINING_SAMPLES*NUMBER_OF_CLASSES, 1, CV_32F);
	
    // define testing data storage matrices
    Mat testing_data = Mat(NUMBER_OF_TESTING_SAMPLES, ATTRIBUTES_PER_SAMPLE, CV_32F);
    Mat testing_classifications = Mat(NUMBER_OF_TESTING_SAMPLES, 1, CV_32F);
	
	if ((read_data_from_csv(train, training_data, training_classifications, NUMBER_OF_TRAINING_SAMPLES*NUMBER_OF_CLASSES) &&
            read_data_from_csv(test, testing_data, testing_classifications, NUMBER_OF_TESTING_SAMPLES)))
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
        return 0;
	}
	
	// not OK : main returns -1
    return -1;
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

// plot data and class
/* void plot_binary( Mat& data, Mat& classes, string name, int size) {
    Mat plot(size, size, CV_8UC3);
    plot.setTo(cv::Scalar(255.0,255.0,255.0));
	namedWindow(name, 0 );
    for(int i = 0; i < data.rows; i++) {

        float x = data.at<float>(i,0) * size;
        float y = data.at<float>(i,1) * size;

        if(classes.at<float>(i, 0) > 0) {
            circle(plot, Point(x,y), 2, CV_RGB(255,0,0),1);
        } else {
            circle(plot, Point(x,y), 2, CV_RGB(0,255,0),1);
        }
    }
    imshow(name, plot);
	resizeWindow(name, 300, 300);
} */

/*
	Extract the array of (32+80+18+36) = 166 variables
	that characterize the image of the page.
	And save it in a %s.csv file, depending of witch
	class belongs. 
		- Beatiful or Ugly
		- Old or New
		- Web Page Topic
*/
void Extract( char* filePath, char type )
{
	int i, j;
	
	IplImage *imageRGB = NULL; 
	imageRGB = cvLoadImage(filePath, CV_LOAD_IMAGE_COLOR);

	if(!imageRGB){ 
		printf("Error loading image\n");
	}
	int height	= imageRGB->height;
 	int width 	= imageRGB->width;
	if(width > 1024)	width = 1024;
/* 	// else if( (width < 4096) && (width >3840) ) width = 3840;
	// else if( (width < 3840) && (width >3584) ) width = 3584;
	// else if( (width < 3584) && (width >3328) ) width = 3328;
	// else if( (width < 3328) && (width >3072) ) width = 3072;
	// else if( (width < 3072) && (width >2816) ) width = 2816;
	// else if( (width < 2816) && (width >2560) ) width = 2560;
	// else if( (width < 2560) && (width >2304) ) width = 2304;
	// else if( (width < 2304) && (width >2048) ) width = 2048; 
	else if( (width < 2048) && (width >1920) ) width = 1920;
	else if( (width < 1920) && (width >1792) ) width = 1792;
	else if( (width < 1792) && (width >1664) ) width = 1664;
	else if( (width < 1664) && (width >1536) ) width = 1536;
	else if( (width < 1536) && (width >1408) ) width = 1408;
	else if( (width < 1408) && (width >1280) ) width = 1280;
	else if( (width < 1280) && (width >1152) ) width = 1152;
	else if( (width < 1152) && (width >1024) ) width = 1024;*/
	else if( (width < 1024) && (width >896) ) width = 896;
	else if( (width < 896) && (width >768) ) width = 768;
	else if( (width < 768) && (width >640) ) width = 640;
	else if( (width < 640) && (width >512) ) width = 512;
	
	if(height > 1024)	height = 1024;
/* 	// else if( (height < 4096) && (height >3840) ) height = 3840;
	// else if( (height < 3840) && (height >3584) ) height = 3584;
	// else if( (height < 3584) && (height >3328) ) height = 3328;
	// else if( (height < 3328) && (height >3072) ) height = 3072;
	// else if( (height < 3072) && (height >2816) ) height = 2816;
	// else if( (height < 2816) && (height >2560) ) height = 2560;
	// else if( (height < 2560) && (height >2304) ) height = 2304;
	// else if( (height < 2304) && (height >2048) ) height = 2048;
	else if( (height < 2048) && (height >1920) ) height = 1920;
	else if( (height < 1920) && (height >1792) ) height = 1792;
	else if( (height < 1792) && (height >1664) ) height = 1664; 
	else if( (height < 1664) && (height >1536) ) height = 1536;
	else if( (height < 1536) && (height >1408) ) height = 1408;
	else if( (height < 1408) && (height >1280) ) height = 1280;
	else if( (height < 1280) && (height >1152) ) height = 1152;
	else if( (height < 1152) && (height >1024) ) height = 1024;*/
	else if( (height < 1024) && (height >896) ) height = 896;
	else if( (height < 896) && (height >768) ) height = 768;
	else if( (height < 768) && (height >640) ) height = 640;
	else if( (height < 640) && (height >512) ) height = 512;
	IplImage *newImage = NULL;
	newImage = cvCreateImage(cvSize(width, height), imageRGB->depth, imageRGB->nChannels);
	CvScalar s;
	for( i = 0; i < width; i++){
		for( j = 0; j < height; j++){
			s=cvGet2D(imageRGB,j,i); // get the (i,j) pixel value
			cvSet2D(newImage,j,i,s); // set the (i,j) pixel value
		}
	}
	cvReleaseImage(&imageRGB);
	
	/* FUNCTION TO EXTRACT FEATURES*/
	ExtractFeatures( newImage, type );	
	cvReleaseImage(&newImage);
	return;
}