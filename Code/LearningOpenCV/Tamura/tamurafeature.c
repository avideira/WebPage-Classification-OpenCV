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


IplImage* convertRGBtogray(const IplImage *imageRGB);
/* Contrast */
double 	Contrast(IplImage* img);
/* Coarseness */
double 	Coarseness(int imgheight, int imgwidth, int k);
double 	avgNeigh(int n0, int n1, int k);
double avgNeighVer(double result1, double result2);
double avgNeighHor(double result1, double result2);
double GetPixelVal(int x, int y);
int sizeLeadDiffValue(int imgheight, int imgwidth, double A[][imgwidth], int k, unsigned int argm[][imgwidth]);
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
	
	/* Fcon = Contrast(gray);
	printf("Contrast of the image = %f\n", Fcon);
	printf( "................................................................\n");  */
	Fcoar = Coarseness(imgheight,imgwidth,factor);
	printf("Coarseness of the image = %f\n", Fcoar);
	printf( "................................................................\n"); 
/* 	Directionality(imgheight,imgwidth, Fdir, 0.05);
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
 */
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
	double result;
	double A[imgheight][imgwidth];
	memset(A, 0, imgheight*imgwidth*sizeof(double));
	unsigned int i, j, K, index;
	unsigned int argm[imgheight][imgwidth];
	memset(argm, 0, imgheight*imgwidth*sizeof(int));
	
	K = 1;
	while(K <= factor){
		for( i=1; i<imgheight-1; i++ ){
			for( j=1; j<imgwidth-1; j++){
				A[i][j] = avgNeigh(i,j,K);
			}
		}
		sizeLeadDiffValue(imgheight, imgwidth, A, K, argm);
		printf("K aumenta\n");
		K++;
	}
	int valor;
	for( i=1; i<imgheight-1; i++ ){
		for( j=1; j<imgwidth-1; j++ ){
			valor = argm[i][j];
			Fcoars += pow(2,valor);
		}
	}
	Fcoars = Fcoars/(imgheight*imgwidth);		
	return Fcoars;
	
}

int sizeLeadDiffValue(int imgheight, int imgwidth, double A[][imgwidth], int k, unsigned int argm[][imgwidth]) 
{
	double result[imgheight][imgwidth];
	double tmp 	= 0;
	double res1 = 0;
	double res2 = 0;
	double res3 = 0;
	double res4 = 0;
	int i,j,x1,x2,y1,y2,argmax;
	int val = pow(2,k-1);
	argmax = k;
	
	for( i=1; i<imgheight-1; i++){
		for( j=1; j<imgwidth-1; j++){
			x1 = (int)i+val;
			x2 = (int)i-val;
			y1 = (int)j+val;
			y2 = (int)j-val;
			
			if(x1 <= 0 || x1 >= imgheight) res1 = 0;
			else res1 = A[x1][j];
			if(x2 <= 0 || x2 >= imgheight) res2 = 0;
			else res2 = A[x2][j];
			if(y1 <= 0 || y1 >= imgwidth)  res3 = 0;
			else res3 = A[i][y1];
			if(y2 <= 0 || y2 >= imgwidth)  res4 = 0;
			else res4 = A[i][y2];
			
			tmp = max(avgNeighHor(res1,res2), avgNeighVer(res3,res4)); 	//max d=h,v
			
			if ( argmax == 1){ 
				result[i][j] = tmp;
				argm[i][j] = argmax;
			}else{
				if(result[i][j] < tmp){
					result[i][j] = tmp;
					argm[i][j] 	= argmax;
				}
			}
			tmp = 0;
		}
	}
}		

double avgNeighHor(double result1, double result2)
{
	return ABS(result1-result2);
} 
double avgNeighVer(double result1, double result2)
{
	return ABS(result1-result2);
} 
double avgNeigh(int n0, int n1, int k)
{
	double result = 0;
	int border = pow(2,2*k);
	int x,y,i,j;
	int val = pow(2,k-1);
	if(border > gray->height) border = gray->height;

	for( i=1; i<=border; i++){
		for( j=1; j<=border; j++){
			x = n0 - val + i;
			y = n1 - val + j;
			result += GetPixelVal(x,y);
		}
	}

	result = result/border;
	return result;
} 
double GetPixelVal(int x, int y)
{
	CvScalar s;
	if(x <= 0 || x >= gray->height || y <= 0 || y >= gray->width) 
		s.val[0] = 0;
	else 
		s = cvGet2D(gray,x,y);
	
	return(s.val[0]);
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