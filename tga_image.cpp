/*=================================================================================
FILE:			tga_image.cpp
  
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

#include "tga_image.h"

#include "AEE.h"
#include "AEEShell.h"
#include "AEEFile.h"

#include "AEEstdlib.h"

/*-------------------------------------------------------------------------------*
 *                      D A T A   S T R U C T U R E                              *
 *-------------------------------------------------------------------------------*/
struct tgaheader_t
{

	GLubyte   idLength;
	GLubyte   colorMapType;
	GLubyte   imageType;
	GLubyte   colorMapSpec[5];
	GLushort  xOrigin;
	GLushort  yOrigin;
	GLushort  width;
	GLushort  height;
	GLubyte   bpp;
	GLubyte   imageDesc;

};


enum TGATypes
{

	TGA_NODATA = 0,
	TGA_INDEXED = 1,
	TGA_RGB = 2,
	TGA_GRAYSCALE = 3,
	TGA_INDEXED_RLE = 9,
	TGA_RGB_RLE = 10,
	TGA_GRAYSCALE_RLE = 11

};

static int GetPadding(int width, int bpp)
{

	return (((width * bpp + 31) & 0xFFFFFFE0) - width * bpp) / 8;

}

/*-------------------------------------------------------------------------------*
 *                          B E G I N   P R O G R A M                            *
 *-------------------------------------------------------------------------------*/

/*===========================================================================
FUNCTION: TGA_IMAGE::TGA_IMAGE() : m_pData(NULL)
  
DESCRIPTION:
   The TGA_IMAGE default constructor.  
   
PROTOTYPE:
   TGA_IMAGE::TGA_IMAGE()
      
PARAMETERS:
   none
            
DEPENDENCIES
   none
              
RETURN VALUE
   none
                
===========================================================================*/
TGA_IMAGE::TGA_IMAGE() : m_pData(NULL)
{

} // end TGA_IMAGE::constructor


/*===========================================================================
FUNCTION: TGA_IMAGE::TGA_IMAGE(IShell *shell) : m_pData(NULL)
  
DESCRIPTION:
   The TGA_IMAGE default constructor.  
   
PROTOTYPE:
   TGA_IMAGE::TGA_IMAGE(IShell *shell)
      
PARAMETERS:
   shell : Pointer to the IShell Interface object
            
DEPENDENCIES
   none
              
RETURN VALUE
   none
                
===========================================================================*/
TGA_IMAGE::TGA_IMAGE(IShell *shell) : m_pData(NULL)
{

	m_pIShell = shell;

} // end TGA_IMAGE::constructor


/*===========================================================================
FUNCTION: TGA_IMAGE::~TGA_IMAGE()
  
DESCRIPTION:
   The TGA_IMAGE default destructor.  
   
PROTOTYPE:
   TGA_IMAGE::~TGA_IMAGE()
      
PARAMETERS:
   none
            
DEPENDENCIES
   none
              
RETURN VALUE
   none
                
===========================================================================*/
TGA_IMAGE::~TGA_IMAGE()
{

	FreeData();

} // end TGA_IMAGE::destructor


/*===========================================================================
FUNCTION: TGA_IMAGE::FreeData()
  
DESCRIPTION:
   Release the memory currently holding the image data.
   
PROTOTYPE:
   TGA_IMAGE::FreeData()
      
PARAMETERS:
   none
            
DEPENDENCIES
   none
              
RETURN VALUE
   none
                
===========================================================================*/
void TGA_IMAGE::FreeData()
{
	
	FREE(m_pData);
	m_pData = NULL;
  
} // end TGA_IMAGE::FreeData()


/*===========================================================================
FUNCTION: TGA_IMAGE::AddAlphaChannel()
  
DESCRIPTION:
   Adds an alpha channel to an image
   
PROTOTYPE:
   TGA_IMAGE::AddAlphaChannel()
      
PARAMETERS:
   none
            
DEPENDENCIES
   none
              
RETURN VALUE
   none
                
===========================================================================*/
void TGA_IMAGE::AddAlphaChannel(GLubyte value)
{
	
	if (24 == m_colorDepth)
	{
		// for 24 bit images, just add another 8 bits for alpha
		GLubyte* newImage = new GLubyte[m_width * m_height * 4];
		if (!newImage)
		return;

		m_imageSize = m_width * m_height * 4;

		int pad = GetPadding(m_width, 24);
		rgb_t* source = (rgb_t*)m_pData;
		rgba_t* dest = (rgba_t*)newImage;

		for (int x = 0; x < m_height; ++x)
		{
			for (int y = 0; y < m_width; ++y)
			{
				dest->r = source->r;
				dest->g = source->g;
				dest->b = source->b;
				dest->a = value;

				++source;
				++dest;
			}
			source = (rgb_t*)((GLubyte*)source + pad);
		}

		delete [] m_pData;
		m_pData = newImage;
		m_colorDepth = 32;
		m_dataType = GL_UNSIGNED_BYTE;
		m_dataFormat = GL_RGBA;
	}
  
} // end TGA_IMAGE::AddAlphaChannel()


/*===========================================================================
FUNCTION: TGA_IMAGE::Flip()
  
DESCRIPTION:
   Flips the image vertically
   
PROTOTYPE:
   TGA_IMAGE::Flip()
      
PARAMETERS:
   none
            
DEPENDENCIES
   none
              
RETURN VALUE
   TRUE: if flipping the image successful
   FALSE: if error occured
                
===========================================================================*/
bool TGA_IMAGE::Flip()
{
	
	if (!m_pData)
		return false;

	rgba_t* tmpBits = new rgba_t[m_width];
	if (!tmpBits)
		return false;

	int lineWidth = m_width * 4;

	rgba_t* top = (rgba_t*)m_pData;
	rgba_t* bottom = (rgba_t*)(m_pData + lineWidth*(m_height-1));

	for (int i = 0; i < (m_height / 2); ++i)
	{
		MEMCPY(tmpBits, top, lineWidth); 
		MEMCPY(top, bottom, lineWidth);
		MEMCPY(bottom, tmpBits, lineWidth);

		top = (rgba_t*)((GLubyte*)top + lineWidth);
		bottom = (rgba_t* )((GLubyte*)bottom - lineWidth);
	}

	delete [] tmpBits;
	tmpBits = 0;

	return true; 

} // TGA_IMAGE::Flip()


/*===========================================================================
FUNCTION: TGA_IMAGE::SwapBlueAndRed()
  
DESCRIPTION:
   Flips the blue and red components of every pixel
   
PROTOTYPE:
   TGA_IMAGE::SwapBlueAndRed()
      
PARAMETERS:
   none
            
DEPENDENCIES
   none
              
RETURN VALUE
   none
                
===========================================================================*/
void TGA_IMAGE::SwapBlueAndRed()
{

	switch (m_colorDepth)
	{
		case 32:
		{
			GLubyte temp;
			rgba_t* source = (rgba_t*)m_pData;

			for (int pixel = 0; pixel < (m_width * m_height); ++pixel)
			{
				temp = source[pixel].b;
				source[pixel].b = source[pixel].r;
				source[pixel].r = temp;
			}
		} break;
		case 24:
		{
			GLubyte temp;
			rgb_t* source = (rgb_t*)m_pData;

			for (int pixel = 0; pixel < (m_width * m_height); ++pixel)
			{
				temp = source[pixel].b;
				source[pixel].b = source[pixel].r;
				source[pixel].r = temp;
			}
		} break;
		default:
		// ignore other color depths
		break;
	}

} // end TGA_IMAGE::ToggleBlueAndRed()


/*===========================================================================
FUNCTION: TGA_IMAGE::SetAlpha32()
  
DESCRIPTION:
   Sets the alpha value for every pixel matching color to the specified
   alpha. If color is NULL, every pixel will be set to the indicated value.
   
PROTOTYPE:
   TGA_IMAGE::SetAlpha32()
      
PARAMETERS:
   none
            
DEPENDENCIES
   none
              
RETURN VALUE
   none
                
===========================================================================*/
void TGA_IMAGE::SetAlpha32(const rgb_t* color, GLubyte alpha)
{
	
	if (32 == m_colorDepth)
	{
		rgba_t* pixel = (rgba_t*)m_pData;
		for (int i = 0; i < (m_width * m_height); ++i)
		{
			if (color)
			{
				if (pixel->r == color->r &&
					pixel->g == color->g &&
					pixel->b == color->b)
				{
					pixel->a = alpha;
				}
			}
			else
			{
				pixel->a = alpha;
			}

			++pixel;
		}
	}
 
} 


/*===========================================================================
FUNCTION: TGA_IMAGE::Load(const char* filename)
  
DESCRIPTION:
   Adds an alpha channel to an image
   
PROTOTYPE:
   TGA_IMAGE::Load(const char* filename)
      
PARAMETERS:
   filename : Pointer to the character array of filename
            
DEPENDENCIES
   none
              
RETURN VALUE
   TRUE: if loading the image successful
   FALSE: if error occured
                
===========================================================================*/
bool TGA_IMAGE::Load(const char* filename)
{
	
	IFileMgr *m_pIFileMgr;
	IFile* pFile;

	//if (m_pData != NULL)  FreeData();

	if (ISHELL_CreateInstance(m_pIShell, AEECLSID_FILEMGR, (void **)&m_pIFileMgr)
		!= SUCCESS) 
		return EFAILED;
	pFile = IFILEMGR_OpenFile(m_pIFileMgr, filename, _OFM_READ);

	if (!pFile)
		return false;

	// read in the image type
	tgaheader_t tga;		// TGA header

	IFILE_Read(pFile, &tga, sizeof(tgaheader_t));

	// see if the type is one that we support
	if ((  (tga.imageType != TGA_RGB) 
		&& (tga.imageType != TGA_GRAYSCALE) 
		&& (tga.imageType != TGA_RGB_RLE) 
		&& (tga.imageType != TGA_GRAYSCALE_RLE) ) 
		|| (tga.colorMapType != 0) )
	{
		if (pFile) IFILE_Release(pFile);
		if (m_pIFileMgr) IFILEMGR_Release(m_pIFileMgr);

		return NULL;
	}

	// store texture information
	m_width = tga.width;
	m_height = tga.height;

	// colormode -> 3 = BGR, 4 = BGRA
	int colorMode = tga.bpp / 8;

	// won't handle < 24 bpp for now
	if (colorMode < 3)
	{
		if (pFile) IFILE_Release(pFile);
		if (m_pIFileMgr) IFILEMGR_Release(m_pIFileMgr);

		return NULL;
	}

	m_imageSize = m_width * m_height * colorMode;

	// allocate memory for TGA image data
	m_pData = (GLubyte*) MALLOC(sizeof(GLubyte)*(m_imageSize+1));

	// read image data
	if (tga.imageType == TGA_RGB || tga.imageType == TGA_GRAYSCALE)
	{
		IFILE_Read(pFile, m_pData, m_imageSize);
	}
	else // must be RLE compressed
	{
		GLubyte id;
		GLubyte length;
		rgba_t color = { 0, 0, 0, 0 };
		GLuint i = 0;

		while(i < m_imageSize)
		{

			IFILE_Read(pFile, &id, sizeof(char));

			// see if this is run length data
			if(id & 0x80)
			{
				// find the run length
				length = (GLubyte)(id - 127);

				// next 3 (or 4) bytes are the repeated values

				IFILE_Read(pFile, &color.b, sizeof(char));
				IFILE_Read(pFile, &color.g, sizeof(char));
				IFILE_Read(pFile, &color.r, sizeof(char));

				if(colorMode == 4)
				{
					IFILE_Read(pFile, &color.a, sizeof(char));
				}

				// save everything in this run
				while(length > 0)
				{
					m_pData[i++] = color.b;
					m_pData[i++] = color.g;
					m_pData[i++] = color.r;
					if (colorMode == 4)
					{
						m_pData[i++] = color.a;
					}

					--length;
				}
			}
			else
			{
				// the number of non RLE pixels
				length = GLubyte(id + 1);

				while (length > 0)
				{

					IFILE_Read(pFile, &color.b, sizeof(char));
					IFILE_Read(pFile, &color.g, sizeof(char));
					IFILE_Read(pFile, &color.r, sizeof(char));

					if(colorMode == 4)
					{
						IFILE_Read(pFile, &color.a, sizeof(char));
					}
					m_pData[i++] = color.b;
					m_pData[i++] = color.g;
					m_pData[i++] = color.r;
					if(colorMode == 4)
					{
						m_pData[i++] = color.a;
					}

					--length;
				}
			}
		}
	}

	if (pFile) IFILE_Release(pFile);
	if (m_pIFileMgr) IFILEMGR_Release(m_pIFileMgr);

	switch(tga.imageType)
	{
		case TGA_RGB:
		case TGA_RGB_RLE:
			if (3 == colorMode)
			{
				m_dataFormat = GL_RGB;
				m_dataType = GL_UNSIGNED_BYTE;
				m_colorDepth = 24;
			}
			else
			{
				m_dataFormat = GL_RGBA;
				m_dataType = GL_UNSIGNED_BYTE;
				m_colorDepth = 32;
			}
			break;
		case TGA_GRAYSCALE:
		case TGA_GRAYSCALE_RLE:
			m_dataFormat = GL_LUMINANCE;
			m_dataType = GL_UNSIGNED_BYTE;
			m_colorDepth = 8;
			break;
	}

	SwapBlueAndRed();

	return (m_pData != NULL);


} // end TGA::LoadTGAFile()


/*===========================================================================
FUNCTION: TGA_IMAGE::Get4444()
  
DESCRIPTION:
   Get image in 4 bits red, 4 bits green, 4 bits blue and 4 bits alpha format
   
PROTOTYPE:
   TGA_IMAGE::Get4444()
      
PARAMETERS:
   none
            
DEPENDENCIES
   none
              
RETURN VALUE
   Pointer to struct rgba4444_t
                
===========================================================================*/
rgba4444_t* TGA_IMAGE::Get4444()
{
	
	if (m_colorDepth == 24)
		AddAlphaChannel(255);

	if (m_colorDepth != 32)
		return NULL;

	// for 24 bit images, just add another 8 bits for alpha
	rgba4444_t* newImage = new rgba4444_t[m_width * m_height];

	if (!newImage)
		return NULL;

	rgba_t* source = (rgba_t*)m_pData;
	rgba4444_t* dest = newImage;

	for (int x = 0; x < m_height; ++x)
	{
		for (int y = 0; y < m_width; ++y)
		{
			dest->r = source->r >> 4;
			dest->g = source->g >> 4;
			dest->b = source->b >> 4;
			dest->a = source->a >> 4;

			++source;
			++dest;
		}
	}

	return newImage;
 	
}


/*===========================================================================
FUNCTION: TGA_IMAGE::Get5551()
  
DESCRIPTION:
   Get image in 5 bits red, 5 bits green, 5 bits blue and 1 bits alpha format
   
PROTOTYPE:
   TGA_IMAGE::Get5551()
      
PARAMETERS:
   none
            
DEPENDENCIES
   none
              
RETURN VALUE
   Pointer to struct rgba5551_t
                
===========================================================================*/
rgba5551_t* TGA_IMAGE::Get5551()
{
	
	if (m_colorDepth == 24)
		AddAlphaChannel(255);
	if (m_colorDepth != 32)
		return NULL;

	// for 24 bit images, just add another 8 bits for alpha
	rgba5551_t* newImage = new rgba5551_t[m_width * m_height];

	if (!newImage)
		return NULL;

	rgba_t* source = (rgba_t*)m_pData;
	rgba5551_t* dest = newImage;

	for (int x = 0; x < m_height; ++x)
	{
		for (int y = 0; y < m_width; ++y)
		{
			dest->r = source->r >> 3;
			dest->g = source->g >> 3;
			dest->b = source->b >> 3;
			dest->a = source->a >> 7;

			++source;
			++dest;
		}
	}

	return newImage;
  	
}


/*===========================================================================
FUNCTION: TGA_IMAGE::Get565()
  
DESCRIPTION:
   Get image in 5 bits red, 6 bits green, 5 bits blue and 0 bits alpha format
   
PROTOTYPE:
   TGA_IMAGE::Get565()
      
PARAMETERS:
   none
            
DEPENDENCIES
   none
              
RETURN VALUE
   Pointer to struct rgb565_t
                
===========================================================================*/
rgb565_t* TGA_IMAGE::Get565()
{
	
	if (m_colorDepth != 24)
		return NULL;

	// for 24 bit images, just add another 8 bits for alpha
	rgb565_t* newImage = new rgb565_t[m_width * m_height];

	if (!newImage)
		return NULL;

	rgb_t* source = (rgb_t*)m_pData;
	rgb565_t* dest = newImage;

	for (int x = 0; x < m_height; ++x)
	{
		for (int y = 0; y < m_width; ++y)
		{
			dest->r = source->r >> 3;
			dest->g = source->g >> 2;
			dest->b = source->b >> 3;

			++source;
			++dest;
		}
	}

	return newImage;

}


/*===========================================================================
FUNCTION: TGA_IMAGE::GetAlpha()
  
DESCRIPTION:
   Gets the alpha bits of an image
   
PROTOTYPE:
   TGA_IMAGE::GetAlpha()
      
PARAMETERS:
   none
            
DEPENDENCIES
   none
              
RETURN VALUE
   Pointer to GLubyte
                
===========================================================================*/
GLubyte* TGA_IMAGE::GetAlpha()
{
	
	if (m_colorDepth != 32)
		return NULL;

	// for 24 bit images, just add another 8 bits for alpha
	GLubyte* newImage = new GLubyte[m_width * m_height];

	if (!newImage)
		return NULL;

	rgba_t* source = (rgba_t*)m_pData;
	GLubyte* dest = newImage;

	for (int x = 0; x < m_height; ++x)
	{
		for (int y = 0; y < m_width; ++y)
		{
			*dest = source->a;

			++source;
			++dest;
		}
	}

	return newImage;
  
}


/*===========================================================================
FUNCTION: TGA_IMAGE::GetLuminanceAlpha()
  
DESCRIPTION:
   Gets the luminance alpha bit of an image
   
PROTOTYPE:
   TGA_IMAGE::GetLuminanceAlpha()
      
PARAMETERS:
   none
            
DEPENDENCIES
   none
              
RETURN VALUE
   Pointer to GLubyte
                
===========================================================================*/

GLubyte* TGA_IMAGE::GetLuminanceAlpha()
{
	
	if (m_colorDepth == 24)
		AddAlphaChannel(255);
	if (m_colorDepth !=32)
		return NULL;

	// for 24 bit images, just add another 8 bits for alpha
	GLubyte* newImage = new GLubyte[m_width * m_height * 2];

	if (!newImage)
		return NULL;

	rgba_t* source = (rgba_t*)m_pData;
	la_t* dest = (la_t*)newImage;

	for (int x = 0; x < m_height; ++x)
	{
		for (int y = 0; y < m_width; ++y)
		{
			dest->a = source->a;
			dest->l = (GLubyte)((source->r + source->g + source->b) / 3);

			++source;
			++dest;
		}
	}

	return newImage;

}


/*===========================================================================
FUNCTION: TGA_IMAGE::GetLuminance()
  
DESCRIPTION:
   Gets the luminance bits of an image
   
PROTOTYPE:
   TGA_IMAGE::GetLuminance()
      
PARAMETERS:
   none
            
DEPENDENCIES
   none
              
RETURN VALUE
   Pointer to GLubyte
                
===========================================================================*/

GLubyte* TGA_IMAGE::GetLuminance()
{
	
	if (m_colorDepth != 32)
		return NULL;

	// for 24 bit images, just add another 8 bits for alpha
	GLubyte* newImage = new GLubyte[m_width * m_height];

	if (!newImage)
		return NULL;

	rgba_t* source = (rgba_t*)m_pData;
	GLubyte* dest = newImage;

	for (int x = 0; x < m_height; ++x)
	{
		for (int y = 0; y < m_width; ++y)
		{
			*dest = (GLubyte)((source->r + source->g + source->b) / 3);

			++source;
			++dest;
		}
	}

	return newImage;

}