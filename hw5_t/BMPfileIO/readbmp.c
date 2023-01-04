#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "bmptype.h"



/*-------------------------------------------------------
 * Local procedure to read a true-colored BMP file.
 * Assume the file header and info header have been read.
 */
int readBMP24(FILE *fp, BITMAPINFOHEADER *info, unsigned char *imgData)
{
  unsigned char  buf[3*4096];  //Buffer to keep a row of image.
  int            rowSize; //Real size of a row, including padding bytes.
  int            w, h; //width and height of the image.
  int            i, j, k, count, flag=0;

  
  w = info->biWidth;
  h = info->biHeight;
  rowSize = w*3;
  rowSize = ((rowSize + 3)>>2) << 2;

  //Read the pixels and convert them to gray-level
  k = 0;  //index of the pixel in the imgData[] array.
  for(j=0;j<h;j++){
    count = fread(buf, 1, rowSize, fp);
    if(count<rowSize){
      fprintf(stderr,"Warning:BMP file corrupted. Data are missing.\n");
      flag = -1;
    }
    for(i=0;i<w;i++){
      //Store pixel values.
      //Remember, BMP store pixel values in the "BGR" order !!!
      imgData[k] = buf[i*3+2];
	  imgData[k+1] = buf[i*3+1];
	  imgData[k+2] = buf[i*3];
      k = k + 3;
    }
  }
  return(flag);
}

  
/*-------------------------------------------------------
 * Local procedure to read a 256-colored BMP file.
 * Assume the file header and info header have been read.
 */
int readBMP8(FILE *fp, BITMAPINFOHEADER *info, unsigned char *imgData)
{
  unsigned char  buf[3*4096];  //Buffer to keep a row of image.
  unsigned char  gbr[256][4];
  int            rowSize; //Real size of a row, including padding bytes.
  int            w, h; //width and height of the image.
  int            i, j, k, count, flag=0;


  //Read the colormap first.
  fread(&(gbr[0][0]), 1, 256*4, fp);

  //Read the data row by row.
  w = info->biWidth;          // Image width.
  h = info->biHeight;
 
  rowSize = ((w+3)>>2) << 2;  //Add pending bytes.

  //Read the pixels and convert them to gray-level
  k = 0;  //index of the pixel in the imgData[] array.
  for(j=0;j<h;j++){
    //Read one row of data.
    count = fread(buf, 1, rowSize, fp);
    if(count<rowSize){
      fprintf(stderr,"Warning:BMP file corrupted. Data are missing.\n");
      flag = -1;
    }
    for(i=0;i<w;i++){
      //Convert RGB into gray-level.
      //Remember, BMP store pixel values in the "BGR" order !!!
      imgData[k] = gbr[buf[i]][2];
	  imgData[k+1] = gbr[buf[i]][1];
	  imgData[K+2] = gbr[buf[i]][0];
 
      k = k + 3;
    }
  }
  return(flag);
}


/*-------------------------------------------------------
 * Local procedure to read a black&white BMP file.
 * Assume the file header and info header have been read.
 */
int readBMP1(FILE *fp, BITMAPINFOHEADER *info, float *imgData)
{
  //Not implemented yet !
  return(-1);
}


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
int  readBMPfile(char *filename, int *width, int *height, unsigned char **imgData)
{
  FILE   *fp;
  BITMAPFILEHEADER   header;
  BITMAPINFOHEADER   info;
  int     flag=0;


  //Open the file.
  fp = fopen(filename, "rb");
  if(fp==NULL){
    fprintf(stderr,"BMP file %s not exists !\n", filename);
    return(-1);
  }
 
  // Read the file header.
  // Don't read the file header by using the following instruction.
  // The compiler may add 2 extra bytes after the bfType field for
  //   matching the memory alignment.
  //  fread(&header, sizeof(BITMAPFILEHEADER), 1, fp);
  fread(&(header.bfType), sizeof(unsigned short), 1, fp);
  fread(&(header.bfSize), sizeof(unsigned int), 1, fp);
  fread(&(header.bfReserved1), sizeof(unsigned short), 1, fp);
  fread(&(header.bfReserved2), sizeof(unsigned short), 1, fp);
  fread(&(header.bfOffBits), sizeof(unsigned int), 1, fp);

  if(header.bfType!=19778){
    fprintf(stderr,"Error:File %s is not a BMP file! \n", filename);
    return(-1);
  }
  
  // Read the image infomation segment.
  fread(&info, sizeof(BITMAPINFOHEADER), 1, fp);
  //Check whether the new image has different shape.
  if(*width != info.biWidth || *height != info.biHeight){
    *width = info.biWidth;
    *height = info.biHeight;
    if(*imgData!=NULL){
      free(*imgData);
      *imgData = NULL;
    }
  }

  //  fprintf(stderr," image width, height= %d %d\n", *width, *height);

  // If info.biSize=12 bytes, not 40 bytes, means an OS2 file.
  // Don't read it.
  if(info.biSize==12){
    fprintf(stderr,"File %s is for OS2. Can't read it\n", filename);
    return(-1);
  }

  // Check the compression flag. Assume no compression.
  // If compressed, return -1. Don't read it.
  if(info.biCompression!=0){
    fprintf(stderr,"Can't read compressed BMP file %s\n", filename);
    return(-1);
  }

  //Allocate space for the input data, if necessary.
  if(*imgData==NULL)
    *imgData = (float *) malloc(sizeof(float)*(*width)*(*height));

  // Check the num. of bits for representing a pixel.
  // and call a suitable procedure to read the data.
  if(info.biBitCount==24||info.biBitCount==0)
    flag = readBMP24(fp, &info, *imgData);
  else if(info.biBitCount==8)
    flag = readBMP8(fp, &info,  *imgData);
  else if(info.biBitCount==1)
    flag = readBMP1(fp, &info, *imgData);
  else{
    fprintf(stderr,"Can't read BMP file %s\n", filename);
    fprintf(stderr,"Num. of bits, per pixel=%d\n", info.biBitCount);
    return(-1);
  }
  // return the flag to indicate the validity of the data file.
  return(flag);
}

