
EXTRA_DIST = hocr.i

if WITH_PYTHON

DEPS= $(top_builddir)/src/libhocr.la

CLEANFILES = *.o *.cxx *.so *wrap* *.py

all-am: _hocr.so
	

_hocr.so: hocr_wrap.cxx
	g++ -fpic -c hocr_wrap.cxx $(PY_CFLAGS) -I$(top_srcdir)/src
	g++ -shared ../../src/.libs/libhocr.so hocr_wrap.o -o _hocr.so

hocr_wrap.cxx: hocr.i
	swig -python -c++ -threads hocr.i

install-data-local: _hocr.so
	$(mkinstalldirs) $(DESTDIR)$(PY_MODULES_PATH)
	$(INSTALL_DATA) _hocr.so $(DESTDIR)$(PY_MODULES_PATH)/_hocr.so
	$(INSTALL_DATA) hocr.py $(DESTDIR)$(PY_MODULES_PATH)/hocr.py

endif
