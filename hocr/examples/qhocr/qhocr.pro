TMP_DIR      = .tmp
UI_DIR       = .tmp
MOC_DIR      = .tmp
OBJECTS_DIR  = .tmp

win32{
SOURCES    +=  \
  ../../src/font_layout.c \
  ../../src/font_markers.c \
  ../../src/page_layout.c \
  ../../src/hocr.c 
}

unix{
LIBS += ../../src/.libs/libhocr.so
}

TEMPLATE	 = app

FORMS		+= \
	ui/mainwindow.ui

INCLUDEPATH	+= \
	src \
	../../src	

HEADERS		+= \
	src/mainwindow.h 
	
SOURCES		+= \
	src/main.cpp \
	src/mainwindow.cpp
