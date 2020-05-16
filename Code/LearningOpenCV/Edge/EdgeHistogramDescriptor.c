/*
	File:   edgehist.c
 	Author: António Videira
	Created on 25 February, 2013
	Function that extract the Edge Histogram from an image.
	Compute 4 directional edges and 1 non-directional edge
	The image is split into 4x4 non-overlapping rectangular regions.
	In each region, a 1x5 edge orientation histogram is computed (horizontal, 
	vertical, 2 diagonals and 1 non-directional).
	
*/
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define ABS(a)((a<0)?-(a):(a))
#define	NoEdge						0
#define	vertical_edge				1
#define	horizontal_edge				2
#define	non_directional_edge		3
#define	diagonal_45_degree_edge		4
#define	diagonal_135_degree_edge	5

IplImage *imageRGB;
IplImage *gray;
IplImage *img;

double 	FirstBlock		(int i, int j, int block_size);
double 	SecondBlock		(int i, int j, int block_size);
double 	ThirdBlock		(int i, int j, int block_size);
double 	FourthBlock		(int i, int j, int block_size);
int		GetBlockSize	(unsigned int width, unsigned int height);
int 	GetEdgeFeature	(int i, int j, unsigned short int block_size);
double 	EdgeCalculation	(unsigned int image_width, unsigned int image_height, unsigned short int block_size, double *Local_Edge);

int main(int argc, char** argv)
{
	int i;
	unsigned int imgwidth, imgheight;
	unsigned short int block_size;
	double EHDTable[80];
	
	imageRGB	= 	cvLoadImage(argv[1], 1);
	gray 		= 	cvCreateImage(cvGetSize(imageRGB), 8, 1);
	cvCvtColor(imageRGB, gray, CV_BGR2GRAY );
	
	imgheight 	= 	gray->height;
	imgwidth  	= 	gray->width;
	block_size 	= 	GetBlockSize(imgwidth, imgheight);
	EdgeCalculation(imgwidth, imgheight, block_size, EHDTable);
	
	int k;
    for (k = 0; k < 80; k++) {
        printf(" %f \n" , EHDTable[k]);
    }
	
	cvWaitKey(0);
	cvNamedWindow( "gray IMAGE", 1 );
	cvShowImage( "gray IMAGE", gray);
	
}

double EdgeCalculation(unsigned int image_width, unsigned int image_height, unsigned short int block_size, double *Local_Edge)
{
	int i, j, k;
	int EdgeTypeOfBlock = 0, local_index = 0;
	int Count_Local[16];
	
	for (i = 0; i < 16; i++)
    {
        Count_Local[i] = 0;
    }
	
	for(i=0; i<=image_height-block_size; i+=block_size){
		for(j=0; j<=image_width-block_size; j+=block_size){
			
			local_index = (int)(j*4/image_width)+(int)(i*4/image_height)*4;
			Count_Local[local_index]++;	
			EdgeTypeOfBlock = GetEdgeFeature(i,j,block_size);	
			
			switch( EdgeTypeOfBlock ) {
			case NoEdge:					break;
			case vertical_edge:				Local_Edge[local_index*5]++;	break;
			case horizontal_edge:			Local_Edge[local_index*5+1]++;	break;
			case diagonal_45_degree_edge:	Local_Edge[local_index*5+2]++;	break;
			case diagonal_135_degree_edge:	Local_Edge[local_index*5+3]++;	break;
			case non_directional_edge:		Local_Edge[local_index*5+4]++;	break;
			} 
		}//for(i)
        for (k = 0; k < 80; k++)
        {
			Local_Edge[k] /= Count_Local[(int)k / 5]; 
		}
        //return (Local_Edge);
	}
	
}

int GetEdgeFeature(int i, int j, unsigned short int block_size)
{
	float threshold = 0.01; //perguntar ao prof.
	int e_index;
	double e_h, e_v, e_45, e_135, e_m, e_max;
	double strengths[5];
	double EdgeFilters[5][4] = {{1.0, -1.0, 1.0, -1.0},
								{1.0, 1.0, -1.0, -1.0},
								{sqrt(2), 0.0, 0.0, -sqrt(2)},
								{0.0, sqrt(2), -sqrt(2), 0.0},
								{2.0, -2.0, -2.0, 2.0}};

	//Calculate the average of gray levels in each sub-block of the (i,j)th imageblock
	double average[4] = {FirstBlock(i,j,block_size), SecondBlock(i,j,block_size), ThirdBlock(i,j,block_size), FourthBlock(i,j,block_size)};
	printf("average: '%f', '%f', '%f', '%f' \n\n",average[0],average[1],average[2],average[3] );
	
	int e,k;
	for (e = 0; e < 5; e++){
        for (k = 0; k < 4; k++){
            strengths[e] += average[k] * EdgeFilters[e][k];
        }
        strengths[e] = fabs(strengths[e]);
    }
	printf("strengths: '%f', '%f', '%f', '%f', '%f \n\n",strengths[0],strengths[1],strengths[2],strengths[3],strengths[4]);
	
	e_v = strengths[0];
	e_h = strengths[1];
	e_45 = strengths[2];
	e_135 = strengths[3];
	e_m = strengths[4];
	
	e_max = e_v;
	e_index = vertical_edge;
	if(e_h > e_max){
		e_max = e_h;
		e_index = horizontal_edge;
	}
	if(e_45 > e_max){
		e_max = e_45;
		e_index = diagonal_45_degree_edge;
	}
	if(e_135 > e_max){
		e_max = e_135;
		e_index = diagonal_135_degree_edge;
	}
	if(e_m > e_max){
		e_max = e_m;
		e_index = non_directional_edge;
	}
	if(e_max < threshold)
		e_index = NoEdge;
	return(e_index);
}

/*
	height e width representam o tamanho horizontal
	e vertical da imagem, respectivamente.
*/
int GetBlockSize(unsigned int width, unsigned int height)
{
	unsigned short int block_size;
	unsigned short int x;
	int num_block = 16; //numero de blocos desejados
	
	x = sqrt((width*height)/num_block);
	block_size = abs(x/2)*2;
	
	if(block_size<2) block_size = 2;
	
	return block_size;
}
double FirstBlock(int i, int j, int block_size)
{
    int m,n;
	double average_brightness = 0;
	CvScalar s;
	for (m = i; m < (block_size/2)+i; m++) {
        for (n = j; n < (block_size/2)+j; n++) {
            s = cvGet2D(gray,m,n);
            average_brightness = average_brightness + s.val[0];
        }
    }
      
    double bs = block_size * block_size;
    double div = 4 / bs;
    average_brightness = average_brightness * div;
    return average_brightness;
 }
double SecondBlock(int i, int j, int block_size)
{
    int m,n;
	double average_brightness = 0;
	CvScalar s;
	for (m = i; m < (block_size/2)+i; m++) {
        for (n = (block_size/2)+j; n < block_size+j; n++) {
 			s = cvGet2D(gray,m,n);
            average_brightness = average_brightness + s.val[0];
		}
    }
      
    double bs = block_size * block_size;
    double div = 4 / bs;
    average_brightness = average_brightness * div;
    return average_brightness;
 }
double ThirdBlock(int i, int j, int block_size)
{
    int m,n;
	double average_brightness = 0;
	CvScalar s;
	for (m = (block_size/2)+i; m < (block_size+i); m++) {
        for (n = j; n < (block_size/2)+j; n++) {
 			s = cvGet2D(gray,m,n);
            average_brightness = average_brightness + s.val[0];
		}
    }
      
    double bs = block_size * block_size;
    double div = 4 / bs;
    average_brightness = average_brightness * div;
    return average_brightness;
 }
double FourthBlock(int i, int j, int block_size)
{
    int m,n;
	double average_brightness = 0;
	CvScalar s;
	for (m = (block_size/2)+i; m < block_size+i; m++) {
        for (n = (block_size/2)+j; n < block_size+j; n++) {
 			s = cvGet2D(gray,m,n);
            average_brightness = average_brightness + s.val[0];
		}
    }
    double bs = block_size * block_size;
    double div = 4 / bs;
    average_brightness = average_brightness * div;
    return average_brightness;
 }
