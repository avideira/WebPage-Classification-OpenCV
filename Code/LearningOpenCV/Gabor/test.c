#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>

using namespace cv;

/** @function main */
int main ( int argc, char** argv )
{
  /// Declare variables
  Mat src, dst;

  Mat kernel;
  Point anchor;
  double delta;
  int ddepth;
  int kernel_size;
  char* window_name = "filter2D Demo";

  int c;

  /// Load an image
  src = imread( argv[1] );

  if( !src.data )
  { return -1; }
  
  int row, col;
  int height = src->height;
  int width =  src->width;
  

   for(row=0; row<height; row++) {
        for(col=0; col<width; col++) {
            x_theta = row*sin(thetaArray[row*width + col]) + col*cos(thetaArray[row*width + col]);
            y_theta = col*sin(thetaArray[row*width + col]) - row*cos(thetaArray[row*width + col]);
            
            exp_temp = -0.5*(((x_theta*x_theta)/((double)sigmaX*(double)sigmaX)) + ((y_theta*y_theta)/((double)sigmaY*(double)sigmaY)));
            
            gaborArray[row*width + col] = exp(exp_temp) * cos(2.0*M_PI*(1/ridgeFrequency)*x_theta);
        }
    }
    
    /*-----------------------------------
                Convolution
     -----------------------------------*/
    for(row=2; row<height-2; row++) {
        for(col=2; col<width-2; col++) {
            gaborTemp = 0;
            for (I=-2; I<=2; I++) {
                for (J=-2; J<=2; J++) {
                    gaborTemp += pixels[(row+I) + (col+J)*width] * gaborArray[(row+I) + (col+J)*width];
                }
            }
            imageAfterGabor[row*width + col] = gaborTemp;
        }
    }

  return 0;
}