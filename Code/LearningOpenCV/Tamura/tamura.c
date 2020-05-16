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
int imgheight;
int imgwidth;
double max;
IplImage* convertRGBtogray(const IplImage *imageRGB);
/* Contrast */
double contrast(IplImage* img);
/* Coarseness */
double Coarseness(IplImage* img, int k);
unsigned char avgdiff(unsigned char neigh[imgheight][imgwidth],int n0, int n1, int k);
double avgNeigh(IplImage *img,int n0, int n1, int k);
/* Directionality */
void DrawHistogram(IplImage* img,int bins);
void Directionality(IplImage* img, int bins);
int calcPixels(int bins, IplImage* img);

int main(int argc, char** argv)
{
	int bins = 16; //16 níveis
	double Fcon=0;
	double Fcoar=0;
	
	imageRGB  = cvLoadImage(argv[1], 1);
	convertRGBtogray(imageRGB);
	imgheight = gray->height;
	imgwidth = gray->width;
	
	Fcon = contrast(gray);
	printf("Contrast of the image = %f\n", Fcon);
	printf( "................................................................\n"); 

	Fcoar = Coarseness(gray,factor);
	printf("Coarseness of the image = %f\n", Fcoar);
	printf( "................................................................\n"); 
	//Directionality(gray,bins);
	// printf( "................................................................\n"); 
	// cvNamedWindow( "Histogram", 0);
	// cvResizeWindow("Histogram",400, 400);
	// cvShowImage( "Histogram", histimg);
	cvWaitKey(0);
	//Liberta a memoria
 	cvDestroyWindow( "GRAY IMAGE" );
	cvReleaseImage(&imageRGB);
	cvReleaseImage(&gray);
}


void Directionality(IplImage* img, int bins){
	int i,j;
	
	float hor[] = { -1, 0, 1, 
					-1, 0, 1, 
					-1, 0, 1 };
	float ver[] = { -1, -1, -1, 
					0, 0, 0, 
					1, 1, 1 };

	//IplImage* Hkernel;
	//IplImage* Vkernel;
	
	CvMat Hk = cvMat(3,3,CV_32FC1,hor); //coeficients of the convolution kernel should
 	CvMat Vk = cvMat(3,3,CV_32FC1,ver); //always be floating-point numbers.	
	//cvFlip(&Vk,&Vk,-1);
	//cvFlip(&Hk,&Hk,-1);
/* 	for(i = 0; i<Hk.rows; i++){
		for(j = 0; j < Hk.cols; j++ ) { 
			CvScalar scal = cvGet2D( &Hk, i, j ); 
			printf( "%f\n", scal.val[0]); 
		}
	} */

	//Vertical and Horizontal derivatives
	IplImage* Vdst= cvCreateImage(cvGetSize(img),IPL_DEPTH_8U, 1);
	IplImage* Hdst = cvCreateImage(cvGetSize(img),IPL_DEPTH_8U, 1);
	IplImage* theta= cvCreateImage(cvGetSize(img),IPL_DEPTH_32F, 1);
	CvPoint anchor = cvPoint(1,1);

	//are calculated by convolution of the image X(no,n1) with the 3x3 operators
	//cvFilter2D(img, Hdst,&Hk,cvPoint(Hk.cols-anchor.x-1,Hk.rows-anchor.y-1));
	//cvFilter2D(img, Vdst,&Vk,cvPoint(Vk.cols-anchor.x-1,Vk.rows-anchor.y-1));
	cvFilter2D(img, Vdst, &Vk,cvPoint(-1,-1));
	cvFilter2D(img, Hdst, &Hk,cvPoint(-1,-1));

	double value;
	//double deltv, delth;
	CvScalar s;
	CvScalar deltv;
	CvScalar delth;
	
	for( i=0; i<theta->width; i++){
		for( j=0; j<theta->height; j++){
			deltv = cvGet2D(Vdst,i,j);
			delth = cvGet2D(Hdst,i,j);
			//deltv = ((uchar *)(Vdst->imageData + i*Vdst->widthStep))[j*Vdst->nChannels +0];
			//delth = ((uchar *)(Hdst->imageData + i*Hdst->widthStep))[j*Hdst->nChannels +0];
			if(deltv.val[0] == 0 && delth.val[0] == 0){
			//if(deltv == 0 && delth == 0){
				s.val[0] = 0;
			}else if(delth.val[0] == 0){
			//}else if(delth == 0){
				s.val[0] = CV_PI;
			}else{
				value = (CV_PI/2) + atan(deltv.val[0]/delth.val[0]);
				//value = (CV_PI/2) + atan(deltv/delth);
				s.val[0] = value;
			}
			max = s.val[0];
			if(max<s.val[0]) max = s.val[0];
				
			cvSet2D(theta,i,j,s);
			//printf("valor: %f \n ", s.val[0]);
		}
	}
	printf("valor maximo: %f\n", max);
	cvReleaseImage(&Vdst);
	cvReleaseImage(&Hdst);
	DrawHistogram(theta,bins);
	calcPixels(bins,theta);
}

void DrawHistogram(IplImage* img,int bins){
	int i;
	int normalized, value;
	int hist_size[] = {bins}; //Just 1 dimension
	/*Range of the histogram*/
	float range[]={0,CV_PI}; // value varies from 0 to PI
	float* ranges[] = { range };
	IplImage* planes[] = {img};
	float max_value = 0.0;
	
	histograma = cvCreateHist(1,hist_size,CV_HIST_ARRAY,ranges,1);
	//calcula histograma da imagem e guarda no array 
  	cvCalcHist(planes, histograma, 0, NULL );
	//valores minimos e máximos
    cvGetMinMaxHistValue(histograma, 0, &max_value, 0, 0 );
	histimg = cvCreateImage(cvSize(bins, 2*CV_PI),8,1);
    cvRectangle(histimg, cvPoint(0,0), cvPoint(256,2*CV_PI), CV_RGB(255,255,255),-1,8,0);
	//draw the histogram 
    for(i=0; i < bins; i++){
            value = cvQueryHistValue_1D(histograma, i);
            normalized = cvRound(value*2*CV_PI/max_value);
            cvLine(histimg,cvPoint(i,2*CV_PI), cvPoint(i,2*CV_PI-normalized), CV_RGB(0,0,0),1,8,0);

	}
	
	return;
}

int calcPixels(int bins,IplImage* img){
	int row, col;
	int pixels;
	int i;
	float v = CV_PI/bins;
	int somatorio= 0;
	float min=0, max=0;
	CvScalar s;
	int pixelstotal = img->imageSize;
	printf("Numero de Pixels na imagem = %d \n", pixelstotal);
	for(i=1; i < bins+1; i++){	
		pixels = 0;
		max = ((i*v)-0.001);
		min = max - v;
		if(min <0) min = 0;
		for(row = 0; row < img->height; row++)
		{
			for (col = 0; col < img->width; col++)
			{
				s=cvGet2D(img,row,col);
				if( s.val[0] >= min && s.val[0] <= max){
					pixels++;
				}
			}
		}
		printf("Números de pixels entre '%f' e '%f' = '%d'\n ",min, max, pixels);
		somatorio = somatorio + pixels;
	}
	printf("total de pixels contados = %d\n", somatorio);
	return 0;
}

/*
	The coarseness gives information about the 
	size of the texture elements. The higher the 
	coarseness value is, the rougher is the texture
*/
double Coarseness(IplImage* img, int k){
	int  i,j;//the height and widht of the image.
	int step;
	unsigned char neigh[imgheight][imgwidth];
	unsigned char diffs[imgheight][imgwidth];
	unsigned char  max[imgheight][imgwidth];
	int  	sbest[imgheight][imgwidth];
	memset(neigh,0, sizeof(char)*imgheight*imgwidth);
	memset(diffs,0, sizeof(char)*imgheight*imgwidth);
	memset(sbest,0, sizeof(int)*imgheight*imgwidth);
	double Fcoars = 0;
	int power;
	for(step = 0; step <=k; step++){
		power = pow(2,step); //windows powers of 2.
		for( i=power; i<imgheight-power; i++){
			for( j=power; j<imgwidth-power; j++){
				neigh[i][j] = (unsigned char)avgNeigh(img,i,j,step);
			}
		}
	
		for( i=power; i<imgheight-power; i++){
			for (j=power; j<imgwidth-power; j++){ 
				diffs[i][j]= avgdiff(neigh,i,j,step);
				if(step == 0){
					max[i][j] = diffs[i][j];
				}else{
					if(max[i][j] < diffs[i][j]){ 
						max[i][j] = diffs[i][j];
						sbest[i][j] = step;
						}
				}
			
			}
			
		}
		
	}
	
	for( i=0; i<imgheight; i++){
		for( j=0; j<imgwidth; j++){
			Fcoars += pow(2,sbest[i][j]);
		}
	}
	Fcoars = Fcoars/(imgheight*imgwidth);		
	return Fcoars;
}
/*
	Function: avgdiff
	For every point (x,y) calculates differences
	between the not overlapping neighborhoods on oppossite
	sides of the point in horizontal and vertical dirrections.
*/
unsigned char avgdiff(unsigned char neigh[imgheight][imgwidth], int n0, int n1, int k){
	unsigned char result;
	unsigned char diffH; //diferença horizontal
	unsigned char diffV; //diferença vertical
	int xH,xV,yH,yV;
	xH = n0 + (pow(2, k-1));
	yH = n0 - (pow(2, k-1));
	xV = n1 + (pow(2, k-1));
	yV = n1 - (pow(2, k-1));
	
	unsigned char nei1, nei2, nei3, nei4;
	
	if(xH < 0 || xH > imgheight) nei1 = 0;
			else nei1 = neigh[xH][n1];
	
	if(xV < 0 || xV > imgwidth) nei2 = 0;
			else nei2 = neigh[n0][xV];
		
	if(yH < 0 || yH > imgheight) nei3 = 0;
			else nei3 = neigh[yH][n1];
		
	if(yV < 0 || yV > imgwidth) nei4 = 0;
			else nei4 = neigh[n0][yV];
			
	diffH = ABS((nei1)-(nei3));
	diffV = ABS((nei2)-(nei4));
	result = max(diffH, diffV); //max d=h,v
	return result;
} 

/*
	Function: avgNeigh
	For every point (x,y) calculates 
	the average over neighborhoods.
*/
double avgNeigh(IplImage *img,int n0, int n1, int k){
	int border = pow(2,2*k);
	int value = pow(2,(k-1));
	
	double result = 0;
	int i,j;
	int x, y;
	CvScalar s;
 	for( i=1; i<=border; i++){
		for( j=1; j<=border; j++){
			x = n0 - value + i;
			y = n1 - value + j;
			if(x <= 0 || x >= imgheight){
				s.val[0] = 0;
			}else if(y <= 0 || y >= imgwidth){
				s.val[0] = 0;
			}else{
				s = cvGet2D(img,x,y);
			}	
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
double contrast(IplImage* img){
	int y,x;
	CvScalar s;
	CvScalar mean;
    CvScalar stddev;
	double diff;
	double norm4;
    double var; 
	double result = 0;
	cvAvgSdv(img,&mean,&stddev,NULL);
	//printf("mean value of pixels = %f\n", mean.val[0]);
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
/*
	Function: convertRGBtogray
	Convert an RGB image to a grayscale.
*/
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