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

IplImage* convertRGBtogray(const IplImage *image);
/* Coarseness */

double 	Coarseness(int imgheight, int imgwidth, int k);
double 	avgNeigh(int n0, int n1, int k, double FirstCol);
double avgNeighVer(double result1, double result2);
double avgNeighHor(double result1, double result2);
double GetPixelVal(int x, int y);
int sizeLeadDiffValue(int imgheight, int imgwidth, double A[][imgwidth], int k, unsigned int argm[][imgwidth]);

int main(int argc, char** argv)
{
	double Fcoar=0;
	imageRGB  = cvLoadImage(argv[1], 1);
	convertRGBtogray(imageRGB);
	int imgheight = gray->height;
	int imgwidth = gray->width;

	Fcoar = Coarseness(imgheight,imgwidth,factor);
	printf("Coarseness of the image = %f\n", Fcoar);
	printf( "................................................................\n"); 

	cvWaitKey(0);
	//Liberta a memoria
 	cvDestroyWindow( "GRAY IMAGE" );
	cvReleaseImage(&imageRGB);
	cvReleaseImage(&gray);
}

double Coarseness(int imgheight, int imgwidth, int k)
{	
	double Fcoars;
	double result;
	double LastNeigh;
	double FirstCol;
	double A[imgheight][imgwidth];
	memset(A, 0, imgheight*imgwidth*sizeof(double));
	unsigned int i, j, K, index;
	unsigned int argm[imgheight][imgwidth];
	memset(argm, 0, imgheight*imgwidth*sizeof(int));
	
	K = 1;
	while(K <= factor){
		
		for( i=1; i<imgheight-1; i++ ){ //Altura
			LastNeigh = 0;
			for( j=1; j<imgwidth-1; j++){ //Largura
				if(K < 3){
					A[i][j] = avgNeigh(i,j,K,FirstCol);
				}else{
					FirstCol = 0; 
					result = avgNeigh(i,j,K,FirstCol);
					A[i][j] = (LastNeigh - FirstCol) + result;
					LastNeigh = A[i][j];
				}
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
double avgNeigh(int n0, int n1, int k, double FirstCol)
{
	double result = 0;
	double firstc = 0;
	int border = pow(2,2*k);
	int x,y,i,j;
	int val = pow(2,k-1);
	if(border > gray->height) border = gray->height;
	if(k < 3){
		for( i=1; i<=border; i++){    // calculate the average over neighborhoods. 
			for( j=1; j<=border; j++){
				x = n0 - val + i;
				y = n1 - val + j;
				result += GetPixelVal(x,y);
			}
		}
	}else{
		for( i=1; i<=border; i++){    // calculate the average over neighborhoods. 
			for( j=1; j<=border; j++){
				if(n1 == 1){
					x = n0 - val + i;
					y = n1 - val + j;
					result += GetPixelVal(x,y); //x - coluna
												//y - linha
				}else{
					i = border;
					x = (n0 - val + 1)-1;
					y = (n1 - val + j)-1;
					firstc += GetPixelVal(x,y);
					x = (n0 - val + i);
					y = (n1 - val + j);
					result += GetPixelVal(x,y);
				}
			}
		}
		if(n1 == 1) FirstCol = 0;
		else 		FirstCol = firstc/border;
	}

	return (result/border);
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
	
	for( i=1; i<imgheight-1; i++){		// For each point of A, select the size leading to the
		for( j=1; j<imgwidth-1; j++){	// highest value. 
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
			
			//For every point calculate the differences between the not overlapping neighborhoods on
			//opposite sides of the point horizontal and vertical dirrections.
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

IplImage* convertRGBtogray(const IplImage *image)
{
	CvSize size;
	size = cvGetSize(image);

	gray = cvCreateImage(size,IPL_DEPTH_8U,1);
    /* Convert from Red-Green-Blue to GRAY */
    cvCvtColor(image,gray,CV_BGR2GRAY);
	cvNamedWindow( "GRAY IMAGE", CV_WINDOW_AUTOSIZE);
	cvShowImage( "GRAY IMAGE", gray ); 
}