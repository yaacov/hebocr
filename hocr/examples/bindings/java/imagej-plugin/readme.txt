About this directory
=============
This directory builds an hocr plugin for ImageJ program.

Files
===
Hocr_Filter.java  -  ImageJ's plugin source file
Hocr_Filter.class  -  ImageJ's plugin compiled file (good for all platforms)
hocr_java.c - source file for JNI library (.dll or .so) that lets Java
		use the c written hocr library
hocr_java.h - header file declaring Java Native Interface calls made by Hocr_Filter.java
Makefile - regular makefile (no automake/autoconf support)
                 Needs manual editing.
readme.txt - this file
hocr_java.dll - hocr Java bindings for hocr (needed by Hocr_Filter.java)
                       This compiled windows version. For others platforms, dynamic library / shared object
                       must be compiled.

About ImageJ
=========
ImageJ is a program for Image Processing and Analysis in Java.
It runs on almost every platform, has a GUI, supports plugins, scripts, etc;
It's Open Source.
Program homepage:   http://rsb.info.nih.gov/ij/
Direct download link: http://rsb.info.nih.gov/ij/download/zips/ij134.zip
A lot of plugin filters and file format filters are also available at the program homepage.

Compiling
=======
Only hocr_java.dll (or hocr_java.so etc.) needs to be compiled.
Edit Makefile and correct the first rule that builds hocr_java.dll to be ok for your system.
[If you want to compile Hocr_Filter.java, you first need to install ImageJ, then you should
adjust ImageJ_SRC variable in the Makefile to point to ij.jar]

Install
====
- Install ImageJ (simply unzip ij134.zip) to some directory.
- copy Hocr_Filter.class to <ImageJ_dir>/plugins/Analyze
- copy hocr_java.dll (or hocr_java.so) to <ImageJ_dir>

Using the Filter
==========
The program lets you manipulate the image in every way you can think of. We can
play with image untill we get the best OCR result.
- Open an image (File->Open)
- Now you can, zoom/out, scroll the image, apply filter, etc.
- Conver the image to 8-bit grayscale (Image->Type->8-bit)
  Current the hocr plugin only supports 8-bit images. (minimal RGB support already there...)
- If you want to: draw a selection box around the area you want to OCR.
  (You might want to zoom in to see exactly what you are doing)
- From the menu: Plugins->Analyze->Hocr Plugin
- a text window with ocr text pops up  (KNOWN BUG: text is displayed left aligned)
  AND
  boxes are drawn around the recognized letters in the image (Use Edit->Undo to remove them)
