#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmptype.h"

#ifndef IMGSIZE
#include "display.h"
#endif

/********************************************************************
 *	Procedure: to save the rgb-image pic[][][3] into 24-bit BMP
 *	image file.
 *	w, h: size of the image
 *  IMGSIZE: size of the 2d array storing the images.
 ********************************************************************/
void SaveBMP24(unsigned char pic[][IMGSIZE][3], int w, int h, 
	       unsigned char filename[])
{
  unsigned char    magic_char[2];
  BITMAPFILEHEADER header;
  BITMAPINFOHEADER info;

  BITMAPCOREHEADER core;

  FILE * fp ;
  RGBTRIPLE pixel ;
  int i, j, k, mod4;
  BYTE tmp = 0 ;

  fprintf(stderr,"size of fileheader=%d, info header = %d\n",
	  sizeof(BITMAPFILEHEADER), sizeof(BITMAPINFOHEADER));
  fp = fopen( filename, "wb" ) ;

  mod4 = (w * 3) % 4 ; // rest number of mod 4
  mod4 = (4 - mod4) % 4 ; // complement of mod 4
  // initialize header
  header.bfOffBits = 54 ;
  header.bfReserved1 = header.bfReserved2 = 0 ;
  header.bfSize = 54 + ((w * 3) + mod4) * h ;
//  header.bfType = 19778 ; // 'MB'

  // wirte BITMAPFILEHEADER into file
  strcpy(magic_char, "BM");
  fwrite( magic_char, sizeof(char), 2, fp);
  fwrite( &header, sizeof( BITMAPFILEHEADER ), 1, fp ) ;

  // initialize info to write
  info.biBitCount = 24 ;
  info.biHeight = h ;
  info.biWidth = w ;
  info.biSize = 40 ;
  info.biPlanes = 1 ;
  info.biClrImportant = info.biClrUsed = info.biCompression = 
    info.biSizeImage = info.biXPelsPerMeter = info.biYPelsPerMeter = 0 ;
  // write BITMAPINFOHEADER into file
  fwrite( &info, sizeof( BITMAPINFOHEADER ), 1, fp ) ;

  core.bcBitCount = 24 ;
  core.bcHeight = h ;
  core.bcWidth = w ;
  core.bcSize = 12 ;
  core.bcPlanes = 1 ;
  //  fwrite( &core, sizeof( BITMAPCOREHEADER ), 1, fp ) ;

  // write all pixels values into BMP image file
  for( i=0 ; i<h ; i++ ){
    for( j=0 ; j<w ; j++ ){
      pixel.rgbtBlue = pic[i][j][2] ; // blue
      pixel.rgbtGreen = pic[i][j][1] ; // green
      pixel.rgbtRed = pic[i][j][0] ; // red
      // write current pixel
      fwrite( &pixel, sizeof( RGBTRIPLE ), 1, fp ) ;
    }
    // align bytes to multiple of 4 on current scanline
    for( k=0 ; k<mod4 ; k++ )
      fwrite( &tmp, sizeof( BYTE ), 1, fp ) ;
  }
  fclose( fp ) ;
}
