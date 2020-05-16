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
#include "tamurafeatures.h"
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define 	max(a, b)(a > b ? a : b)
#define 	ABS(a)((a < 0) ? -(a):(a))
#define 	factor 5 	//2x2, 4x4, 8x8, 16x16, 32x32

void ExtractTamuraFeat(IplImage *image , double* TamuraVector, int coars)
{
	double 	Fdir[16];
	double 	Fcon 		= 0; 
	double 	Fcoar;

	int 	i;
	int 	tamurabins 	= 18;	
	int 	imgheight 	= image->height;
	int 	imgwidth 	= image->width;
	
	Fcon = Contrast(image);
	if(coars == 1){
		Fcoar = Coarseness(image);
	}else{
		Fcoar = 0;
	}
	Directionality(image,imgheight,imgwidth, Fdir);
	
	TamuraVector[0]  =  Fcon;
	TamuraVector[1]  =  Fcoar;
	
	double norm = 0.0;
   
	for( i = 0; i < tamurabins-2; i++)
	{
		norm += Fdir[i]*Fdir[i];
	}

	norm = sqrt( norm );

	for( i = 0; i < tamurabins-2; i++)
	{
		Fdir[i] = Fdir[i] / norm;
		TamuraVector[i+2] = Fdir[i];
 
	}
	
}

void Directionality(IplImage *image,int imgheight, int imgwidth, double *hist)
{
	int 	i, j, bin;
	double 	deltv, delth, /*delMag,*/ value, pixels;
	double 	binw = CV_PI / 16;
	
	pixels 	= 0;

	
	for( i = 0; i < imgheight; i++)
	{
		for( j = 0; j < imgwidth; j++)
		{
			deltv 	= 	CalcDeltaV(image,i,j);
			delth 	= 	CalcDeltaH(image,i,j);
			//delMag 	= 	(fabs(delth) + fabs(deltv)) / 2;
			if(deltv ==	0)	 		value = 0;
			else if(delth == 0)		value = CV_PI;
			else			value = (CV_PI/2) + atan(deltv/delth);
			
			bin 	= 	value/binw;
			if( bin > 15 ) bin = 0;
			hist[bin]++;
			pixels++;
		}
	}
}

double CalcDeltaV(IplImage *image,int x, int y)
{
	int 	i, j, n0, n1;
	double 	result = 0;
	CvScalar s;
	
	float filterV[3][3] = { {-1, -1, -1}, {0, 0, 0}, {1, 1, 1} };
	
	for (i = 0; i < 3; i ++)
	{
		for (j = 0; j < 3; j ++)
		{
			n0 	= 	(x+i-1);
			n1 	= 	(y+j-1);
 			if(n0 <= 0) 			n0 = 0; 
			if(n0 >= image->height) n0 = image->height-1;
			if(n1 <= 0) 			n1 = 1; 
			if(n1 >= image->width) 	n1 = image->width-1;
			s 	= 	cvGet2D(image,n0,n1);
			
			result = result + (s.val[0] * filterV[i][j]);
		}
	}
	return result;
}

double CalcDeltaH(IplImage *image,int x, int y)
{
	int 	i, j, n0, n1;
	double 	result = 0;
	CvScalar s;
	
	float filterH[3][3] = { {-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1} };
	
	for (i = 0; i < 3; i ++)
	{
		for (j = 0; j < 3; j ++)
		{
			n0 	= 	(x+i-1);
			n1 	= 	(y+j-1);
 			if(n0 <= 0) 			n0 = 1; 
			if(n0 >= image->height) n0 = image->height-1;
			if(n1 <= 0) 			n1 = 1; 
			if(n1 >= image->width) 	n1 = image->width-1;
			s 	= 	cvGet2D(image,n0,n1);
			
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
double Coarseness(IplImage *image)
{	
	double 	Fcoars, LastNeigh, FirstCol, result;
	double 	**A;
	int 	i, j; 

	unsigned 	int 	**argm;
	int 	K, valor;
	int 	imgheight = image->height;
	int 	imgwidth = image->width;
	
	A = (double**)malloc((imgheight)*sizeof(double*));
	argm  = (unsigned int**)malloc((imgheight)*sizeof(unsigned int*));
	
	for ( i = 0; i < imgheight; i++ )
	{
		A[i] = (double *)malloc (imgwidth * sizeof(double));
	}
	for ( i = 0; i < imgheight ; i++)
	{
		argm[i] = (unsigned int *)malloc (imgwidth * sizeof(unsigned int));
	}
	
	K = 1;
	while(K <= factor)
	{
		for( i = 1; i < imgheight-1; i++ )		//Altura
		{
			LastNeigh = 0;
			for( j = 1; j < imgwidth-1; j++) 	//Largura
			{ 
				if(K < 3)
				{
					A[i][j] = avgNeigh(image,i,j,K,FirstCol);
				}
				else
				{
					FirstCol 	= 0; 
					result 		= avgNeigh(image,i,j,K,FirstCol);
					A[i][j] 	= (LastNeigh - FirstCol) + result;
					LastNeigh 	= A[i][j];
				}
			}
		}
		
		sizeLeadDiffValue(imgheight, imgwidth, A, K, argm); 
		//printf("K aumenta\n");
		K++;
	}
	int maxval = 0;
	
	for( i = 1; i < imgheight-1; i++ )
	{
		for( j = 1; j < imgwidth-1; j++ )
		{
			valor 	= 	argm[i][j];
			Fcoars 	+= 	pow(2,valor);
		}
	}
	
	maxval = pow(2, factor);
	Fcoars 	= 	Fcoars/(imgheight*imgwidth); 
	Fcoars  =  Fcoars / maxval; /* normalize by maximum value */ 
	return Fcoars;
}

void sizeLeadDiffValue(int imgheight, int imgwidth, double **A, int k, unsigned int **argm) 
{
	int i,j,x1,x2,y1,y2,argmax;
	double **result;
	result = (double**)malloc((imgheight)*sizeof(double*));
		
	for ( i = 0; i < imgheight; i++ )
	{
		result[i] = (double *)malloc (imgwidth * sizeof(double));
	}
	double tmp 	= 0;
	double res1 = 0;
	double res2 = 0;
	double res3 = 0;
	double res4 = 0;

	int val = 2*k;
	argmax = k;
	
	for( i = 1; i < imgheight-1; i++) // For each point of A, select the size leading to the highest value. 
	{		
		for( j = 1; j < imgwidth-1; j++)
		{
			x1 = (int)i+val;
			x2 = (int)i-val;
			y1 = (int)j+val;
			y2 = (int)j-val;
			
			if( x1 <= 0 || x1 >= imgheight ) res1 = 0;
			else 							 res1 = A[x1][j];
			if( x2 <= 0 || x2 >= imgheight ) res2 = 0;
			else 							 res2 = A[x2][j];
			if( y1 <= 0 || y1 >= imgwidth )  res3 = 0;
			else 							 res3 = A[i][y1];
			if( y2 <= 0 || y2 >= imgwidth )  res4 = 0;
			else 							 res4 = A[i][y2];
			
			//For every point calculate the differences between the not overlapping neighborhoods on
			//opposite sides of the point horizontal and vertical dirrections.
			
			tmp = 	max(ABS(res1-res2), ABS(res3-res4)); //max d=h,v
			
			if ( argmax == 1 )
			{ 
				result[i][j] 	= tmp;
				argm[i][j] 		= argmax;
			}
			else
			{
				if( result[i][j] < tmp )
				{
					result[i][j] 	= tmp;
					argm[i][j] 		= argmax;
				}
			}
			tmp = 0;
		}
	}
}


double avgNeigh(IplImage *image, int n0, int n1, int k, double FirstCol)
{
	double 	result 	= 0;
	double 	firstc 	= 0;
	int 	x,y,i,j;
	int 	border 	= pow(2,2*k);
	int 	val 	= pow(2,k-1);
	
	if( border > image->height ) 	border = image->height;
	
	if( k < 3 )
	{
		for( i = 1; i <= border; i++ )  //calculate the average over neighborhoods.
		{    
			for( j = 1; j <= border; j++ )
			{
				x 		= 	n0 - val + i;
				y 		= 	n1 - val + j;
				result 	+= 	GetPixelVal(image,x,y);
			}
		}
	}
	else{
		for( i = 1; i <= border; i++ ) 	//calculate the average over neighborhoods.
		{     
			for( j = 1; j <= border; j++)
			{
				if( n1 == 1 )
				{
					x 		= 	n0 - val + i;
					y 		= 	n1 - val + j;
					result 	+= 	GetPixelVal(image,x,y); 	//x - coluna; y - linha											
				}
				else{
					i 		= 	border;
					x 		= 	(n0 - val + 1)-1;
					y 		= 	(n1 - val + j)-1;
					firstc 	+= 	GetPixelVal(image,x,y);
					x 		= 	(n0 - val + i);
					y 		= 	(n1 - val + j);
					result 	+= 	GetPixelVal(image,x,y);
				}
			}
		}
		if( n1 == 1 ) 	FirstCol = 0;
		else 			FirstCol = firstc/border;
	}
	
	return (result/border);
}

double GetPixelVal(IplImage *image, int x, int y)
{
	CvScalar s;
	if( x <= 0 || x >= image->height || y <= 0 || y >= image->width ) 
		s.val[0] = 0;
	else 
		s 	= cvGet2D(image,x,y);
	
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
	int 		y,x;
	double 		diff, norm4, var;
	double 		result = 0;
	CvScalar 	s;
	CvScalar 	mean;
    CvScalar 	stddev;
	
	cvAvgSdv( img, &mean, &stddev, NULL);
	
	for( y = 0; y < img->height; y++ )
	{
		for( x = 0; x < img->width; x++ )
		{
			s 		= 	cvGet2D(img,y,x);  		//get the (y,x) pixel value
			diff 	= 	s.val[0] - mean.val[0];
			var 	+= 	diff*diff;
			norm4 	+= 	diff*diff*diff*diff; 	//fourth moment about the mean u
		}
	}
	
	var 	/= 	(img->width * img->height);
	norm4 	/= 	(img->width * img->height);
	
	if( norm4 == 0 ) 
		return 0;
	
	result 	=  	(var / pow(norm4,0.25)) / 128; /* normalize by maximum value */ 
	return result; 
}