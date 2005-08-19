TMP_DIR      = .tmp
UI_DIR       = .tmp
MOC_DIR      = .tmp
OBJECTS_DIR  = .tmp

LIBS += ../../src/.libs/libhocr.a

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
