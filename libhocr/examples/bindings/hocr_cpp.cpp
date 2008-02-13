// 
// hocr_c.c
//
// example file for libhocr.
// compile: g++ `pkg-config --libs --cflags libhocr-gtk` -o hocr_cpp hocr_cpp.cpp
// use: hocr_cpp IMAGE_FILENAME
//
// Copyright (C) 2008  Yaacov Zamir <kzamir@walla.co.il>
//
//

//  
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include <iostream>
#include <hocrpp.h>

#include <ho_gtk.h> // we will use hocr-gtk in this example to load images
#include <gtk/gtk.h> // when we use hocr-gtk we also need gtk 

using namespace std;
using namespace hocr;

int
main (int argc, char * argv[])
{
  ho_pixbuf * pix = NULL;
  Hocr * ocr_obj = NULL;
  
  // we use hocr-gtk, we need to init gtk
  gtk_init (&argc, &argv);
  
  // load the picture pointed to by argv[1]
  /* NOTE: this function use the hocr-gtk functions 
     to load images */
  pix = ho_gtk_pixbuf_load (argv[1]);
  
  // create a new hocr object
  ocr_obj = new Hocr (pix);
  
  // do ocr on picture
  ocr_obj->do_ocr();
  
  // print out the text
  cout << ocr_obj->get_text () << endl;
  
  // free ocr object
  /* NOTE: the picture will be deleted as part of 
     the object */
  delete (ocr_obj);
  
  return 0;
}
