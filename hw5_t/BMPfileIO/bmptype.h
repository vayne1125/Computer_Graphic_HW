#ifndef BMPTYPE__H
#define BMPTYPE__H

typedef unsigned int       DWORD;
typedef unsigned char      BYTE;
typedef unsigned short int WORD;
typedef unsigned int       LONG;

typedef struct tagBITMAPINFOHEADER{
  DWORD  biSize; 
  LONG   biWidth; 
  LONG   biHeight; 
  WORD   biPlanes; 
  WORD   biBitCount; 
  DWORD  biCompression; 
  DWORD  biSizeImage; 
  LONG   biXPelsPerMeter; 
  LONG   biYPelsPerMeter; 
  DWORD  biClrUsed; 
  DWORD  biClrImportant; 
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;

typedef struct tagBITMAPFILEHEADER { 
  unsigned short        bfType; 
  unsigned int          bfSize; 
  unsigned short        bfReserved1; 
  unsigned short        bfReserved2; 
  unsigned int          bfOffBits; 
} BITMAPFILEHEADER;

typedef struct tagRGBTRIPLE { 
  BYTE rgbtBlue; 
  BYTE rgbtGreen; 
  BYTE rgbtRed; 
} RGBTRIPLE; 

#endif


/*------------------------------------------------------
 * Procedure to read a BMP file and keep the results in
 * a gray-level array. The intensities are of float type.
 * Input:
 *   filename[]: name of the input file.
 * Output:
 *   width, height: width and height of the image data.
 *   imgData[]: array to keep the intensity.
 * Return 0, if successful, -1, if fail.
 */
int  readBMPfile(char *filename, int *width, int *height, unsigned char **imgData);



/*------------------------------------------------------------------
 *	Procedure: to save the rgb-image pic[] into 24-bit BMP
 *	image file.
 *      pic: the image in unsigned byte data type.
 *	w, h: width & height of the image
 */
void save_rgb_2_BMP24(unsigned char pic[], int w, int h, 
                      unsigned char filename[]);


/*-------------------------------------------------------------
 * Procedure to write a gray-level (0-255) image into a .BMP
 * file. 
 *1. Each pixel is represented by using a floating number.
 *   The gray-level range is [0-255].
 *2. The output .bmp file is not compressed.
 *3. The .bmp file is in RGB format (24-bits).
 *Input:
 *   img[]: the input image,
 *   w, h: the width & height of the image.
 *   filename: name of the .bmp file.
 *Output:
 *   A .bmp file.
 */
void save_float_2_BMP24(float img[], int w, int h, unsigned char filename[]);
