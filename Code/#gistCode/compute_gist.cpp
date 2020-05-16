/* Lear's GIST implementation, version 1.1, (c) INRIA 2009, Licence: PSFL */
#include <iostream>
#include <fstream>
#include "highgui.h"
#include "cv.h"
#include "ml.h"
#include <cstdio>
#include <string>
#include <cstring>
#include <cmath>
#include "compute_gist.h"
#include "lbp.hpp"
extern "C" {
	#include "gist.h"
	#include "standalone_image.h"
}

using namespace cv;
using namespace std;

const static int GIST_SIZE = 128;
const static int feature_vector_length = 960;
const static int nblocks = 4;
const static int n_scale = 3;
const static int orientations_per_scale[50]={8,8,4};
const static int pos_label = 1;
const static int neg_label = 2;

float 	**Features;


///****************************************************************************
//                     Center Symmetric LBP
// Only 16 possible values, the spatial position of p0..p7 around current pixel 
// is the same as the LBP operator
int GetCS_LBPImage(IplImage *imageIn,IplImage *imageOut )
{
	unsigned char lbp_value;
	unsigned char *p0,*p1,*p2,*p3,*p4,*p5,*p6,*p7;
	unsigned char *currentPixel;
	unsigned char *currentLBP;
	int LBPTHRESH = 50;
	int nRows=imageIn->height;
	int nCols=imageIn->width;

	//Input image must be grayscale
	if ( (imageIn->nChannels!=1) || (imageOut->nChannels!=1) )
	{
		printf("Image must be gray\n" );
		return 0;
	}
	if ( (imageIn->height!=imageOut->height) || (imageIn->width!=imageOut->width) )
	{
		printf("Images Out and In, must be the same size\n" );
		return 0;
	}
	cvSetZero(imageOut);//set the result image to zero

	for (int rows=1;rows<nRows;rows++)
		{
			
			currentPixel=(unsigned char *) imageIn->imageData+rows*imageIn->widthStep;//point to start of data in row[rows]
			p0=currentPixel-imageIn->widthStep;
			p1=p0++;                                          //   p0  p1  p2
			p2=p1++;                                          //   p3  cP  p4
			p3=currentPixel;                                  //   p5  p6  p7
			currentPixel++;                                           
			p4=currentPixel+1;
			p5=p3+imageIn->widthStep;
			p6=currentPixel+imageIn->widthStep;
			p7=p4+imageIn->widthStep;
			currentLBP=(unsigned char *) imageOut->imageData+rows*imageOut->widthStep;
			*currentLBP=0;   
			int cols;
			for (cols=1; cols<nCols; cols++)
			{
				//stricter greater than central pixel. The original is >=
				//lbp_values exact as original paper
			   /*lbp_value=0;
			  lbp_value |=(*p2++>*p5++) ;         lbp_value <<=1;
			  lbp_value |=(*p1++>*p6++) ;      lbp_value <<=1;
			  lbp_value |=(*p0++>*p7++) ;      lbp_value <<=1;
			  lbp_value |=(*p3++>*p4++) ;     */ 
								
			  lbp_value = 0;
			  (lbp_value |= (*p2++-*p5++)) >LBPTHRESH;    lbp_value <<=1;
			  (lbp_value |= (*p1++-*p6++)) >LBPTHRESH;    lbp_value <<=1;
			  (lbp_value |= (*p0++-*p7++)) >LBPTHRESH;    lbp_value <<=1;
			  (lbp_value |= (*p3++-*p4++)) >LBPTHRESH;      
			 // currentPixel++;
			  *currentLBP++=lbp_value;
			}// for cols

		 
		}// for rows

	return 0;
}

void convert_IplImage_to_LEAR(IplImage* src, color_image_t* dst) {
  assert(src->width == GIST_SIZE && src->height == GIST_SIZE);
  assert(src->depth == IPL_DEPTH_8U);
  RgbImage imgA(src);
  int x, y, i = 0;
  for (y = 0; y < GIST_SIZE; y++) {
    for (x = 0; x < GIST_SIZE; x++) {
      dst->c1[i] = imgA[y][x].r;
      dst->c2[i] = imgA[y][x].g;
      dst->c3[i] = imgA[y][x].b;
      i++;
    }
  }
  assert(i == (GIST_SIZE * GIST_SIZE));
}

/* void convert_LEAR_to_IplImage(IplImage* dst, color_image_t* src) {
  assert(dst->width == GIST_SIZE && dst->height == GIST_SIZE);
  assert(dst->depth == IPL_DEPTH_8U);
  RgbImage imgA(dst);
  int x, y, i = 0;
  for (y = 0; y < GIST_SIZE; y++) {
    for (x = 0; x < GIST_SIZE; x++) {
	  imgA[y][x].r = src->c1[i];
      imgA[y][x].g = src->c2[i];
      imgA[y][x].b = src->c3[i];
      i++;
    }
  }
  
  assert(i == (GIST_SIZE * GIST_SIZE));
} */

float* my_compute_gist(IplImage* img, IplImage* rsz) {
	/* Resize image to 128x128 before calculating GIST descriptor. */
	assert(img);
	assert(rsz);
	// const char* window1 = "original";
	// flash_image(img, window1);
	cvResize(img, rsz, CV_INTER_LINEAR);
	// const char* window2 = "resize";
	// flash_image(rsz, window2);
	/* Lear's compute_gist takes a ppm and computes gist descriptor for it. */
	color_image_t *lear = color_image_new(GIST_SIZE, GIST_SIZE);
	assert(lear);
	convert_IplImage_to_LEAR(rsz, lear);
	//assert(Gabor);
	int i;
	Features  = (float**)malloc((lear->width*lear->height)*sizeof(float*));
	for ( i = 0; i < (lear->width*lear->height) ; i++)
	{
		Features[i] = (float *)malloc (3 * sizeof(float));
	}
  
	float *desc = color_gist_scaletab(lear, nblocks, n_scale, orientations_per_scale, Features);
	  
	/* Cleanup. */
	color_image_delete(lear);
	return desc;
} 

/* static color_image_t *load_ppm(const char *fname) {
  FILE *f=fopen(fname,"r");
  if(!f) {
    perror("could not open infile");
    exit(1);
  }


  int px,width,height,maxval;
  if(fscanf(f,"P%d %d %d %d",&px,&width,&height,&maxval)!=4 || maxval!=255 || (px!=6 && px!=5)) {
    fprintf(stderr,"Error: input not a raw PGM/PPM with maxval 255\n");
    exit(1);    
  }
  fgetc(f);
  color_image_t *im=color_image_new(width,height);
	int i;
  for(i=0;i<width*height;i++) {
    im->c1[i]=fgetc(f);
    if(px==6) {
      im->c2[i]=fgetc(f);
      im->c3[i]=fgetc(f);    
    } else {
      im->c2[i]=im->c1[i];
      im->c3[i]=im->c1[i];   
    }
  }
  
  fclose(f);
  return im;
} */

int number_of_lines(const char* filename) {
  ifstream file(filename);
  assert(file);
  int n = 0;
  string line;
  while (getline(file, line)) {
    /* Ignore empty lines. */
    if (strlen(line.c_str()) != 0) {
      n++;
    }
  }
  return n;
}


CvMat* feature_vectors_img128x128(const char* imagelist_filename,
				  char* savexml = NULL) {
  //int number_samples = number_of_lines(imagelist_filename);
  //CvMat* training = cvCreateMat(1, feature_vector_length, CV_32FC1);
  // CvMat row;
  // int i = 0, row_index = 0;
  // ifstream imagelist_file(imagelist_filename);
  // assert(imagelist_file);
  int i = 0;
  //string filename;
  IplImage *img, *gist_img, *gray;
  float *desc;
  
  //printf("Beginning to extract GIST descriptors from %s image\n", imagelist_filename);
  //while (getline(imagelist_file, filename)) {
    /* Ignore empty lines. */
 /*    if (strlen(filename.c_str()) == 0) {
      continue;
    } */
    img = cvLoadImage(imagelist_filename);
    if (!img) {
      cout << "Error opening image file named: " << imagelist_filename << "\n";
      assert(img);
    }
	gray 	= 	cvCreateImage(cvGetSize(img), 8, 1);
	cvCvtColor(img, gray, CV_BGR2GRAY );
	IplImage *destoriginal = cvCreateImage(cvSize(gray->width,gray->height), IPL_DEPTH_8U, 1);
	Mat Ogray(gray);
	IplImage *OLBPoriginal = cvCreateImage(cvSize(gray->width,gray->height), IPL_DEPTH_8U, 1);
	Mat LBPimage(OLBPoriginal);

	int radius = 1;
	int neighbors = 8;
	lbp::ELBP(Ogray, LBPimage, radius, neighbors);
	//lbp::OLBP(Ogray, LBPimage);
	//GetCS_LBPImage(gray,destoriginal);
	namedWindow( "EXTENDED LBP", CV_WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "EXTENDED LBP", Ogray );
	// cvNamedWindow("LBPGRAY", CV_WINDOW_AUTOSIZE);
	// cvShowImage("LBPGRAY", destoriginal);
	cvWaitKey(0);
	
    gist_img = cvCreateImage(cvSize(GIST_SIZE, GIST_SIZE), IPL_DEPTH_8U, 1);
	
	
    desc = my_compute_gist(destoriginal, gist_img);
	
    for (i = 0; i < feature_vector_length; i++) {
      printf("%f ",desc[i]);
    }
	printf("\n");

    /* Clean up descriptor. */
    free(desc);
  
}

int for_calc_features(int argc, char** argv) {
  if (argc != 4) {
    printf("Usage: gist-classifier calc_features <image-list> <savexml>\n");
    return 0;
  }
  string calc_features("calc_features");
  char* option = argv[1];
  assert(calc_features.compare(option) == 0);
  char* imagelist_filename = argv[2];
  char* savexml = argv[3];
  assert(savexml);
  
  feature_vectors_img128x128(imagelist_filename, /*number_samples,*/ savexml);
  return 0;
}


int main(int argc,char **args) 
{  
	if (argc < 2) {
		printf("Usage: gist-classifier <option> <optional image or video path>\n");
		return 0;
	}
	
	//IplImage *src_img = cvLoadImage(args[1]);
	return for_calc_features(argc, args);

}