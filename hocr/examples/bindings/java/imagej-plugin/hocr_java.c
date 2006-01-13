#include <stdio.h>
#include <hocr.h>
#include <hocr_pixbuf.h>
#include <assert.h>

#include "hocr_java.h"

JNIEXPORT jstring JNICALL Java_Pixbuf_doOcr
  (JNIEnv *env, jobject obj)
{
  hocr_text_buffer *textbuf;
  hocr_pixbuf *pixbuf = hocr_pixbuf_new();
  assert(pixbuf);
	
  pixbuf->n_channels=3; /** number of color channels in the pixpuf (e.g. 3 for red,green,blue). */
  //pixbuf->n_channels=1; /** number of color channels in the pixpuf (e.g. 3 for red,green,blue). */
  pixbuf->brightness = 100; /** value from which a gray-scale pixel is considered white. */
  pixbuf->height = 200; /** get height in pixels. */
  pixbuf->width = 200; /** get width in pixels. */
   
  /** get number of bytes in a raw of pixels. */
  assert(pixbuf->n_channels==3 || pixbuf->n_channels==1);
  if (pixbuf->n_channels == 3)
    pixbuf->rowstride = 3*pixbuf->width;
  else
    pixbuf->rowstride = 1*pixbuf->width;
    
  // raw pixpuf data.
  pixbuf->pixels =  (unsigned char*)
    malloc(pixbuf->rowstride * pixbuf->height);
  assert(pixbuf->pixels); // TODO: check, report, cleanup, abort
  
	// build ocr command 
	pixbuf->command  = HOCR_COMMAND_OCR;
  pixbuf->command |= HOCR_COMMAND_NIKUD; // use nikud
  pixbuf->command |= HOCR_COMMAND_USE_SPACE_FOR_TAB; // use spaces
  pixbuf->command |= HOCR_COMMAND_USE_INDENTATION; // use indentation
  pixbuf->command |= HOCR_COMMAND_COLOR_BOXES;
  pixbuf->command |= HOCR_COMMAND_COLOR_MISREAD;

	textbuf = hocr_text_buffer_new (); // create text buffer
	assert(textbuf);
	hocr_do_ocr (pixbuf, textbuf); // run the ocr

	//pixbuf->pixels = NULL;
	hocr_pixbuf_unref (pixbuf);  // unref memory
	pixbuf = NULL;

  ////fputs(textbuf->text,fp);
	hocr_text_buffer_unref (textbuf); // unref textbuf

	printf("Hello World!\n", (void*)&hocr_do_ocr);
	{
	jstring js = (*env)->NewStringUTF(env,"tal");
	return js;
	return NULL;
  }
}

/**************************************************************
	public static native String doOcr_Params(
    byte[] pixels,
    int height,
    int width,
    int rowstride, //0=calculate using n_channels,width
    int n_channels, // 1,3
    int brightness, //0=default
    int command //0=default  
    int start_x,    // ocr will start at start_x, start_y
    int start_y     // -"-
  );
**************************************************************/
/*
 * Class:     Hocr_Plugin
 * Method:    doOcr_Params
 * Signature: ([BIIIIII)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_Hocr_1Plugin_doOcr_1Params
  (JNIEnv *env,
   jclass cl,
   jbyteArray pixels,
   jint height,
   jint width,
   jint rowstride,  //0=calculate using n_channels,width
   jint n_channels, // 1,3,
   jint brightness,
   jint command,    //0=default
   jint start_x,    // ocr will start at start_x, start_y
   jint start_y     // -"-
   )   
{
	jstring jstr = NULL;
	jbyte *pixel_arr_elements;
  hocr_text_buffer *textbuf;
  hocr_pixbuf *pixbuf = hocr_pixbuf_new();
  assert(pixbuf);
	
  pixbuf->n_channels=n_channels; /** number of color channels in the pixpuf (e.g. 3 for red,green,blue). */
  //pixbuf->n_channels=1; /** number of color channels in the pixpuf (e.g. 3 for red,green,blue). */
  pixbuf->brightness = brightness; /** value from which a gray-scale pixel is considered white. */
  pixbuf->height = height; /** get height in pixels. */
  pixbuf->width = width; /** get width in pixels. */
   
  /** get number of bytes in a raw of pixels. */
  if (rowstride)
    pixbuf->rowstride = rowstride;
  else
  {
    assert(pixbuf->n_channels==3 || pixbuf->n_channels==1);
    if (pixbuf->n_channels == 3)
      pixbuf->rowstride = 3*pixbuf->width;
    else
      pixbuf->rowstride = 1*pixbuf->width;
  }
    
  // raw pixpuf data.
  pixel_arr_elements = (*env)->GetByteArrayElements(env, pixels, NULL);

  // ocr should start at start_x, start_y
  //  so we skip appropriate number of bytes from pixel data
  pixbuf->pixels = pixel_arr_elements
    + start_x*pixbuf->n_channels + start_y*pixbuf->rowstride;
  
  if (!command)
  {
  	// build ocr command 
  	pixbuf->command  = HOCR_COMMAND_OCR;
    pixbuf->command |= HOCR_COMMAND_NIKUD; // use nikud
    pixbuf->command |= HOCR_COMMAND_USE_SPACE_FOR_TAB; // use spaces
    pixbuf->command |= HOCR_COMMAND_USE_INDENTATION; // use indentation
    pixbuf->command |= HOCR_COMMAND_COLOR_BOXES;
    pixbuf->command |= HOCR_COMMAND_COLOR_MISREAD;
  }
  else
    pixbuf->command = command;

	textbuf = hocr_text_buffer_new (); // create text buffer
	assert(textbuf);
	
	hocr_do_ocr (pixbuf, textbuf); // run the ocr
	
  (*env)->ReleaseByteArrayElements(env, pixels, pixel_arr_elements, 0);
	pixel_arr_elements = pixbuf->pixels = NULL; // hocr_pixbuf_unref() shouldn't free pixel mem
	hocr_pixbuf_unref (pixbuf);  // unref memory
	pixbuf = NULL;

  // create java String object from OCR'ed text
  jstr = (*env)->NewStringUTF(env, textbuf->text);
  
	hocr_text_buffer_unref (textbuf); // unref textbuf

	return jstr;
}

