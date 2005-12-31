
TEMPLATE = app

win32
{
	INCLUDEPATH += ../../src
	LIBS += ../../src/.libs/libhocr.a
}

unix
{
	LIBS += -lhocr
}

HEADERS += hocr_qt.h
SOURCES += main.cpp hocr_qt.cpp
