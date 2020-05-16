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
#include "edgehistogram.h"
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define		Desired_Num_of_Blocks	2048

#define 	ABS(a)((a<0)?-(a):(a))
#define		NoEdge						0
#define		vertical_edge				1
#define		horizontal_edge				2
#define		non_directional_edge		3
#define		diagonal_45_degree_edge		4
#define		diagonal_135_degree_edge	5

void ExtractEdgeHist( double* edgevector, IplImage *image )
{
	unsigned int 		imgwidth, imgheight;
	unsigned short int 	block_size;
	//double 				EHDTable[80];
	
	imgheight 	= 	image->height;
	imgwidth  	= 	image->width;
	
	block_size 	= 	GetBlockSize( imgwidth, imgheight );
	
	EdgeCalculation( imgwidth, imgheight, block_size, edgevector, image );
}

void EdgeCalculation( unsigned int width, unsigned int height, unsigned short int block_size, double *Local_Edge, IplImage* img )
{
	int 	i, j, k;
	int 	Count_Local[16];
	int 	EdgeTypeOfBlock = 0; 
	int 	local_index = 0;
	long 	LongTyp_Local_Edge[80];
	
	memset(Count_Local, 0, 16*sizeof(int));
	memset(LongTyp_Local_Edge, 0, 80*sizeof(long));
	
	for( i = 0; i <= (height-block_size); i += block_size )
	{
		for( j = 0; j <= (width-block_size); j += block_size)
		{	
			local_index 	= (int)(j*4/width) + (int)(i*4/height)*4;
			EdgeTypeOfBlock = GetEdgeFeature(i,j,block_size,img);	
			
			Count_Local[local_index]++;
			
			switch( EdgeTypeOfBlock ) 
			{
			case 	NoEdge:						break;
			case 	vertical_edge:				LongTyp_Local_Edge[local_index*5]++;	break;
			case 	horizontal_edge:			LongTyp_Local_Edge[local_index*5+1]++;	break;
			case 	diagonal_45_degree_edge:	LongTyp_Local_Edge[local_index*5+2]++;	break;
			case 	diagonal_135_degree_edge:	LongTyp_Local_Edge[local_index*5+3]++;	break;
			case 	non_directional_edge:		LongTyp_Local_Edge[local_index*5+4]++;	break;
			} 
		}
	}
	
    for ( k = 0; k < 80; k++ )
    {
		Local_Edge[k] = (double)LongTyp_Local_Edge[k]/Count_Local[(int)k / 5]; 
	}
}

int GetEdgeFeature( int i, int j, unsigned short int block_size, IplImage* img )
{ 
	int e_index,e,k;
	int threshold = 11;
	double e_h, e_v, e_45, e_135, e_m, e_max;
	double strengths[5];
	double EdgeFilters[5][4] = {{1.0, -1.0, 1.0, -1.0},
								{1.0, 1.0, -1.0, -1.0},
								{sqrt(2), 0.0, 0.0, -sqrt(2)},
								{0.0, sqrt(2), -sqrt(2), 0.0},
								{2.0, -2.0, -2.0, 2.0}};

	//Calculate the average of gray levels in each sub-block of the (i,j)th imageblock
	double average[4] = { FirstBlock(i,j,block_size,img), SecondBlock(i,j,block_size,img), ThirdBlock(i,j,block_size,img), FourthBlock(i,j,block_size,img) };
	
	for ( e = 0; e < 5; e++ )
	{
        for ( k = 0; k < 4; k++ )
		{
            strengths[e] += average[k] * EdgeFilters[e][k];
        }
        strengths[e] = fabs(strengths[e]);
    }
		
	e_v 	= strengths[0];
	e_h 	= strengths[1];
	e_45 	= strengths[2];
	e_135 	= strengths[3];
	e_m 	= strengths[4];
	e_max 	= e_v;
	e_index = vertical_edge;
	
	if( e_h > e_max )
	{
		e_max 	= e_h;
		e_index = horizontal_edge;
	}
	if(	e_45 > e_max )
	{
		e_max 	= e_45;
		e_index = diagonal_45_degree_edge;
	}
	if( e_135 > e_max )
	{
		e_max 	= e_135;
		e_index = diagonal_135_degree_edge;
	}
	if(e_m > e_max)
	{
		e_max	= e_m;
		e_index = non_directional_edge;
	}
	if( e_max < threshold )
		e_index = NoEdge;
		
	return(e_index);
}

int GetBlockSize( unsigned int width, unsigned int height )
{
	unsigned short int block_size;
	unsigned short int x;
	
	int num_block = Desired_Num_of_Blocks; //numero de blocos desejados
	
	x 			= 	sqrt((width*height)/num_block);
	block_size 	= 	abs(x/2)*2;
	
	if( block_size < 2 ) 
		block_size = 2;
	
	return block_size;
}

double FirstBlock( int i, int j, int block_size, IplImage* img )
{
    int 		m, n;
	double 		bs, div;
	double 		average_brightness = 0;
	CvScalar 	s;
	
	for ( m = i; m < (block_size/2) + i; m++ )
	{
        for ( n = j; n < (block_size/2) + j; n++ )
		{
            s 	= 	cvGet2D(img,m,n);
            average_brightness 	= 	average_brightness + s.val[0];
        }
    }
      
	bs 		= 	block_size * block_size;
    div 	= 	4 / bs;
    average_brightness 	= 	average_brightness * div;
	
    return average_brightness;
 }

double SecondBlock( int i, int j, int block_size, IplImage* img )
{
    int 		m, n;
	double 		bs, div;
	double 		average_brightness = 0;
	CvScalar 	s;
	
	for ( m = i; m < (block_size/2) + i; m++ )
	{
        for ( n = (block_size/2) + j; n < block_size + j; n++ )
		{
 			s 	=  	cvGet2D(img,m,n);
            average_brightness 	= 	average_brightness + s.val[0];
		}
    }
      
    bs 		= 	block_size * block_size;
    div 	= 	4 / bs;
    average_brightness 	= 	average_brightness * div;
	
    return average_brightness;
 }

double ThirdBlock( int i, int j, int block_size, IplImage* img )
{
    int 		m, n;
	double 		bs, div;
	double 		average_brightness = 0;
	CvScalar 	s;
	
	for ( m = (block_size/2) + i; m < (block_size + i); m++ )
	{
        for ( n = j; n < (block_size/2) + j; n++ )
		{
 			s 	= 	cvGet2D( img, m, n );
            average_brightness  =  average_brightness + s.val[0];
		}
    }
      
    bs 		= 	block_size * block_size;
    div 	= 	4 / bs;
    average_brightness 	= 	average_brightness * div;
	
    return average_brightness;
 }

double FourthBlock( int i, int j, int block_size, IplImage* img )
{
    int 		m, n;
	double 		bs, div;
	double 		average_brightness = 0;
	CvScalar 	s;
	
	for ( m = (block_size/2) + i; m < block_size + i; m++ )
	{
        for ( n = (block_size/2) + j; n < block_size + j; n++ )
		{
 			s 	= 	cvGet2D( img, m, n);
            average_brightness 	= 	average_brightness + s.val[0];
		}
    }
    bs 		= 	block_size * block_size;
    div 	= 	4 / bs;
    average_brightness 	= 	average_brightness * div;
	
    return average_brightness;
 }