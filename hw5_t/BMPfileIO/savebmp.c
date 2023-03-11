#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmptype.h"


/*------------------------------------------------------------------
 *	Procedure: to save the rgb-image pic[] into 24-bit BMP
 *	image file.
 *      pic: the image in unsigned byte data type.
 *	w, h: width & height of the image
 */
void save_rgb_2_BMP24(unsigned char pic[], int w, int h, 
                      unsigned char filename[])
{
  BITMAPFILEHEADER header;
  BITMAPINFOHEADER info;
  FILE * fp ;
  RGBTRIPLE pixel ;
  int i, j, k, mod4;
  BYTE tmp[4] = {0} ;

  fprintf(stderr,"size of fileheader=%d, info header = %d\n",
	  sizeof(BITMAPFILEHEADER), sizeof(BITMAPINFOHEADER));
  fp = fopen( filename, "wb" ) ;

  mod4 = (w * 3) % 4 ; // remainder of (row-length mod 4)
  mod4 = (4 - mod4) % 4 ; // number of bytes to be appended.

  // initialize header
  header.bfOffBits = 54 ;   //Offset to image chunk.
  header.bfReserved1 = header.bfReserved2 = 0 ;
  header.bfSize = 54 + ((w * 3) + mod4) * h ;
  header.bfType = 19778 ; // 'MB'

  // wirte BITMAPFILEHEADER into file
  // Don't write the header by using the following instruction.
  // The BITMAPFILEHEADER data type is of 16 bytes not 14 bytes !!!
  // This instruction add two extra bytes after the bfType field.
  //  fwrite( &header, sizeof( BITMAPFILEHEADER ), 1, fp ) ;
  fwrite(&(header.bfType), sizeof(unsigned short), 1, fp);
  fwrite(&(header.bfSize), sizeof(unsigned int), 1, fp);
  fwrite(&(header.bfReserved1), sizeof(unsigned short), 1, fp);
  fwrite(&(header.bfReserved2), sizeof(unsigned short), 1, fp);
  fwrite(&(header.bfOffBits), sizeof(unsigned int), 1, fp);

  // initialize info to write
  info.biBitCount = 24 ;   //true-colored
  info.biHeight = h ;      //Image height & width
  info.biWidth = w ;
  info.biSize = 40 ;       //Size of the info header
  info.biPlanes = 1 ;      //must be 1
  //uncompressed BMP file. Leave the following fields 0.
  info.biClrImportant = info.biClrUsed = info.biCompression = 
    info.biSizeImage = info.biXPelsPerMeter = info.biYPelsPerMeter = 0 ;
  // write BITMAPINFOHEADER into file
  fwrite( &info, sizeof( BITMAPINFOHEADER ), 1, fp ) ;

  // write all pixels values into BMP image file
  for( i=0 ; i<h ; i++ ){
    for( j=0 ; j<w ; j++ ){
      pixel.rgbtRed = pic[3*i*w + 3*j]; // red
      pixel.rgbtGreen = pic[3*i*w + 3*j + 1] ; // green
      pixel.rgbtBlue = pic[3*i*w + 3*j + 2] ; // blue
      // write current pixel
      fwrite( &pixel, sizeof( RGBTRIPLE ), 1, fp ) ;
    }
    // align bytes to multiple of 4 on current scanline
    fwrite( &(tmp[0]), sizeof( BYTE ), mod4, fp ) ;
  }
  fclose( fp ) ;
}



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
void save_float_2_BMP24(float img[], int w, int h, unsigned char filename[])
{
  BITMAPFILEHEADER header;
  BITMAPINFOHEADER info;
  FILE * fp ;
  RGBTRIPLE pixel ;
  int i, j, k, mod4;
  BYTE tmp[4] = {0};


  fprintf(stderr,"size of fileheader=%d, info header = %d\n",
	  sizeof(BITMAPFILEHEADER), sizeof(BITMAPINFOHEADER));
  fp = fopen( filename, "wb" ) ;

  mod4 = (w*3) % 4 ; // remainder of (row-length mod 4)
  mod4 = (4-mod4) % 4 ; // number of bytes to be appended.

  // initialize header
  header.bfOffBits = 54 ;   //Offset to image chunk.
  header.bfReserved1 = header.bfReserved2 = 0 ;
  header.bfSize = 54 + ((w * 3) + mod4) * h ;
  header.bfType = 19778 ; // 'MB'

  // wirte BITMAPFILEHEADER into file
  // Don't write the header by using the following instruction.
  //  fwrite( &header, sizeof( BITMAPFILEHEADER ), 1, fp ) ;
  // The BITMAPFILEHEADER data type is of 16 bytes not 14 bytes !!!
  // This instruction add two extra bytes after the bfType field.
  fwrite(&(header.bfType), sizeof(unsigned short), 1, fp);
  fwrite(&(header.bfSize), sizeof(unsigned int), 1, fp);
  fwrite(&(header.bfReserved1), sizeof(unsigned short), 1, fp);
  fwrite(&(header.bfReserved2), sizeof(unsigned short), 1, fp);
  fwrite(&(header.bfOffBits), sizeof(unsigned int), 1, fp);

  // initialize info to write
  info.biBitCount = 24;   //true-colored
  info.biHeight = h;      //Image height & width
  info.biWidth = w;
  info.biSize = 40;       //Size of the info header
  info.biPlanes = 1;      //must be 1
  //uncompressed BMP file. Leave the following fields 0.
  info.biClrImportant = info.biClrUsed = info.biCompression = 
    info.biSizeImage = info.biXPelsPerMeter = info.biYPelsPerMeter = 0;
  // write BITMAPINFOHEADER into file
  fwrite( &info, sizeof( BITMAPINFOHEADER ), 1, fp);

  //write all pixels into the .BMP file
  for( i=0 ; i<h ; i++ ){
    for( j=0 ; j<w ; j++ ){
      pixel.rgbtBlue = (unsigned char) img[i*w + j]; // blue
      pixel.rgbtGreen = (unsigned char) img[i*w + j]; // green
      pixel.rgbtRed = (unsigned char) img[i*w + j]; // red
      // write current pixel
      fwrite(&pixel, sizeof( RGBTRIPLE ), 1, fp);
    }
    //align row-length to multiple of 4 on current row
    fwrite( &(tmp[0]), sizeof(BYTE), mod4, fp ) ;
  }
  fclose( fp ) ;
}
