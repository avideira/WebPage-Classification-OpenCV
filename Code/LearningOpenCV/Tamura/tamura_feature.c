/*
	File:   tamura.c
	
 	Author: António Videira
	Created on 22 February, 2013
	Finished on 
	
	Function that extract the Tamura Features from an image.
	Coarseness, contrast and directionality of the image.
	The first two are single values, the other is slitinto 16 bins. 
	This result in a Tamura Feature vector of 18 attributes.
*/
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define max(a,b)(a>b?a:b)
#define ABS(a)((a<0)?-(a):(a))

#define factor 5 //1x1, 2x2, 4x4,...., 32x32

IplImage *imageRGB;
IplImage *gray;
IplImage* histimg; //image to hold the histogram
CvHistogram *histograma; //histogram to store the information from image

IplImage* convertRGBtogray(const IplImage *imageRGB);
/* Contrast */
double 	Contrast(IplImage* img);
/* Coarseness */
double 	Coarseness(int imgheight, int imgwidth, int k);
double 	avgNeigh(int n0, int n1, int k);
double avgNeighVer(int n0, int n1, int k);
double avgNeighHor(int n0, int n1, int k);
int		sizeLeadDiffValue(int n0, int n1, int k);
/* Directionality */
float filterH[3][3] = { {-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1} };
float filterV[3][3] = { {-1, -1, -1}, {0, 0, 0}, {1, 1, 1} };
double Directionality(int imgheight, int imgwidth, double *hist, float threshold);
double CalcDeltaV(int x, int y);
double CalcDeltaH(int x, int y);

int main(int argc, char** argv)
{
	double Fcon=0, Fcoar=0;
	double Fdir[16];
	imageRGB  = cvLoadImage(argv[1], 1);
	convertRGBtogray(imageRGB);
	int imgheight = gray->height;
	int imgwidth = gray->width;
	//IplImage* hist;
	
	Fcon = Contrast(gray);
	printf("Contrast of the image = %f\n", Fcon);
	printf( "................................................................\n"); 
	//Fcoar = Coarseness(imgheight,imgwidth,factor);
	printf("Coarseness of the image = %f\n", Fcoar);
	printf( "................................................................\n"); 
	Directionality(imgheight,imgwidth, Fdir, 0.05);
	int i;
	double numPixels = gray->imageSize;
	printf("numero de pixels oficiais = %f \n", numPixels);
	float soma = 0;
	for(i = 0; i< 16; i++){
		printf("Directionality in bin %d = %f\n",i, (Fdir[i]/numPixels));
		soma = soma + ((Fdir[i]/numPixels));
	}

	printf(" %f \n",soma*100);
	printf( "................................................................\n"); 

	cvWaitKey(0);
	//Liberta a memoria
 	cvDestroyWindow( "GRAY IMAGE" );
	cvReleaseImage(&imageRGB);
	cvReleaseImage(&gray);
}

double Directionality(int imgheight, int imgwidth, double *hist, float threshold)
{
	int i,j;
	double value;
	double deltv, delth, delMag;
	double binw = CV_PI / 16;
	int bin;
	double pixels = 0;
	int check = 0;
	for( i=0; i<imgheight; i++){
		for( j=0; j<imgwidth; j++){
			deltv = CalcDeltaV(i,j);
			delth = CalcDeltaH(i,j);
			delMag = (fabs(delth) + fabs(deltv)) / 2;
			if(deltv == 0)	 		value = 0;
			else if(delth == 0)		value = CV_PI;
			else					value = (CV_PI/2) + atan(deltv/delth);
			bin = value/binw;
			if(bin > 15) bin=0;
			hist[bin]++;
			pixels++;
		}
	}
	printf("Pixels out of bin = %d\n", check);
	printf("Numero de pixels = %f\n", pixels);
}
double CalcDeltaV(int x, int y)
{
	int i, j;
	double result = 0;
	CvScalar s;
	int n0,n1;
	for (i = 0; i < 3; i ++){
		for (j = 0; j < 3; j ++) {
			n0 = (x+i-1);
			n1 = (y+j-1);
 			if(n0 <= 0) n0 = 0; 
			if(n0 >= gray->height) n0 = gray->height-1;
			if(n1 <= 0) n1 = 1; 
			if(n1 >= gray->width) n1 = gray->width-1;
			s = cvGet2D(gray,n0,n1);	
			result = result + (s.val[0] * filterV[i][j]);
		}
	}
	return result;
}
double CalcDeltaH(int x, int y)
{
	int i, j;
	double result = 0;
	CvScalar s;
	int n0,n1;
	for (i = 0; i < 3; i ++){
		for (j = 0; j < 3; j ++) {
			n0 = (x+i-1);
			n1 = (y+j-1);
 			if(n0 <= 0) n0 = 1; 
			if(n0 >= gray->height) n0 = gray->height-1;
			if(n1 <= 0) n1 = 1; 
			if(n1 >= gray->width) n1 = gray->width-1;
			s = cvGet2D(gray,n0,n1);	
			result = result + (s.val[0] * filterH[i][j]);
		}
	}
	return result;
}

/*
	The coarseness gives information about the 
	size of the texture elements. The higher the 
	coarseness value is, the rougher is the texture
*/
double Coarseness(int imgheight, int imgwidth, int k)
{	
	double Fcoars;
	int i, j;
	double result;
	for( i=1; i<imgheight-1; i++){
		for( j=1; j<imgwidth-1; j++){
			result = sizeLeadDiffValue(i,j,factor);
			Fcoars += pow(2,result);
		}
	}
	Fcoars = Fcoars/(imgheight*imgwidth);		
	return Fcoars;
}
int sizeLeadDiffValue(int n0, int n1, int k) 
{
	double result = 0, tmp;
	int argmax = 1;
	int i;
	for (i = 0; i < k; i++) {
		tmp = max(avgNeighHor(n0, n1, i),avgNeighVer(n0, n1, i));//max d=h,v
			//printf("tmp = %f\n", tmp);
		if (result < tmp) {
			argmax = i;
			result = tmp;
		}
	}
	return argmax;
}
double avgNeighHor(int n0, int n1, int k)
{
	double result1 = 0, result2 = 0;
	int val = pow(2,k-1);
	result1 = avgNeigh(n0+val, n1, k);
	result2 = avgNeigh(n0-val, n1, k);
	return ABS(result1-result2);
}
double avgNeighVer(int n0, int n1, int k)
{
	double result1 = 0, result2 = 0;
	int val = pow(2,k-1);
	result1 = avgNeigh(n0, n1+val, k);
	result2 = avgNeigh(n0, n1-val, k);
	return ABS(result1-result2);
} 
double avgNeigh(int n0, int n1, int k)
{
	double result = 0, border;
	border = pow(2,2*k);
	int x = 0, y = 0;
	int i,j;
	CvScalar s;
 	for( i=1; i<=border; i++){
		for( j=1; j<=border; j++){
			x = n0 - pow(2,k-1) + i;
			y = n1 - pow(2,k-1) + j;
			if(x <= 0) x = 1;
			if(y <= 0) y = 1;
			if(x >= gray->height) x = gray->height-1;
			if(y >= gray->width) y = gray->width-1;

			s = cvGet2D(gray,x,y);	
			result += s.val[0];
		}
	}
	result = result/border;
	return result;
} 

/*
	Function: contrast
	The contrast of an imaged is calculated by
	Fcon = var/norm4.^.25 , where norm4 = u4/var4
	u4 is the forth moment about the mean u.
*/
double Contrast(IplImage* img)
{
	int y,x;
	CvScalar s;
	CvScalar mean;
    CvScalar stddev;
	double diff;
	double norm4;
    double var; 
	double result = 0;
	cvAvgSdv(img,&mean,&stddev,NULL);
	
	for(y=0; y<img->height; y++) {
		for(x=0; x < img->width; x++) {
			s=cvGet2D(img,y,x); // get the (y,x) pixel value
			diff = s.val[0] - mean.val[0];
			var += diff*diff;
			norm4 += diff*diff*diff*diff; //fourth moment about the mean u
		}
	}
	
	var /= (img->width * img->height);
	norm4 /= (img->width * img->height);
	if(norm4 == 0) return 0;
	result = (var / pow(norm4,0.25)); 
	return result; 
}
IplImage* convertRGBtogray(const IplImage *image)
{
	CvSize size;
	size = cvGetSize(image);
	//CvSize size = CvSize(height,width);
	//CvSize(height,width)
	gray = cvCreateImage(size,IPL_DEPTH_8U,1);
    /* Convert from Red-Green-Blue to GRAY */
    cvCvtColor(image,gray,CV_BGR2GRAY);
	cvNamedWindow( "GRAY IMAGE", CV_WINDOW_AUTOSIZE);
	cvShowImage( "GRAY IMAGE", gray ); 
}