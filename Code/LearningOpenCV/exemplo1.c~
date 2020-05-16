#include "cv.h" //main OpenCVfunctions
#include "highgui.h" //OpenCVGUI functions

int main( int argc, char** argv ) {
	IplImage* img = cvLoadImage( argv[1] );
	cvNamedWindow( "Imagem", CV_WINDOW_AUTOSIZE );
	cvShowImage( "Imagem", img );
	cvWaitKey(0);
	cvReleaseImage( &img );
	cvDestroyWindow( "Example1" );
}

