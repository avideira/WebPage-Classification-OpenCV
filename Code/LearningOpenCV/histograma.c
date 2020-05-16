#include "cv.h"
#include "highgui.h"
 
int main( int argc, char** argv )
{
    	IplImage* img  = cvLoadImage(argv[1], 1);
 

 	if( !img ) return -1;
 
 	IplImage* channel = cvCreateImage( cvGetSize(img), 8, 1 );
        IplImage *histograma = cvCreateImage(cvSize(300,240), 8, 3);
	cvSet(histograma, cvScalarAll(255), 0 );
 
 	CvHistogram *Hred;
 	CvHistogram *Hgreen;
 	CvHistogram *Hblue;
 
    	int hist_size = 32;     
    	float range[]={0,256};
    	float* ranges[] = { range };
     
    	float max_value = 0.0;
    	float max = 0.0;
    	float w_scale = 0.0;
 
 	Hred = cvCreateHist(1, &hist_size, CV_HIST_ARRAY, ranges, 1);
 	Hgreen = cvCreateHist(1, &hist_size, CV_HIST_ARRAY, ranges, 1);
 	Hblue = cvCreateHist(1, &hist_size, CV_HIST_ARRAY, ranges, 1);
 
 	//Vermelho como região de interesse
 	cvSetImageCOI(img,3);
  	cvCopy(img,channel);
  	cvResetImageROI(img);
 
 	//calcula histograma da imagem e guarda no array 
  	cvCalcHist( &channel, Hred, 0, NULL );
 
  	cvSetImageCOI(img,2);
  	cvCopy(img,channel);
  	cvResetImageROI(img);
  	cvCalcHist( &channel, Hgreen, 0, NULL );

  	cvSetImageCOI(img,1);
  	cvCopy(img,channel);
  	cvResetImageROI(img);
 	cvCalcHist( &channel, Hblue, 0, NULL );

        //valores minimos e máximos
        cvGetMinMaxHistValue( Hred, 0, &max_value, 0, 0 );
	cvGetMinMaxHistValue( Hgreen, 0, &max, 0, 0 );
 
 	max_value = (max > max_value) ? max : max_value;
 
 	cvGetMinMaxHistValue( Hblue, 0, &max, 0, 0 );
 
 	max_value = (max > max_value) ? max : max_value; //valor máximo dos 3 histogramas
 	
 	//Reduz para caber na imagem
        cvScale( Hred->bins, Hred->bins, ((float)histograma->height)/max_value, 0 );
 	cvScale( Hgreen->bins, Hgreen->bins, ((float)histograma->height)/max_value, 0 );
 	cvScale( Hblue->bins, Hblue->bins, ((float)histograma->height)/max_value, 0 );
                                            
 	//Reduz o limite do histograma para a largura da imagem
    	w_scale = ((float)histograma->width)/hist_size;
 
 	// Plot do histograma
    	for( int i = 0; i < hist_size; i++ )
    	{
      		cvRectangle( histograma, cvPoint((int)i*w_scale , histograma->height), cvPoint((int)(i+1)*w_scale, histograma->height - cvRound(cvGetReal1D(Hred->bins,i))), CV_RGB(255,0,0), -1, 8, 0 );
      		cvRectangle( histograma, cvPoint((int)i*w_scale , histograma->height), cvPoint((int)(i+1)*w_scale, histograma->height - cvRound(cvGetReal1D(Hgreen->bins,i))), CV_RGB(0,255,0), -1, 8, 0 );
      		cvRectangle( histograma, cvPoint((int)i*w_scale , histograma->height), cvPoint((int)(i+1)*w_scale, histograma->height - cvRound(cvGetReal1D(Hblue->bins,i))), CV_RGB(0,0,255), -1, 8, 0 );
   	 }
 
    	cvNamedWindow( "Original", 1 );
   	cvShowImage( "Original",img);
 

    	cvNamedWindow("Histograma", 1);
    	cvShowImage( "Histograma", histograma);
 
    	cvWaitKey(0);
 
 	cvDestroyWindow( "Original" );
 	cvDestroyWindow( "Histograma" );
    	cvReleaseImage( &img );
    	return 0;
}
