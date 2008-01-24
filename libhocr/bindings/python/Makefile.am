
EXTRA_DIST = hocr.i

if WITH_PYTHON

DEPS= $(top_builddir)/src/libhocr.la

CLEANFILES = *.o *.c *.so *wrap* *.py

all-am: _hocr.so
	

_hocr.so: hocr_wrap.c
	gcc -fpic -c hocr_wrap.c $(PY_CFLAGS) $(gtk_CFLAGS) -I$(top_srcdir)/src
	gcc -shared $(gtk_LIBS) $(top_builddir)/src/*.o  hocr_wrap.o -o _hocr.so

hocr_wrap.c: hocr.i
	swig -python hocr.i

install-data-local: _hocr.so
	$(mkinstalldirs) $(DESTDIR)$(PY_MODULES_PATH)
	$(INSTALL_DATA) _hocr.so $(DESTDIR)$(PY_MODULES_PATH)/_hocr.so
	$(INSTALL_DATA) hocr.py $(DESTDIR)$(PY_MODULES_PATH)/hocr.py

endif
