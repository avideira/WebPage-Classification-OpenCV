/*
This file is Implementation of OpenCV SVM tutorial program.

Implementation of OpenCV SVM tutorial is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Implementation of ARFL is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Implementation of OpenCV SVM tutorial.  If not, see <http://www.gnu.org/licenses/>.

*/
//author Behzad Tabibian
#include "ml.h"
#include "highgui.h"
#define TRAIN_SAMPLE_COUNT 150
#define SIGMA 60
int main( int argc, char** argv )
{
	//Setup Matrices for TrainData set and Class Labels.
	//Most of OpenCV Machine Learning algorithms accept CV_32FC1 matrix format as their input/ouput
	CvMat* trainClass=cvCreateMat(TRAIN_SAMPLE_COUNT,1,CV_32FC1);
	CvMat* trainData=cvCreateMat(TRAIN_SAMPLE_COUNT,2,CV_32FC1);
	//Creating a image to represent outputs
	IplImage* frame  = cvCreateImage(cvSize(500,500), IPL_DEPTH_8U, 3);
	//a vector to use for predicting data
	CvMat* sample=cvCreateMat(1,2,CV_32FC1);

	//Setting up Train Data
	CvMat subtrainData;
	cvGetRows(trainData,&subtrainData,0,TRAIN_SAMPLE_COUNT/3);

	CvMat trainData_col;
	cvGetCols(&subtrainData,&trainData_col,0,1);
	CvRNG rng_state = cvRNG(-1);
	cvRandArr(&rng_state,&trainData_col,CV_RAND_NORMAL,cvScalar(100),cvScalar(SIGMA));
	cvGetCols(&subtrainData,&trainData_col,1,2);
	cvRandArr(&rng_state,&trainData_col,CV_RAND_NORMAL,cvScalar(300),cvScalar(SIGMA));


	cvGetRows(trainData,&subtrainData,TRAIN_SAMPLE_COUNT/3,2*TRAIN_SAMPLE_COUNT/3);
	cvRandArr(&rng_state,&subtrainData,CV_RAND_NORMAL,cvScalar(400),cvScalar(SIGMA));

	cvGetRows(trainData,&subtrainData,2*TRAIN_SAMPLE_COUNT/3,TRAIN_SAMPLE_COUNT);

	cvGetCols(&subtrainData,&trainData_col,0,1);
	cvRandArr(&rng_state,&trainData_col,CV_RAND_NORMAL,cvScalar(300),cvScalar(SIGMA));
	cvGetCols(&subtrainData,&trainData_col,1,2);
	cvRandArr(&rng_state,&trainData_col,CV_RAND_NORMAL,cvScalar(100),cvScalar(SIGMA));

	//Setting up train classes
	CvMat subclassData;
	cvGetRows(trainClass,&subclassData,0,TRAIN_SAMPLE_COUNT/3);
	cvSet(&subclassData,cvScalar(1));
	cvGetRows(trainClass,&subclassData,TRAIN_SAMPLE_COUNT/3,2*TRAIN_SAMPLE_COUNT/3);
	cvSet(&subclassData,cvScalar(2));
	cvGetRows(trainClass,&subclassData,2*TRAIN_SAMPLE_COUNT/3,TRAIN_SAMPLE_COUNT);
	cvSet(&subclassData,cvScalar(3));
	//Setting up SVM parameters
	CvSVMParams params;
	params.kernel_type=CvSVM::LINEAR;
	params.svm_type=CvSVM::C_SVC;
	params.C=1;
	params.term_crit=cvTermCriteria(CV_TERMCRIT_ITER,100,0.000001);
	CvSVM svm;

	//Training the model
	bool res=svm.train(trainData,trainClass,cv::Mat(),cv::Mat(),params);

	//using the model to to pridict some data
	for (int i = 0; i < frame->height; i++)
	{
		for (int j = 0; j < frame->width; j++)
		{
			//setting sample data values
			*((float*)CV_MAT_ELEM_PTR(*sample,0,0)) = j;
			*((float*)CV_MAT_ELEM_PTR(*sample,0, 1 ) ) = i;

			float response = svm.predict(sample);
			uchar* ptr = (uchar*) (frame->imageData + i * frame->widthStep);
			//checking class labels against predicted class.
			if(response ==1)
			{
				ptr[0]=255;
				ptr[3*j+1] = 0;
				ptr[3*j+2] = 0;
			}
			if(response ==2)
			{
				ptr[0]=0;
				ptr[3*j+1] = 255;
				ptr[3*j+2] = 0;
			}   
			if(response ==3)
			{
				ptr[0]=0;
				ptr[3*j+1] = 0;
				ptr[3*j+2] = 255;
			}



		}
	}
	//Showing support vectors
	int c = svm.get_support_vector_count();
	for (int i = 0; i < c; i++)
	{
		const float* v = svm.get_support_vector(i);
		CvPoint2D32f p1 = cvPoint2D32f(v[0], v[1]);
		cvDrawCircle(frame,cvPointFrom32f(p1),4,cvScalar(128, 128, 128),2);

	}
	//making all sample points visible on the image.
	for (int i = 0; i < (TRAIN_SAMPLE_COUNT / 3); i++)
	{
		CvPoint2D32f p1 = cvPoint2D32f(CV_MAT_ELEM(*trainData,float,i,0),CV_MAT_ELEM(*trainData,float,i,1));
		cvDrawCircle(frame,cvPointFrom32f(p1),2,cvScalar(255, 20, 20),-1);
		CvPoint2D32f p2 = cvPoint2D32f(CV_MAT_ELEM(*trainData,float,TRAIN_SAMPLE_COUNT / 3+i,0),CV_MAT_ELEM(*trainData,float,TRAIN_SAMPLE_COUNT / 3+i,1));
		cvDrawCircle(frame,cvPointFrom32f(p2),2,cvScalar(20, 255, 20),-1);
		CvPoint2D32f p3 = cvPoint2D32f(CV_MAT_ELEM(*trainData,float,2*TRAIN_SAMPLE_COUNT / 3+i,0),CV_MAT_ELEM(*trainData,float,2*TRAIN_SAMPLE_COUNT / 3+i,1));
		cvDrawCircle(frame,cvPointFrom32f(p3),2,cvScalar(20, 20, 255),-1);

	}
	cvNamedWindow( "SVM Tutorial", CV_WINDOW_AUTOSIZE );
	cvShowImage( "SVM Tutorial", frame ); 
	cvWaitKey();
}

