#include <stdio.h>
#include <hocr.h>
#include <hocr_pixbuf.h>
#include <assert.h>

#include "hocr_java.h"

/**************************************************************
	public static native String doOcr_Params(
    byte[] pixels,
    int height,
    int width,
    int rowstride, //0=calculate using n_channels,width
    int n_channels, // 1,3
    int brightness, //0=default
    int command //0=default  
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
   jint command     //0=default
   )   
{
	jstring jstr = NULL;
  hocr_text_buffer *textbuf;
  hocr_pixbuf *pixbuf = hocr_pixbuf_new();
  assert(pixbuf);
	
  pixbuf->n_channels=n_channels; /** number of color channels in the pixpuf (e.g. 3 for red,green,blue). */
  pixbuf->brightness = brightness; /** value from which a gray-scale pixel is considered white. */
  pixbuf->height = height; /** get height in pixels. */
  pixbuf->width = width; /** get width in pixels. */
   
  /** get number of bytes in a raw of pixels. */
  if (rowstride)
    pixbuf->rowstride = rowstride;
  else
  {
    assert(pixbuf->n_channels==3 || pixbuf->n_channels==1);
    pixbuf->rowstride = pixbuf->n_channels * pixbuf->width;
  }
    
  // raw pixpuf data.
  pixbuf->pixels = (unsigned char*)((*env)->GetByteArrayElements(env, pixels, NULL));

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
	
	(*env)->ReleaseByteArrayElements(env, pixels, (signed char*)(pixbuf->pixels), 0);
	pixbuf->pixels = NULL; // hocr_pixbuf_unref() shouldn't free pixel mem
	hocr_pixbuf_unref (pixbuf);  // unref memory
	pixbuf = NULL;

  // create java String object from OCR'ed text
  jstr = (*env)->NewStringUTF(env, textbuf->text);
  
	hocr_text_buffer_unref (textbuf); // unref textbuf

	return jstr;
}
