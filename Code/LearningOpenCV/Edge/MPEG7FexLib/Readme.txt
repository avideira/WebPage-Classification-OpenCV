MPEG-7 Low Level Feature Extraction Library (Linux - Ubuntu 9.10)

by Muhammet Bastan, (c) February 2010
Bilkent University
Department of Computer Engineering
Bilkent, Ankara, TURKIYE

e-mail: mubastan@gmail.com

download page: www.cs.bilkent.edu.tr/~bilmdg/bilvideo-7/Software.html

date: 20.02.2010
update:
----------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------
Table of Contents:

	Introduction
	License - Liability
	Supported Descriptors
	How to compile your programs
	Using the API
	Similarity Computation (Matching)
----------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------
		Introduction
----------------------------------------------------------------------------------------

This library is compiled (C++) with gcc 4.3 on Ubuntu 9.10 
and uses OpenCV 1.0 for image handling (IplImage data structure).
It was developed for BilVideo-7 MPEG-7 compatible video indexing and retrieval system:

Muhammet Bastan, Hayati Cam, Ugur Gudukbay and Ozgur Ulusoy, 
"BilVideo-7: An MPEG-7 Compatible Video Indexing and Retrieval System", 
IEEE MultiMedia, vol. 17, no. 3, pp. 62-73, July-September 2010.

Bibtex entry:
@article{BilVideo7-MM2010,
	author = {Muhammet Ba\c{s}tan and Hayati \c{C}am and U\v{g}ur G\"{u}d\"{u}kbay and \"{O}zg\"{u}r Ulusoy},
	title = {{BilVideo-7: An MPEG-7-Compatible Video Indexing and Retrieval System}},
	journal ={IEEE MultiMedia},
	volume = {17},
	number = {3},
	pages = {62--73},
	year = {2009},
	doi = {http://doi.ieeecomputersociety.org/10.1109/MMUL.2009.74},
	publisher = {IEEE Computer Society},
	address = {Los Alamitos, CA, USA},

Please cite this paper if you use this library.
Please feel free to contact the author (bastan@cs.bilkent.edu.tr) in case of any bugs (which is of course possible),
or valuable suggestions.

---------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------
License - Liability: 
(GPL - You are free to use/distribute the tool including this Readme.txt and all the other files as is)
In no event and under no legal theory,      
whether in tort (including negligence), contract, or otherwise,      
unless required by applicable law (such as deliberate and grossly      
negligent acts) or agreed to in writing, shall any Contributor be      
liable to You for damages, including any direct, indirect, special,      
incidental, or consequential damages of any character arising as a      
result of this License or out of the use or inability to use the      
Work (including but not limited to damages for loss of goodwill,      
work stoppage, computer failure or malfunction, or any and all      
other commercial damages or losses), even if such Contributor      
has been advised of the possibility of such damages.
----------------------------------------------------------------------------------------

The library is adapted from the MPEG-7 XM Reference Software available at 
http://standards.iso.org/ittf/PubliclyAvailableStandards/index.html

As also available in the header files adapted from the XM software, please note that:
"ISO/IEC gives users of MPEG-7 free license to this software module or
modifications thereof for use in hardware or software products claiming
conformance to MPEG-7".

The XM reference software contains some bugs and does not work properly.
Moreover, it is not meant to be used in applications directly, therefore, 
there is no decent interface to use it easily, not mentioning the horrible coding, 
which makes everybody run away at the first glance!
This library is an attempt to make the MPEG-7 feature extraction easier to integrate into new systems..

----------------------------------------------------------------------------------------

Supported descriptors in this library :

 + Color Structure Descriptor (CSD)
 + Scalable Color Descriptor (SCD)
 + GoF/GoP Color Descriptor (GoF/GoP)
 + Color Layout Descriptor (CLD)
 + Dominant Color Descriptor (DCD)
 + Homogeneous Texture Descriptor (HTD)
 + Edge Histogram Descriptor (EHD)
 + Face Recognition Descriptor (FRD)

MPEG-7 Motion descriptors (e.g., Motion Activity) will also be included in the library in the future..

Please see http://www.chiariglione.org/mpeg/standards/mpeg-7/mpeg-7.htm for a brief description of each.
For more details: 
B. S. Manjunath, P. Salembier, and T. Sikora, Eds., Introduction to MPEG-7: Multimedia Content Description Interface. England: WILEY, 2002.

-----------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------
		How to compile your programs
-----------------------------------------------------------------------------------------------

Directory Structure:

MPEG7FexLib
	- include: header files
	- lib: contains static library libMPEG7Fex.a
	- sample: sample codes with (hopefully just enough) comments illustrating the usage of the library (with CodeBlocks project file)


The library is compiled with OpenCV 1.0, which in turn was compiled with FFmpeg 0.4.9.


include directories:
	MPEG7FexLib/include
	opencv/include/opencv
	

libraries:
	MPEG7FexLib: MPEG7FexLib/include/libMPEG7Fex.a
	
	{	
	OpenCV: libcxcore, libcv, libhighgui
	FFmpeg: since OpenCV was compiled with FFmpeg (0.4.9), libavcodec, libavformat, libavutil
	}

Running: you should have the path of shared object libraries in your LD_LIBRARY_PATH.
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:OpenCV1.0_lib_dir:FFmpeg_lib_dir

-----------------------------------------------------------------------------------------------
		Using the API
-----------------------------------------------------------------------------------------------

The library has a very easy-to-use API.
You only need to include the "Feature.h" in your code and call the static member functions of the "Feature" class.
Have a look at the sample codes in the "sample" directory to see how to use the library and what the output (descriptors) looks like.
The codes are commented and are (hopefully) self-explanatory.

Example for Color Structure Descriptor:

#include "highgui.h"	// for loading images
#include "Feature.h"	// to use the feature extraction library
	
#include <iostream>

int main( int argc, char* argv[] )
{	
	// load an image
	IplImage* image = cvLoadImage("motor.jpg");

	///-- extract global image descriptors --

	// create a Frame object (see include/Frame.h)
	// allocate memory for 3-channel color and 1-channel gray image and mask
	Frame* frame = new Frame( image->width, image->height, true, true, true);

	// set the image of the frame
	frame->setImage(image);
	
	// compute Color Structure descriptor
	XM::ColorStructureDescriptor* csd = Feature::getColorStructureD(frame, descSize);

	// write to screen
	for(unsigned int i = 0; i < csd->GetSize(); i++)
		std::cout << (int)csd->GetElement(i) << " " ;
	std::cout << std::endl;
	
	// release descriptor
	delete csd;
	
	// release the images..
	delete frame;
	cvReleaseImage(&image);
	
	return 0;
}


I will try to expand this Readme whe I have time.
Please feel free to contact me (bastan@cs.bilkent.edu.tr) if you have any difficulty/problem using the library.

-----------------------------------------------------------------------------------------------------------
		Similarity Computation (Matching):
-----------------------------------------------------------------------------------------------------------

For the distance metrics proposed by MPEG-7, see the MPEG-7 book:
B. S. Manjunath, P. Salembier, and T. Sikora, Eds., Introduction to MPEG-7: Multimedia Content Description Interface. England: WILEY, 2002.

or the following papers:

Muhammet Bastan, Hayati Cam, Ugur Gudukbay and Ozgur Ulusoy, 
"BilVideo-7: An MPEG-7 Compatible Video Indexing and Retrieval System", 
IEEE MultiMedia, vol. 17, no. 3, pp. 62-73, July-September 2010.

Muhammet Bastan, Hayati Cam, Ugur Gudukbay, Ozgur Ulusoy, 
An MPEG-7 Compatible Video Retrieval System with Integrated Support for Complex Multimodal Queries,
Bilkent University, Technical Report (BU-CE-0905), 2009.
Available at: http://www.cs.bilkent.edu.tr/tech-reports/2009/BU-CE-0905.pdf

H. Eidenberger, Distance measures for MPEG-7-based retrieval, 
in Proceedings of the 5th ACM SIGMM International Workshop on Multimedia Information Retrieval (MIR 03), 
2003, pp. 130137.

For instance, you may use L1 distance for CSD, SCD..

Good Luck.
