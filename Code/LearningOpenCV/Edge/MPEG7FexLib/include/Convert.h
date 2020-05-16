/***************************************************************
 * Name:      Convert.h
 * Purpose:   Convert IplImage to MomVop
 * Author:    Muhammet Bastan (bastan@cs.bilkent.edu.tr)
 * Created:   07.07.2008
 * Copyright: Muhammet Bastan (http://www.cs.bilkent.edu.tr/~bastan)
 * License:
 **************************************************************/
#ifndef __CONVERT__H__
#define __CONVERT__H__

#include "cxcore.h"
#include "cv.h"

#include "AddressLib/vopio.h"

class Convert {

public:
	static void ipl2momvop( IplImage* src, MomVop* dst );
	static void setShape( IplImage* mask, MomVop* img, int val, int fval = 255, CvRect* roi = NULL );
	static void setShape( IplImage* mask, MomVop* img, int val, int bval = 0, int fval = 255, CvRect* roi = NULL );

	// Set all the values of the mask (a_chan) to fval
	static void setMaskValue( MomVop* img, int fval = 255 );


};

#endif

