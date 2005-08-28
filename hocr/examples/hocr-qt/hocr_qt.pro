MOC_DIR      = src
OBJECTS_DIR  = src
TMP_DIR      = src

TEMPLATE = app

RESOURCES   += \
	hocr_qt.qrc

# on systems with no dynamic linking use this lines
##INCLUDEPATH += ../../src
##LIBS += ../../src/.libs/libhocr.a

LIBS += -lhocr

HEADERS += src/hocr_qt.h
SOURCES += src/main.cpp src/hocr_qt.cpp
