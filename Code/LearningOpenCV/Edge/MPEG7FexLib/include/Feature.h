/***************************************************************
 * Name:      Feature.h
 * Purpose:   Extract low-level MPEG-7 descriptors (uses OpenCV 1.0 for image handling)
 * Author:    Muhammet Bastan (bastan@cs.bilkent.edu.tr)
 * Created:   2008
 * Copyright: Muhammet Bastan (http://www.cs.bilkent.edu.tr/~bastan)
 * License:
 * Update: 2009-11-08
 * Update: 2009-11-18
 * Update: 2010-02-20
 *
 * TODO: modify the Feature class or provide a new class which
 *       does not use static functions, for efficiency in memory management
 **************************************************************/

/************************************************************************************
					How to use this class
					---------------------
1. include this header:		#include "Feature.h"

2. instantiate a Frame object: Frame* frame = new Frame( width, height, true, false, false );			// width-height of the input image/frame
   set the image
                frame->setImage( img );								// IplImage* img: input image
    or use this:
                Frame* frame = new Frame( img );					// IplImage* img: input image

3.a) Extracting descriptors from the whole frame:
		XM::ColorStructureDescriptor* csd = Feature::getColorStructureD(frame);
		use the descriptor values in csd ( have a look at the descriptor classes under ./Descriptors/ directory)

NOTE: use the namespace XM to refer to the descriptors rather than declaring "using namespace XM",
      it may mess up with other namespaces!

3.b) Extracting descriptors from an arbitrary region - need to set the region mask
		set the mask for the region:
            frame->setMaskAll( IplImage* mask, int regionID, 255, 0 );	// regionID: ID/value of the region in the mask image (1,2,3,..)
		compute the feature:
            XM::ColorStructureDescriptor* csd = Feature::getColorStructureD(frame);

4. when you are done with the computed descriptor, relase it:
            delete csd;		(avoid memory leakage)

IMPORTANT NOTES:

1. If you set a mask for the frame to extract features from arbitrary regions, and after that, if you want to extract features
   from the whole frame you need to reset the mask!
        Use: frame->resetMaskAll();
   Otherwise you will continue to use the mask..
   Therefore, it is better (more efficient) to extract the global features from the whole frame first,
   and then set the mask for arbitrary regions..

2. Homogeneous Texture Descriptor, Feature::getHomogeneousTextureD, uses a single channel grayscale image as input,
   therefore you need to convert your color image to grayscale first.

   Suppose you have the RGB color image IplImage* img,
			// create a single channel IplImage to store the grayscale image
			IplImage* gray = cvCreateImage( cvGetSize(img), 8, 1 );
			// convert the input image from RGB to gray
			cvCvtColor( img, gray, CV_BGR2GRAY );
			// Create a frame (with only the gray image, grayFlag = true, see Frame.h)
			Frame* frame = new Frame( width, height, false, true, false );
			// set the grayscale image of the frame
			frame->setGray( gray );
			// extract the descriptor
			XM::HomogeneousTextureDescriptor* htd = Feature::getHomogeneousTextureD( frame, true );	// true:full layer
            // do whatever you like with `htd`
            // ...
            // release the descriptor
            delete htd;

 ************************************************************************************/

#ifndef _FEATURE_H_
#define _FEATURE_H_

#include <vector>

#include "Frame.h"

#include "Descriptors.h"

typedef enum
{
    MP7_CSD, MP7_SCD, MP7_DCD, MP7_CLD,
    MP7_EHD, MP7_HTD, MP7_GOF, MP7_FRD, MP7_MAc
} MP7FeatureType;


class Feature
{
public:

/* 	///--------------------------------------------------
	///           MPEG-7 Color Descriptors
	///--------------------------------------------------

	// Color Structure Descriptor (CSD)
	// input: 3-channel RGB color image (f->image)
	// descSize: descriptor size (allowable values: 32, 64, 128, 256)
	// default descriptor size: 64
	static XM::ColorStructureDescriptor* getColorStructureD( Frame* f, int descSize = 64 );

	// Scalable Color Descriptor (SCD)
	// input: 3-channel RGB color image (f->image)
	// Frame 'f' shoud have 'mask' allocated
	// note: if maskFlag = true, it sets all pixels of the mask to value 255 (whole image),
	//       extracts the descriptor and at the end detaches the mask from f->image  again
	//       If maskFlag = false, you need to set the mask before you call this functions -
	//       So, better use maskFlag = true to extract the descriptor from the whole image/frame
	//       If the description is to be extracted from a region, you need to set the mask for the region anyway,
	//       set the mask explicitly using a foreground value of 255, and use maskFlag = false
	// numCoeff = descriptor size (allowable values:16, 32, 64, 128, 256)
	static XM::ScalableColorDescriptor* getScalableColorD( Frame* f,  bool maskFlag = true, int numCoeff = 256, int bitPlanesDiscarded = 0);

	// GoFGoP color descriptor (for a group of frames, such as shots) (GoFGoP)
	// input: vector of 3-channel RGB color images (f->image)
	// instead of this function use the GoF class defined below! It is easier to use.
	// Output is a Scalable Color descriptor
	static XM::ScalableColorDescriptor* getGoFColorD( std::vector<IplImage*> frames, int numCoeff = 256, int bitPlanesDiscarded = 0 );

	// Dominant Color Descriptor (DCD)
	// input: 3-channel RGB color image (f->image)
	// normalize: normalize values to MPEG-7 range (color:0-32, variance:0,1, weight:0-32)?
	//            if normalize = false, descriptor values are not normalized,
	//            => (color:RGB values[0-255], variance:as computed, weight:0-100 percent)
	// variance: compute the variance?
	// spatial: compute the spatial coherency
	// bin1, bin2, bin3: bin numbers to quantize the dominat color values
	static XM::DominantColorDescriptor* getDominantColorD( Frame* f, bool normalize = true, bool variance = true, bool spatial = true, int bin1 = 32, int bin2 = 32, int bin3 = 32 );

	// Color Layout Descriptor (CLD)
	// input: 3-channel RGB color image (f->image)
	// numberOfYCoeff/numberOfCCoeff: (allowable values: 3, 6, 10, 15, 21, 28, 64)	( DC(1) + AC )
	static XM::ColorLayoutDescriptor* getColorLayoutD( Frame* f, int numberOfYCoeff = 64, int numberOfCCoeff = 28 );

 */
	///--------------------------------------------------
	///             MPEG-7 Texture Descriptors
	///--------------------------------------------------

	// Edge Histogram Descriptor (EHD)
	// input: 3-channel RGB color image (f->image), converted to grayscale within the extractor
	// TODO: input grayscale image as HTD
	static XM::EdgeHistogramDescriptor* getEdgeHistogramD( Frame* f );

/* 	// Homogeneous Texture Descriptor (HTD)
	// input: single band grayscale image (f->gray);
	//        image size must be > 128x128
	// use f->setGray(grayImage); before calling this function
	// layerFlag: base-layer 32-components,  true:full-layer 62 components (both energy and deviation)
	static XM::HomogeneousTextureDescriptor* getHomogeneousTextureD( Frame* f, bool layerFlag = true );
 */

/* 	///--------------------------------------------------
	///             MPEG-7 Shape Descriptors
	///--------------------------------------------------

	// Contour Shape descriptor (CShD)
	// input: single channel mask image (f->mask); background: 0, foreground: 255
	//        set f->mask before calling this function (f->setMask(mask))
	static XM::ContourShapeDescriptor* getContourShapeD( Frame* f );

	// Region Shape Descriptor (RSD),
	// input: single channel mask image (f->mask); background: 0, foreground: 255
	//        set f->mask before calling this function (f->setMask(mask))
	static XM::RegionShapeDescriptor* getRegionShapeD( Frame* f );
 */
};


/* ///*********************************************************************************
///			FRD: Face Recognition Descriptor (PCA-based)
///*********************************************************************************
#include "Extraction/FaceRecognitionExtraction.h"
class FaceRecognitionFeature
{
    public:
    FaceRecognitionFeature();
    ~FaceRecognitionFeature();

    // input: single band grayscale image (f->gray);
    // input: face image of size w = 46, height = 56, normalized
    // do not delete the FRD* (eigenfeature[48]) returned, it is reused by the extraction tool (FRD: Descriptors/FaceRecognition.h)
    // TODO: include face normalization within this class
    XM::FRD* getFaceRecognitionD( Frame* f );

    XM::FaceRecognitionExtractionTool* facerexTool;
};


///*********************************************************************************
///			GoF = Group of Frame Descriptor
///*********************************************************************************

#include "Extraction/GoFGoPColorExtraction.h"
/********************************************
	Howto use the GoF class

    GoF* gofd = new GoF( 1, video->getWidth(), video->getHeight());
    while(...){
        // read/get frame

        // add the frame to the group
        gofd->addFrame1( frame );
    }
    // end
    gofd->end();

    // get the descriptor
    XM::ScalableColorDescriptor* scd = gofd->scd;

    //release gofd (descriptor is not released by the GoF destructor, release it when you are done with it)
    delete gofd;

*********************************************/
/* 
// The output is a Scalable Color descriptor
class GoF
{
public:

	// type = 1: from whole frame, type = 2: from arbitrary regions
	// aggregationTye = 0: average, aggregationTye = 1: median, aggregationTye = 2: intersection
	GoF( int type, int w = 0, int h = 0, int aggregationTye = 0, int numCoeff = 256, int bitPlanesDiscarded = 0 );

	// destructor, descriptor 'scd' is not released, it should be released by the clients
	~GoF();

    // reset and start again
	void restart( int aggregationType, int numCoeff, int bitPlanesDiscarded );

	// reset to initial state
	void reset();
	void start( int aggregation );

	// for type 1 (whole frame)
	void addFrame1( IplImage* img );

	// for type 2 (arbitrary region), mask should be set before calling this function
	void addFrame2( Frame* f );

	// post processing - aggregation at the end of processing all the frames in the group
	void end();

	///-- Data --

	// number of frames in the group
	int numFrames;

	// type of input, 1: whole frame, 2: from an arbitrary region
	int type;

	Frame* f;

	XM::GoFGoPColorExtractionTool* gof;
	
	// Output: a Scalable Color descriptor
	XM::ScalableColorDescriptor* scd;

}; */

#endif
