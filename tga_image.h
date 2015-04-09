#ifndef TGA_IMAGE_H
#define TGA_IMAGE_H

/*=================================================================================
FILE:			tga_image.h
  
DESCRIPTION:	This file is provide as a standard sample Brew source file. 
				Please refer to this OpenGL(R)ES brew sample application as a 
				reference on how to use the standard OpenGL-ES and EGL APIs.
                          
ABSTRACT:		Port code to work with Brew 

AUTHOR:			QUALCOMM
                        
				Copyright (c) 2004 QUALCOMM Incorporated.
					   All Rights Reserved.
					QUALCOMM Proprietary/GTDR
=================================================================================*/

/*-------------------------------------------------------------------------------*
 *                      I N C L U D E   F I L E S                                *
 *-------------------------------------------------------------------------------*/

#include "gles/gl.h"
#include "AEEShell.h"

#ifdef WIN32
#pragma warning( disable : 4514 4100 4505 )
#endif

/*-------------------------------------------------------------------------------*
 *                      D A T A   S T R U C T U R E                              *
 *-------------------------------------------------------------------------------*/
struct rgb_t
{
	GLubyte r;
	GLubyte g;
	GLubyte b;
};


// 32-bit color type
struct rgba_t
{
	GLubyte r;
	GLubyte g;
	GLubyte b;
	GLubyte a;
};


// packed pixel formats
struct rgb565_t
{
	GLushort
		b : 5,
		g : 6,
		r : 5;
};

// 16-bit color with alpha type
struct rgba5551_t
{
	GLushort 
		a : 1,
		b : 5,
		g : 5,
		r : 5;
};

// 16-bit color with alpha type
struct rgba4444_t
{
	GLushort
		a : 4,
		b : 4,
		g : 4,
		r : 4;
};

struct la_t
{
	GLubyte l;
	GLubyte a;
};

/*-------------------------------------------------------------------------------*
 *                     T G A _ I M A G E   C L A S S                             *
 *-------------------------------------------------------------------------------*/

class TGA_IMAGE
{
public:
	TGA_IMAGE();
	TGA_IMAGE(IShell *shell);

	virtual ~TGA_IMAGE();

	bool  Load(const char* filename);

	// returns the image's width, in pixels
	int   GetWidth() const { return m_width; }
	// returns the image's height in pixels
	int   GetHeight() const { return m_height; }

	// returns the pixel format of the data using OpenGL pixel format types
	GLenum  GetDataFormat() const { return m_dataFormat; }
	// returns the data type used to store color info using the OpenGL data type
	int   GetDataType() const { return m_dataType; }

	// returns a pointer to the image data
	GLubyte*  GetData() const { return m_pData; }

	// swaps the red and blue components of every color
	void  SwapBlueAndRed();

	void  SetAlpha32(const rgb_t* color, GLubyte alpha);
	void  AddAlphaChannel(GLubyte value);
	bool  ApplyAlphaMap(TGA_IMAGE* pAlphaMap);

	rgba4444_t*  Get4444();
	rgba5551_t*  Get5551();
	rgb565_t*  Get565();
	GLubyte*  GetLuminance();
	GLubyte*  GetAlpha();
	GLubyte*  GetLuminanceAlpha();

	IShell *m_pIShell;
	void  FreeData();

private:
	bool  Flip();

	GLubyte*  m_pData;

	GLint   m_width;
	GLint	m_height;
	GLint	m_colorDepth;
	GLenum  m_dataFormat;
	GLint   m_dataType;
	GLuint  m_imageSize;

	GLuint  m_redMask;
	GLuint  m_greenMask;
	GLuint  m_blueMask;

};

#endif 
