///////////////////////////////////////////////////////////////////////////////
//
// This software module was originally developed by
//
// ETRI / Dongguk University, Seoul
// (contributing organizations names)
//
// in the course of development of the MPEG-7 Experimentation Model.
//
// This software module is an implementation of a part of one or more MPEG-7
// Experimentation Model tools as specified by the MPEG-7 Requirements.
//
// ISO/IEC gives users of MPEG-7 free license to this software module or
// modifications thereof for use in hardware or software products claiming
// conformance to MPEG-7.
//
// Those intending to use this software module in hardware or software products
// are advised that its use may infringe existing patents. The original
// developer of this software module and his/her company, the subsequent
// editors and their companies, and ISO/IEC have no liability for use of this
// software module or modifications thereof in an implementation.
//
// Copyright is not released for non MPEG-7 conforming products. The
// organizations named above retain full right to use the code for their own
// purpose, assign or donate the code to a third party and inhibit third parties
// from using the code for non MPEG-7 conforming products.
//
// Copyright (c) 1998-1999.
//
// This notice must be included in all copies or derivative works.
//
// Applicable File Name:  EdgeHist.h
//
// modified by mb - removed unnecessary parts..
// date: 2009-11-13

#ifndef __EDGEHIST_H__
#define __EDGEHIST_H__


namespace XM
{

typedef	struct Edge_Histogram_Descriptor
{
  double Local_Edge[80];
} EHD;


class EdgeHistogramDescriptor
{
public:
  EdgeHistogramDescriptor();
  virtual ~EdgeHistogramDescriptor();

  // Accesssor methods
  virtual unsigned long GetSize(void);
  virtual EHD*	GetEdgeHistogram();
  virtual double*	GetEdgeHistogramD();

  // EHD Output values as char array of size GetSize() (fixed at 80)
  virtual char*	GetEdgeHistogramElement();

  virtual void	SetEdgeHistogram(EHD*	pEdge_Histogram);
  virtual void	SetEdgeHistogramElement(char*	pEdge_Histogram);

  // added by mb
  virtual void resetDescriptor();

private:
  // actual values:
  EHD* m_pEdge_Histogram;
  char* m_pEdge_HistogramElement;

  static double QuantTable[5][8];

};

};

#endif
