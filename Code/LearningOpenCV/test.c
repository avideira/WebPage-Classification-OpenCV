


#include <cv.h>
#include <highgui.h>

using namespace std;

IplImage* image = 0;
IplImage* imgHistogram = 0;
IplImage* gray = 0;
CvHistogram* hist;


int main(int argc, char** argv) {


    if (argc != 2 || !(image = cvLoadImage(argv[1])))
        return -1;

    //size of the histogram -1D histogram
    int bins = 256;
    int hsize[] = {bins};
    //max and min value of the histogram
    float max_value = 0, min_value = 0;
    //value and normalized value
    float value;hist
    int normalized;
    //ranges - grayscale 0 to 256
    float xranges[] = {0, 256};
    float* ranges[] = {xranges};

    //create an 8 bit single channel image to hold a
    //grayscale version of the original picture
    gray = cvCreateImage(cvGetSize(image), 8, 1);
    cvCvtColor(image, gray, CV_BGR2GRAY);

    //Create 3 windows to show the results
    cvNamedWindow("original", 1);
    cvNamedWindow("gray", 1);
    cvNamedWindow("histogram", 1);

    //planes to obtain the histogram, in this case just one
    IplImage * planes[] = {gray};
    //get the histogram and some info about it
    hist = cvCreateHist(1, hsize, CV_HIST_ARRAY, ranges, 1);
    cvCalcHist(planes, hist, 0, NULL);

    cvGetMinMaxHistValue(hist, &min_value, &max_value);
    printf("min: %f, max: %f\n", min_value, max_value);

    //create an 8 bits single channel image to hold the histogram
    //paint it white
    imgHistogram = cvCreateImage(cvSize(bins, 200), 8, 1);
    cvRectangle(imgHistogram, cvPoint(0, 0), cvPoint(256, 200), CV_RGB(255, 255, 255), -1);
    //draw the histogram
    for (int i = 0; i < bins; i++) {
        value = cvQueryHistValue_1D(hist, i);
        normalized = cvRound(value * 200 / max_value);
        cvLine(imgHistogram, cvPoint(i, 200), cvPoint(i, 200 - normalized), CV_RGB(0, 0, 0));
        printf("%d\n", normalized);
    }

    //show the image results
    cvShowImage("original", image);
    cvShowImage("gray", gray);
    cvShowImage("histogram", imgHistogram);
    cvWaitKey();
    return (EXIT_SUCCESS);
}
