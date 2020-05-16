/*
	File:   rgb2hsv.c
 	Author: António Videira
	Created on 18 February, 2013
	Function that converts an image RGB to HSV.
	Computes the Value Histogram.
	Indicates how many pixels is in each bin of the histogram.
	OpenCV uses H: 0 - 180, S: 0 - 255, V: 0 - 255
*/
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
IplImage *imageRGB = 0;
IplImage *imageHSV = 0;
IplImage *hue, *sat, *val;
IplImage *hist = 0;
CvHistogram *vhist;

IplImage* convertRGBtoHSV(const IplImage *imageRGB)
{
	int depth;
    CvSize size;
	/* Create a hsv image with 3 channels and hue, sat e val with 1 channel. All with the same size */
    size = cvGetSize(imageRGB);
    depth = imageRGB->depth;
    hue = cvCreateImage(size, depth, 1);
    sat = cvCreateImage(size, depth, 1);
    val = cvCreateImage(size, depth, 1);
    imageHSV = cvCreateImage(size, depth, 3);
    cvZero(hue);
    cvZero(sat);
    cvZero(val);
    cvZero(imageHSV);
	
    /* Convert from Red-Green-Blue to Hue-Saturation-Value */
    cvCvtColor( imageRGB, imageHSV, CV_BGR2HSV );
 
    /* Split hue, saturation and value of hsv on them */
    cvSplit(imageHSV, hue, sat, val, 0);
}

int DrawHistogram(int bins){
	int normalized, value;
	int hist_size[] = {bins}; //Just 1 dimension
	/*Range of the histogram*/
	float range[]={0,255}; // value varies from 0 (dark) to 255 (light)
	float* ranges[] = { range };
	float max_value = 0.0;
    float w_scale = 0.0;
	//planes to obtain the histogram, in this case just one
	IplImage* planes[] = {val};
	//create histograms of the V channel
	vhist = cvCreateHist(1,hist_size,CV_HIST_ARRAY,ranges,1);
	//calcula histograma da imagem e guarda no array 
  	cvCalcHist(planes, vhist, 0, NULL );
	//valores minimos e máximos
    cvGetMinMaxHistValue(vhist, 0, &max_value, 0, 0 ); //valor máximo do histograma
	//create an 8 bits single channel image to hold the histogram
    //paint it white
    hist = cvCreateImage(cvSize(bins, 50),8,1);
    cvRectangle(hist, cvPoint(0,0), cvPoint(256,50), CV_RGB(255,255,255),-1);
	//draw the histogram 
    for(int i=0; i < bins; i++){
            value = cvQueryHistValue_1D(vhist, i);
            normalized = cvRound(value*50/max_value);
            cvLine(hist,cvPoint(i,50), cvPoint(i,50-normalized), CV_RGB(0,0,0));

	}
	return 0;
}

int calcPixels(int bins){
	int row, col;
	int pixels;
	int somatorio= 0;
	int min=0, max=0;
	CvScalar s;
	int pixelstotal = val->imageSize;
	printf("Numero de Pixels na imagem = %d \n", pixelstotal);
	for(int i=1; i < bins+1; i++){	
		pixels = 0;
		max = ((i*8)-1);
		min = max - 7;
		if(min <0) min = 0;
		for(int row = 0; row < val->height; row++)
		{
			for (int col = 0; col < val->width; col++)
			{
				s=cvGet2D(val,row,col);
				if( s.val[0] >= min && s.val[0] <= max){
					pixels++;
				}
			}
		}
		printf("Números de pixels entre '%d' e '%d' = '%d'\n ",min, max, pixels);
		somatorio = somatorio + pixels;
		
	}
	printf("total de pixels contados = %d\n", somatorio);
	return 0;
}

int main( int argc, char** argv ) 
{
	int bins = 32;//hold 32 levels
	imageRGB  = cvLoadImage(argv[1], 1);
	
	convertRGBtoHSV(imageRGB);
	DrawHistogram(bins);
	calcPixels(bins);
	
	//cvNamedWindow( "RGB IMAGE", 1 );
	//cvShowImage( "RGB IMAGE", imageRGB);
	cvNamedWindow( "HSV IMAGE", 1 );
	cvShowImage( "HSV IMAGE", imageHSV );
	//cvNamedWindow("Hue Channel", 1);
	//cvShowImage( "Hue Channel", hue);
	//cvNamedWindow("Saturation Channel", 1);
	//cvShowImage( "Saturation Channel", sat);
	cvNamedWindow("Value Channel", 1);
	cvShowImage( "Value Channel", val);
	cvNamedWindow( "Value Histogram", 0);
	cvResizeWindow("Value Histogram",400, 400);
	cvShowImage( "Value Histogram", hist);

	cvWaitKey(0);
	//Liberta a memoria
 	//cvDestroyWindow( "RGB IMAGE" );
 	cvDestroyWindow( "HSV IMAGE" );
	//cvDestroyWindow( "Value Channel" );
	//cvDestroyWindow( "Hue Channel" );
	cvDestroyWindow( "Saturation Channel" );
	cvDestroyWindow( "Value Histogram" );

	cvReleaseImage(&imageRGB);
	cvReleaseImage(&imageHSV);	
	cvReleaseImage(&hue);
	cvReleaseImage(&sat);
	cvReleaseImage(&val);
}
