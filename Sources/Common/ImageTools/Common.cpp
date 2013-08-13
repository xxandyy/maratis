/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Maratis Image tools
// Common.cpp
//
// Image common
/////////////////////////////////////////////////////////////////////////////////////////////////////////

//========================================================================
// Copyright (c) 2013 Anael Seghezzi <www.maratis3d.com>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================

#include <MCore.h>
#include "Common.h"


static void ubyteToFloat(MImage * src, MImage * dest)
{
	static const float ubyte_div = 1.0f/255.0f;
	
	unsigned char * data1 = (unsigned char *)src->getData();
	float * data2 = (float *)dest->getData();
	unsigned int size = src->getSize();
	for(int i=0; i<size; i++)
		data2[i] = (float)(data1[i])*ubyte_div;
}

static void floatToUbyte(MImage * src, MImage * dest)
{
	float * data1 = (float *)src->getData();
	unsigned char * data2 = (unsigned char *)dest->getData();
	unsigned int size = src->getSize();
	for(int i=0; i<size; i++)
		data2[i] = (unsigned char)CLAMP(data1[i]*255, 0, 255);
}

bool isImageValid(MImage * image)
{
	if(! image)
		return false;

	if(image->getSize() == 0)
		return false;
		
	return true;
}

bool convertToFloat(MImage * image)
{
	if(! isImageValid(image))
		return false;
		
	unsigned int width = image->getWidth();
	unsigned int height = image->getHeight();
	unsigned int components = image->getComponents();
	
	if(image->getDataType() == M_UBYTE)
	{
		MImage copy(*image);
		image->create(M_FLOAT, width, height, components);
		ubyteToFloat(&copy, image);
		return true;
	}
	
	return false;
}

bool convertToUbyte(MImage * image)
{
	if(! isImageValid(image))
		return false;
		
	unsigned int width = image->getWidth();
	unsigned int height = image->getHeight();
	unsigned int components = image->getComponents();
	
	if(image->getDataType() == M_FLOAT)
	{
		MImage copy(*image);
		image->create(M_UBYTE, width, height, components);
		floatToUbyte(&copy, image);
		return true;
	}
	
	return false;
}

bool convertAlphaToGreyscale(MImage * image)
{
	if(! isImageValid(image))
		return false;

	if(image->getComponents() != 4)
		return false;

	unsigned int width = image->getWidth();
	unsigned int height = image->getHeight();
	unsigned int components = image->getComponents();

	if(image->getDataType() == M_UBYTE)
	{
		MImage copy(*image);
		image->create(M_UBYTE, width, height, 1);
		unsigned char * greyPixel = (unsigned char *)image->getData();
		unsigned char * imagePixel = (unsigned char *)copy.getData();
	
		unsigned int x, y;
		for(y=0; y<height; y++)
		for(x=0; x<width; x++)
		{
			(*greyPixel) = imagePixel[3];
			imagePixel += components;
			greyPixel++;
		}
		
		return true;
	}
	else if(image->getDataType() == M_FLOAT)
	{
		MImage copy(*image);
		image->create(M_FLOAT, width, height, 1);
		float * greyPixel = (float *)image->getData();
		float * imagePixel = (float *)copy.getData();
	
		unsigned int x, y;
		for(y=0; y<height; y++)
		for(x=0; x<width; x++)
		{
			(*greyPixel) = imagePixel[3];
			imagePixel += components;
			greyPixel++;
		}
		
		return true;
	}
	
	return false;
}

bool convertToGreyscale(MImage * image)
{
	if(! isImageValid(image))
		return false;

	unsigned int width = image->getWidth();
	unsigned int height = image->getHeight();
	unsigned int components = image->getComponents();

	if(image->getComponents() != 3)
	{
		if(image->getDataType() == M_UBYTE)
		{
			MImage copy(*image);
			image->create(M_UBYTE, width, height, 1);
			unsigned char * greyPixel = (unsigned char *)image->getData();
			unsigned char * imagePixel = (unsigned char *)copy.getData();
	
			unsigned int x, y;
			for(y=0; y<height; y++)
			for(x=0; x<width; x++)
			{
				float val = 0;
				for(int i=0; i<components; i++)
					val += imagePixel[i];
				(*greyPixel) = val / (float)components;
				imagePixel += components;
				greyPixel++;
			}
		
			return true;
		}
		else if(image->getDataType() == M_FLOAT)
		{
			MImage copy(*image);
			image->create(M_FLOAT, width, height, 1);
			float * greyPixel = (float *)image->getData();
			float * imagePixel = (float *)copy.getData();
	
			unsigned int x, y;
			for(y=0; y<height; y++)
			for(x=0; x<width; x++)
			{
				float val = 0;
				for(int i=0; i<components; i++)
					val += ABS(imagePixel[i]);
				(*greyPixel) = val / (float)components;
				imagePixel += components;
				greyPixel++;
			}
		
			return true;
		}
	
		return false;
	}

	if(image->getDataType() == M_UBYTE)
	{
		MImage copy(*image);
		image->create(M_UBYTE, width, height, 1);
		unsigned char * greyPixel = (unsigned char *)image->getData();
		unsigned char * imagePixel = (unsigned char *)copy.getData();
	
		unsigned int x, y;
		for(y=0; y<height; y++)
		for(x=0; x<width; x++)
		{
			(*greyPixel) = imagePixel[0]*0.3f + imagePixel[1]*0.5f + imagePixel[2]*0.2f;
			imagePixel += components;
			greyPixel++;
		}
		
		return true;
	}
	else if(image->getDataType() == M_FLOAT)
	{
		MImage copy(*image);
		image->create(M_FLOAT, width, height, 1);
		float * greyPixel = (float *)image->getData();
		float * imagePixel = (float *)copy.getData();
	
		unsigned int x, y;
		for(y=0; y<height; y++)
		for(x=0; x<width; x++)
		{
			(*greyPixel) = imagePixel[0]*0.3f + imagePixel[1]*0.5f + imagePixel[2]*0.2f;
			imagePixel += components;
			greyPixel++;
		}
		
		return true;
	}
	
	return false;
}

bool convertToRGB(MImage * image)
{
	if(! isImageValid(image))
		return false;

	if(image->getComponents() != 1)
		return false;

	unsigned int width = image->getWidth();
	unsigned int height = image->getHeight();

	if(image->getDataType() == M_UBYTE)
	{
		MImage copy(*image);
		image->create(M_UBYTE, width, height, 3);
		unsigned char * rgbPixel = (unsigned char *)image->getData();
		unsigned char * imagePixel = (unsigned char *)copy.getData();
	
		unsigned int x, y;
		for(y=0; y<height; y++)
		for(x=0; x<width; x++)
		{
			rgbPixel[0] = rgbPixel[1] = rgbPixel[2] = *imagePixel;
			imagePixel++;
			rgbPixel+=3;
		}
		
		return true;
	}
	else if(image->getDataType() == M_FLOAT)
	{
		MImage copy(*image);
		image->create(M_FLOAT, width, height, 3);
		float * rgbPixel = (float *)image->getData();
		float * imagePixel = (float *)copy.getData();
	
		unsigned int x, y;
		for(y=0; y<height; y++)
		for(x=0; x<width; x++)
		{
			rgbPixel[0] = rgbPixel[1] = rgbPixel[2] = *imagePixel;
			imagePixel++;
			rgbPixel+=3;
		}
		
		return true;
	}
	
	return false;	
}

void getImageSubPixel_ubyte(MImage * image, float x, float y, float * color)
{
	int width = (int)image->getWidth();
	int height = (int)image->getHeight();

	unsigned int i;
	unsigned int components = image->getComponents();

	x = CLAMP(x, 0, width-1);
	y = CLAMP(y, 0, height-1);

	int ix = (int)(x);
	int iy = (int)(y);
	float fx = x - ix;
	float fy = y - iy;
	float A, B;

	unsigned char * colors[4];
	unsigned char * imgData = (unsigned char*)image->getData();
	
	colors[0] = imgData + (width*iy + ix)*components;
	colors[1] = imgData + (width*iy + MIN(width-1, ix+1))*components;
	colors[2] = imgData + (width*MIN(height-1, iy+1) + ix)*components;
	colors[3] = imgData + (width*MIN(height-1, iy+1) + MIN(width-1, ix+1))*components;
	
	for(i=0; i<components; i++)
	{
		A = colors[0][i] + (colors[2][i] - colors[0][i])*fy;
		B = colors[1][i] + (colors[3][i] - colors[1][i])*fy;
		color[i] = A + (B-A)*fx;
	}
}

void getImageSubPixel_float(MImage * image, float x, float y, float * color)
{
	int width = (int)image->getWidth();
	int height = (int)image->getHeight();

	unsigned int i;
	unsigned int components = image->getComponents();

	x = CLAMP(x, 0, width-1);
	y = CLAMP(y, 0, height-1);

	int ix = (int)(x);
	int iy = (int)(y);
	float fx = x - ix;
	float fy = y - iy;
	float A, B;

	float * colors[4];
	float * imgData = (float *)image->getData();
		
	colors[0] = imgData + (width*iy + ix)*components;
	colors[1] = imgData + (width*iy + MIN(width-1, ix+1))*components;
	colors[2] = imgData + (width*MIN(height-1, iy+1) + ix)*components;
	colors[3] = imgData + (width*MIN(height-1, iy+1) + MIN(width-1, ix+1))*components;
	
	for(i=0; i<components; i++)
	{
		A = colors[0][i] + (colors[2][i] - colors[0][i])*fy;
		B = colors[1][i] + (colors[3][i] - colors[1][i])*fy;
		color[i] = A + (B-A)*fx;
	}
}

void getImageSubPixel(MImage * image, float x, float y, float * color)
{
	if(image->getDataType() == M_UBYTE)
		getImageSubPixel_ubyte(image, x, y, color);
		
	else if(image->getDataType() == M_FLOAT)
		getImageSubPixel_float(image, x, y, color);
}
