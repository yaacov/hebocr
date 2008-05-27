
EXTRA_DIST = hocr.i

if WITH_PYTHON

DEPS= $(top_builddir)/src/libhocr.la

CLEANFILES = *.o *.cxx *.so *wrap* *.py

all-am: _hocr.so
	
if LINUX_HOST
_hocr.so: hocr_wrap.cxx
	g++ -fPIC -c hocr_wrap.cxx $(PY_CFLAGS) -I$(top_srcdir)/src
	g++ -shared ../../src/.libs/libhocr.so hocr_wrap.o -o _hocr.so
endif

if DARWIN_HOST
_hocr.so: hocr_wrap.cxx
	g++ -dynamic -fPIC -c hocr_wrap.cxx $(PY_CFLAGS) -I$(top_srcdir)/src
	g++ -bundle -bundle_loader $(prefix)/bin/python ../../src/.libs/libhocr.dylib hocr_wrap.o -o _hocr.so
endif

hocr_wrap.cxx: hocr.i
	swig -python -c++ -threads hocr.i

install-data-local: _hocr.so
	$(mkinstalldirs) $(DESTDIR)$(PY_MODULES_PATH)
	$(INSTALL_DATA) _hocr.so $(DESTDIR)$(PY_MODULES_PATH)/_hocr.so
	$(INSTALL_DATA) hocr.py $(DESTDIR)$(PY_MODULES_PATH)/hocr.py

endif
