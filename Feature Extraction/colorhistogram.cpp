/*
	File:   colorhistogram.c
 	Author: António Videira
*/

#include "colorhistogram.h"
#include <cv.h>
//#include "opencv2/imgproc/imgproc.hpp"
#include <highgui.h>
#include <stdio.h>

using namespace cv;
using namespace std;

void ExtractColorHist	( IplImage *imageRGB , double *RGBdescriptor )
{
	int Colorbins 	= 32;
 	// IplImage 	*red, *green, *blue;
    // CvSize 		size;
	// int 		depth;
	
    // size 	= 	cvGetSize(imageRGB);
    // depth 	= 	imageRGB->depth;
    // red 	= 	cvCreateImage(size, depth, 1);
    // green 	= 	cvCreateImage(size, depth, 1);
    // blue 	= 	cvCreateImage(size, depth, 1);	
    // cvZero( red );
    // cvZero( green );
    // cvZero( blue );
	// cvSplit( imageRGB, blue, green, red, 0 ); 
	double Rgbdesc[Colorbins];
	IplImage *component = RGBtoHSV( imageRGB );
	//IplImage *component = convertImageRGBtoHSV( imageRGB );
	CalcPixels( Colorbins, component, Rgbdesc );
	
	//normalize vector - values between 0 and 1.
	int i;
	double norm = 0.0;
   
	for(i=0; i < Colorbins; i++)
	{
		norm += Rgbdesc[i]*Rgbdesc[i];
	}

	norm = sqrt( norm );

	for(i=0; i < Colorbins; i++)
	{
		RGBdescriptor[i] = Rgbdesc[i] / norm;
 
	}

	// cvReleaseImage( &red );
	// cvReleaseImage( &green );
	// cvReleaseImage( &blue );
	cvReleaseImage( &component );
}

/*
	Indicates how many pixels is in each bin of the histogram.
*/
void CalcPixels( int bins, IplImage* img, double *Rgbdesc )
{
	int 		row, col, pixels, i, min, max;
	int 		value = (256/bins);
	double 		numPixels;
	float 		somatorio= 0;
	CvScalar 	s;

		
		
	min 		= 	0; 
	max 		= 	0;	
	numPixels 	= 	img->imageSize;
	
	for( i = 1; i < bins+1; i++ )
	{	
		pixels = 0;
		Rgbdesc[i-1] = 0;
		
		max 	= 	((i*value)-1);
		min 	= 	max - (value-1);
		
		if( min < 0 ) min = 0;
		
		for( row = 0; row < img->height; row++ )
		{
			for ( col = 0; col < img->width; col++ )
			{
				s	=	cvGet2D(img, row, col);
				if( s.val[0] >= min && s.val[0] <= max){
					pixels++;
				}
			}
		}
		Rgbdesc[i-1] = pixels;
		//printf("bin %d =  %lf \n" , i-1 ,Rgbdesc[i-1]);
		//RGBhistDescriptor[i-1] = RGBhistDescriptor[i-1]/numPixels;
	}
}
/*
	Function that converts an image RGB to HSV.
*/
IplImage* RGBtoHSV(const IplImage *imgRGB)
{
	IplImage 	*imgHSV, *hue, *sat, *val;
    CvSize 		size;
	int 		depth;
	
	/* Create a hsv image with 3 channels and hue, sat e val with 1 channel. All with the same size */
    size 	= 	cvGetSize(imgRGB);
    depth 	= 	imgRGB->depth;
    hue 	= 	cvCreateImage(size, depth, 1);
    sat 	= 	cvCreateImage(size, depth, 1);
    val 	= 	cvCreateImage(size, depth, 1);
    imgHSV 	= 	cvCreateImage(size, depth, 3);
	
    cvZero( hue );
    cvZero( sat );
    cvZero( val );
    cvZero( imgHSV );
	
    /* Convert from Red-Green-Blue to Hue-Saturation-Value */
    cvCvtColor( imgRGB, imgHSV, CV_BGR2HSV );
	
	/* OpenCV uses H: 0 - 180, S: 0 - 255, V: 0 - 255 */
    /* Split hue, saturation and value of hsv on them */
	
    cvSplit( imgHSV, hue, sat, val, 0 );
	
	cvReleaseImage(&val);	
	cvReleaseImage(&sat);
	cvReleaseImage(&imgHSV);	
	
	return hue;
}

IplImage* convertImageRGBtoHSV(const IplImage *imageRGB)
{
	float fR, fG, fB;
	float fH, fS, fV;
	const float FLOAT_TO_BYTE = 255.0f;
	const float BYTE_TO_FLOAT = 1.0f / FLOAT_TO_BYTE;

	// Create a blank HSV image
	IplImage *imageHSV = cvCreateImage(cvGetSize(imageRGB), 8, 3);
	if (!imageHSV || imageRGB->depth != 8 || imageRGB->nChannels != 3) {
		printf("ERROR in convertImageRGBtoHSV()! Bad input image.\n");
		exit(1);
	}

	int h = imageRGB->height;		// Pixel height.
	int w = imageRGB->width;		// Pixel width.
	int rowSizeRGB = imageRGB->widthStep;	// Size of row in bytes, including extra padding.
	char *imRGB = imageRGB->imageData;	// Pointer to the start of the image pixels.
	int rowSizeHSV = imageHSV->widthStep;	// Size of row in bytes, including extra padding.
	char *imHSV = imageHSV->imageData;	// Pointer to the start of the image pixels.
	for (int y=0; y<h; y++) {
		for (int x=0; x<w; x++) {
			// Get the RGB pixel components. NOTE that OpenCV stores RGB pixels in B,G,R order.
			uchar *pRGB = (uchar*)(imRGB + y*rowSizeRGB + x*3);
			int bB = *(uchar*)(pRGB+0);	// Blue component
			int bG = *(uchar*)(pRGB+1);	// Green component
			int bR = *(uchar*)(pRGB+2);	// Red component

			// Convert from 8-bit integers to floats.
			fR = bR * BYTE_TO_FLOAT;
			fG = bG * BYTE_TO_FLOAT;
			fB = bB * BYTE_TO_FLOAT;

			// Convert from RGB to HSV, using float ranges 0.0 to 1.0.
			float fDelta;
			float fMin, fMax;
			int iMax;
			// Get the min and max, but use integer comparisons for slight speedup.
			if (bB < bG) {
				if (bB < bR) {
					fMin = fB;
					if (bR > bG) {
						iMax = bR;
						fMax = fR;
					}
					else {
						iMax = bG;
						fMax = fG;
					}
				}
				else {
					fMin = fR;
					fMax = fG;
					iMax = bG;
				}
			}
			else {
				if (bG < bR) {
					fMin = fG;
					if (bB > bR) {
						fMax = fB;
						iMax = bB;
					}
					else {
						fMax = fR;
						iMax = bR;
					}
				}
				else {
					fMin = fR;
					fMax = fB;
					iMax = bB;
				}
			}
			fDelta = fMax - fMin;
			fV = fMax;				// Value (Brightness).
			if (iMax != 0) {			// Make sure its not pure black.
				fS = fDelta / fMax;		// Saturation.
				float ANGLE_TO_UNIT = 1.0f / (6.0f * fDelta);	// Make the Hues between 0.0 to 1.0 instead of 6.0
				if (iMax == bR) {		// between yellow and magenta.
					fH = (fG - fB) * ANGLE_TO_UNIT;
				}
				else if (iMax == bG) {		// between cyan and yellow.
					fH = (2.0f/6.0f) + ( fB - fR ) * ANGLE_TO_UNIT;
				}
				else {				// between magenta and cyan.
					fH = (4.0f/6.0f) + ( fR - fG ) * ANGLE_TO_UNIT;
				}
				// Wrap outlier Hues around the circle.
				if (fH < 0.0f)
					fH += 1.0f;
				if (fH >= 1.0f)
					fH -= 1.0f;
			}
			else {
				// color is pure Black.
				fS = 0;
				fH = 0;	// undefined hue
			}

			// Convert from floats to 8-bit integers.
			int bH = (int)(0.5f + fH * 255.0f);
			int bS = (int)(0.5f + fS * 255.0f);
			int bV = (int)(0.5f + fV * 255.0f);

			// Clip the values to make sure it fits within the 8bits.
			if (bH > 255)
				bH = 255;
			if (bH < 0)
				bH = 0;
			if (bS > 255)
				bS = 255;
			if (bS < 0)
				bS = 0;
			if (bV > 255)
				bV = 255;
			if (bV < 0)
				bV = 0;

			// Set the HSV pixel components.
			uchar *pHSV = (uchar*)(imHSV + y*rowSizeHSV + x*3);
			*(pHSV+0) = bH;		// H component
			*(pHSV+1) = bS;		// S component
			*(pHSV+2) = bV;		// V component
		}
	}
	return imageHSV;
}