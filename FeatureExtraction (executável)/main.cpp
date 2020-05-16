#include <stdlib.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cv.h>
#include <highgui.h>

#include "ExtractFeatures.h"
#include "opencv2/nonfree/nonfree.hpp"

using namespace std;
using namespace cv;

/**
	Color Histogram 32
	Edge Histogram 80
	Tamura Features 18
	Gabor Features 36
**/
#define NUMBER_OF_FEATURES 166

//See article on BoW model for details
Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("FlannBased");
Ptr<DescriptorExtractor> extractor = DescriptorExtractor::create("SIFT");

//See article on BoW model for details
int dictionarySize = 200;
TermCriteria tc(CV_TERMCRIT_ITER, 10, 0.001);
int retries = 1;
int flags = KMEANS_PP_CENTERS;

BOWKMeansTrainer bowTrainer(dictionarySize, tc, retries, flags);
BOWImgDescriptorExtractor bowDE(extractor, matcher);
Mat img, features, bowDescriptor;
IplImage *imageRGB;

void extractTrainingVocabulary( const char* filePath ){
    SiftFeatureDetector detector;
	
	img = imread(filePath);
	if (!img.empty()) {
		vector<KeyPoint> keypoints;
		detector.detect(img, keypoints);
		printf(" Keypoints Detected \n ");
		if (keypoints.empty()) {
			cerr << "Warning: Could not find key points in image: " << endl;
		} else {					
			extractor->compute(img, keypoints, features);
			bowTrainer.add(features);
		}		
	} else {
		printf( "Warning: Could not read image: %s \n" , filePath );
	}

}

void extractBOWDescriptor( const char* filePath, Mat& descriptors /*Mat& labels*/ ) {
    SiftFeatureDetector detector;
	//float label;
			
	img = imread(filePath);
	if (!img.empty()) {
		vector<KeyPoint> keypoints;
		detector.detect(img, keypoints);
		if (keypoints.empty()) {
			cerr << "Warning: Could not find key points in image: " << endl;
		} else {
			bowDE.compute(img, keypoints, bowDescriptor);
			descriptors.push_back(bowDescriptor);
		}		
	} else {
		cerr << "Warning: Could not read image: " << endl;
	}
}

void Extract( const char* filePath, const char* txtdir, int coars)
{
	
	imageRGB  = cvLoadImage(filePath, 1);
	
	if(!imageRGB){ 
		printf("Error loading image\n");
	}
	
	int height	= imageRGB->height;
 	int width 	= imageRGB->width;
	if(width > 1024)	width = 1024;
	else if( (width < 1024) && (width >896) ) width = 896;
	else if( (width < 896) && (width >768) ) width = 768;
	else if( (width < 768) && (width >640) ) width = 640;
	else if( (width < 640) && (width >512) ) width = 512;
	else if( (width < 512) && (width >448) ) width = 448;
	else if( (width < 448) && (width >384) ) width = 384;
	else if( (width < 384) && (width >352) ) width = 352;
	else if( (width < 352) && (width >320) ) width = 320;
	else if( (width < 320) && (width >288) ) width = 288;
	else if( (width < 288) && (width >256) ) width = 256;
	else if( (width < 256) && (width >224) ) width = 224;
	else if( (width < 224) && (width >192) ) width = 192;
	else if( (width < 192) && (width >176) ) width = 176;
	else if( (width < 176) && (width >160) ) width = 160;
	else if( (width < 160) && (width >152) ) width = 152;
	else if( (width < 152) && (width >144) ) width = 144;
	else if( (width < 144) && (width >136) ) width = 136;
	else if( (width < 136) && (width >128) ) width = 128;
	else if( (width < 128) && (width >120) ) width = 120;
	else if( (width < 120) && (width >112) ) width = 112;
	else if( (width < 112) && (width >104) ) width = 104;
	else if( (width < 104) && (width >96) ) width = 96;
	else if( (width < 96) && (width >88) ) width = 88;
	else if( (width < 88) && (width >84) ) width = 84;
	else if( (width < 84) && (width >80) ) width = 80;
	else if( (width < 80) && (width >76) ) width = 76;
	else if( (width < 76) && (width >72) ) width = 72;
	else if( (width < 72) && (width >68) ) width = 68;
	else if( (width < 68) && (width >64) ) width = 64;
	else if( (width < 64) && (width >60) ) width = 60;
	else if( (width < 60) && (width >56) ) width = 56;
	else if( (width < 56) && (width >52) ) width = 52;
	else if( (width < 52) && (width >48) ) width = 48;
	else if( (width < 48) && (width >44) ) width = 44;
	else if( (width < 44) && (width >40) ) width = 40;
	else if( (width < 40) && (width >36) ) width = 36;
	else if( (width < 36) && (width >32) ) width = 32;
	
	if(height > 1024)	height = 1024;
	else if( (height < 1024) && (height >896) ) height = 896;
	else if( (height < 896) && (height >768) ) height = 768;
	else if( (height < 768) && (height >640) ) height = 640;
	else if( (height < 640) && (height >512) ) height = 512;
	else if( (height < 512) && (height >448) ) height = 448;
	else if( (height < 448) && (height >384) ) height = 384;
	else if( (height < 384) && (height >352) ) height = 352;
	else if( (height < 352) && (height >320) ) height = 320;
	else if( (height < 320) && (height >288) ) height = 288;
	else if( (height < 288) && (height >256) ) height = 256;
	else if( (height < 256) && (height >224) ) height = 224;
	else if( (height < 224) && (height >192) ) height = 192;
	else if( (height < 192) && (height >176) ) height = 176;
	else if( (height < 176) && (height >160) ) height = 160;
	else if( (height < 160) && (height >152) ) height = 152;
	else if( (height < 152) && (height >144) ) height = 144;
	else if( (height < 144) && (height >136) ) height = 136;
	else if( (height < 136) && (height >128) ) height = 128;
	else if( (height < 128) && (height >120) ) height = 120;
	else if( (height < 120) && (height >112) ) height = 112;
	else if( (height < 112) && (height >104) ) height = 104;
	else if( (height < 104) && (height >96) ) height = 96;
	else if( (height < 96) && (height >88) ) height = 88;
	else if( (height < 88) && (height >84) ) height = 84;
	else if( (height < 84) && (height >80) ) height = 80;
	else if( (height < 80) && (height >76) ) height = 76;
	else if( (height < 76) && (height >72) ) height = 72;
	else if( (height < 72) && (height >68) ) height = 68;
	else if( (height < 68) && (height >64) ) height = 64;
	else if( (height < 64) && (height >60) ) height = 60;
	else if( (height < 60) && (height >56) ) height = 56;
	else if( (height < 56) && (height >52) ) height = 52;
	else if( (height < 52) && (height >48) ) height = 48;
	else if( (height < 48) && (height >44) ) height = 44;
	else if( (height < 44) && (height >40) ) height = 40;
	else if( (height < 40) && (height >36) ) height = 36;
	else if( (height < 36) && (height >32) ) height = 32;
	
/* 	IplImage *Image = cvCreateImage(cvSize(width, height), imageRGB->depth, imageRGB->nChannels);
	CvScalar s;
	for( i = 0; i < width; i++){
		for( j = 0; j < height; j++){
			s=cvGet2D(imageRGB,i,j); // get the (i,j) pixel value
			cvSet2D(Image,i,j,s); // set the (i,j) pixel value
		}
	}
 */
	CvRect old_roi = cvGetImageROI(imageRGB);
	cvSetImageROI(imageRGB, cvRect(0, 0, width, height));
	//CvScalar c = cvAvg(source);
	cvSetImageROI(imageRGB,old_roi); // reset old roi
 
	/* FUNCTION TO EXTRACT FEATURES*/
	ExtractFeatures( imageRGB , txtdir, coars );	
	
	cvReleaseImage(&imageRGB);
	//cvReleaseImage(&Image);
	
}

int main(int argc, char ** argv) 
{
	int feat, coars;
	ifstream fin;
	string dir, txtdir, filepath;
	DIR *dp;
	struct dirent *dirp;
	struct stat filestat;
	
	feat = atoi(argv[1]);
	if( feat == 1){
		if (argc != 5) {
			printf("Usage: ./main <features> <image-list> <savecsv> <Coarseness>\n");
			return 0;
		}else{
			//feat = atoi(argv[1]);
			dir = argv[2];
			txtdir = argv[3];
			coars = atoi(argv[4]);
			if( (coars > 1)  || (coars < 0) ){
				printf("<Coarseness> has to be 0 or 1 \n");  
				return 0;
			}
		}
	}else if(feat == 2){
		if (argc != 4) {
			printf("Usage: ./main <features> <image-list> <savecsv>\n");
			return 0;
		}else{
			//feat = atoi(argv[1]);
			dir = argv[2];
			txtdir = argv[3];
		}
	}
	
	switch(feat) {
		case 1:
			printf(" Extracting Color+Edge+Tamura+Gabor Features \n");
			InitTable();
			dp = opendir( dir.c_str() );
			if (dp == NULL)
			{
				cout << "Error opening " << dir << endl;
				return 0;
			}
			while ((dirp = readdir( dp )))
			{
				filepath = dir + "/" + dirp->d_name;

				// If the file is a directory (or is in some way invalid) skip it 
				if (stat( filepath.c_str(), &filestat )) continue;
				if (S_ISDIR( filestat.st_mode ))         continue;
				
				printf(" Processing image in %s.\n", filepath.c_str());
				Extract( filepath.c_str(), txtdir.c_str() , coars);
			}
			closedir( dp );
			break;
			
		case 2:
			printf(" Extracting SIFT Features \n");
			printf(" Creating dictionary... \n" );
			dp = opendir( dir.c_str() );
			if (dp == NULL)
			{
				cout << "Error opening " << dir << endl;
				return 0;
			}
			while ((dirp = readdir( dp )))
			{
				filepath = dir + "/" + dirp->d_name;
				printf(" Processing image in %s \n", filepath.c_str());
				extractTrainingVocabulary( filepath.c_str() );
			}
			closedir( dp );
			
			vector<Mat> descriptors = bowTrainer.getDescriptors();
			int count = 0;
			for( vector<Mat>::iterator iter=descriptors.begin(); iter!=descriptors.end(); iter++ )
			{
				count += iter->rows;
			}
			printf( "Clustering %d features. \n", count );
			Mat dictionary = bowTrainer.cluster();
			
			bowDE.setVocabulary(dictionary);
			printf( "Processing training data...\n" );
			Mat trainingData(0, dictionarySize, CV_32FC1);
			//Mat labels(0, 1, CV_32FC1);
			dp = opendir( dir.c_str() );
			if (dp == NULL)
			{
				cout << "Error opening " << dir << endl;
				return 0;
			}
			while ((dirp = readdir( dp )))
			{
				filepath = dir + "/" + dirp->d_name;
				printf(" Processing image in %s \n", filepath.c_str());
				extractBOWDescriptor( filepath.c_str() , trainingData );
			}
			closedir( dp );
			int i,j;

			double feats[trainingData.cols];
			for( i = 0; i < trainingData.rows; i++){
				for( j = 0; j < trainingData.cols; j++){
					feats[j] = trainingData.at<uchar>(i,j);
				}
				SaveFeatures2 (txtdir.c_str(), feats, j);
			}
			break;
	}
}