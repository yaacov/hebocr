
TEMPLATE = app

#LIBS += -lhocr
LIBS += ../../src/.libs/libhocr.a
INCLUDEPATH += ../../src
HEADERS += hocr_qt.h
SOURCES += main.cpp hocr_qt.cpp
