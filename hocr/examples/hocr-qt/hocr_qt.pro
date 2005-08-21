TEMPLATE	 = app

win32{
	SOURCES    +=  \
  ../../src/.libs/libhocr.a
}

unix{
	LIBS += -lhocr
}

HEADERS		+= \
	hocr_qt.h 
	
SOURCES		+= \
	main.cpp \
	hocr_qt.cpp
