
EXTRA_DIST = hocr.i

if WITH_PERL

DEPS= $(top_builddir)/src/libhocr.la

CLEANFILES = *.o *.cxx *.so *wrap* *.pm

all-am: hocr.so
	

hocr.so: hocr_wrap.cxx
	g++ -fpic -c hocr_wrap.cxx $(PE_CFLAGS) -I$(top_srcdir)/src
	g++ -shared ../../src/.libs/libhocr.so hocr_wrap.o -o hocr.so

hocr_wrap.cxx: hocr.i
	swig -perl -c++ hocr.i

install-data-local: hocr.so
	$(mkinstalldirs) $(DESTDIR)$(PE_MODULES_PATH)/auto/hocr/
	$(INSTALL_DATA) hocr.so $(DESTDIR)$(PE_MODULES_PATH)/auto/hocr/hocr.so
	$(INSTALL_DATA) hocr.pm $(DESTDIR)$(PE_MODULES_PATH)/hocr.pm

endif
