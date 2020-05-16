/*************************************************************
* Name:      Frame.h
* Purpose:   Represent a video frame or image with MomVop structure used in MPEG-7 XM;
              hence, decrease the memory allocation deallocation for each frame during video processing
	          allocation, deallocation, setting the image is done only once and the Frame object can be
	          used to extract all the features (color/shape/texture)
* Author:    Muhammet Bastan (bastan@cs.bilkent.edu.tr)
* Created:   2008
* Copyright: Muhammet Bastan (http://www.cs.bilkent.edu.tr/~bastan)
* License:
* date: 2008-07-07
* Update: 2010-02-18
**************************************************************/

#ifndef __FRAME__H__
#define __FRAME__H__

#include "cxcore.h"

#include "AddressLib/vopio.h"

class Frame
{

public:
    // width, height: size of image to be stored
    // imgFlag: allocate memory for 3-channel color image?
    // grayFlag: allocate memory for single channel grayscale image?
    // maskFlag: allocate memory for foreground mask?
	Frame( int width, int height, bool imgFlag = true, bool grayFlag = true, bool maskFlag = true );

	/// allocates and sets the image (this->image <= src)
	Frame( IplImage* src, bool imgFlag = true, bool grayFlag = true, bool maskFlag = true  );

	// destrutor: release memory
	~Frame();

    // allocate this->image, this->gray, this->mask (depending on the flags)
	void allocate(int width, int height, bool imgFlag = true, bool grayFlag = true, bool maskFlag = true );

    // resize the images in this Frame, if the new size is different from the current size
    // mask is reset (ResetMaskAll()) if it exists
    void resize(int width, int height);

	// set the 3-cahnnel color image (copy data from src to this->image)
	// this->image <= src (if this->image != 0 && src != 0 )
	void setImage( IplImage* src );

	// set the single channel grayscale image (copy data from gray to this->gray)
	// this->gray <= gray (if this->gray != 0 && gray != 0 )
	void setGray( IplImage* gray );

	// set the alpha channel (mask) to the arbitrary shape (copy data to this->mask)
	// mask: single channel mask image
	// rid: value of foreground pixels in the input mask, mask
	// fg: set the foreground pixel values (pixels having value rid in 'mask') to 'fg' in this->mask
	// bg: set the background pixel values (pixels having value != rid) to 'bg'
	void setMask( IplImage* mask, int rid, int fg = 255, int bg = 0 );

	// set all the values in the mask to val, and attach this mask to image and gray as their a_chan
	// this  is required by Scalable Color Descriptor extraction!
	void setMaskValue( int val = 255);

	// set the alpha channel (mask) to the arbitrary shape AND point the alpha channels (a_chan) of image and gray to a_chan of mask
	// use this to extract color and grayscale features of arbitrary shapes
	void setMaskAll( IplImage* mask, int rid, int fg = 255, int bg = 0 );

    // set the mask only for this->image
    void setMaskImage( IplImage* mask, int rid, int fg = 255, int bg = 0 );

    // set the mask only for this->gray
    void setMaskGray( IplImage* mask, int rid, int fg, int bg);

	//point a_chan of image (mask) and gray to 0 again (as initiated) - to extract feature from the whole frame
	void resetMaskAll();
	void resetMaskImage();
	void resetMaskGray();

	///--- data (made public for convenience) ---

	// width-height of the frame
	int width;
	int height;

    // MomVop is the data structure used to store image data in MPEG-7 Reference software
    // see AddressLib/vopio.h

	// 3-channel color image (3x8 bit)
	MomVop* image;

	// single channel grayscale image (8 bit)
	MomVop* gray;

	// single channel mask for an arbitrary shape (8 bit)
	MomVop* mask;

};

#endif
