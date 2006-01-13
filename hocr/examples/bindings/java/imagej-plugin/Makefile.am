
EXTRA_DIST = Makefile.tal readme.txt Hocr_Plugin.java hocr_java.c

if WITH_JAVA

DEPS= $(top_builddir)/src/.libs/libhocr.a

CLEANFILES = *.h *.class *.so *.dll

#if WITH_WIN32
#all-am: hocr_java.dll
#else
all-am: libhocr_java.so
#endif

ImageJ_SRC=$(IMAGEJ_SOURCE_PATH)
jdk=$(JAVA_DEV_KIT_PATH)

#if WITH_WIN32

#hocr_java.dll: hocr_java.c hocr_java.h
#	gcc -mno-cygwin -I${jdk}/include -I${jdk}/include/win32 -Wl,--add-stdcall-alias \
#       -shared -o $@ $< -I$(top_builddir)/src $(DEPS)

#else

libhocr_java.so: hocr_java.c hocr_java.h
	gcc -shared -I$(top_builddir)/src -I${jdk}/include -I${jdk}/include/linux \
	-o $@ $< -I$(top_builddir)/src $(DEPS)

#endif	  

Hocr_Plugin.class: Hocr_Plugin.java
	${jdk}/bin/javac -cp ${ImageJ_SRC}/ij.jar $<

hocr_java.h: Hocr_Plugin.class
	${jdk}/bin/javah -classpath . -o hocr_java.h Hocr_Plugin

#if WITH_WIN32
#install-data-local: hocr_java.dll Hocr_Plugin.class
#	$(mkinstalldirs) $(DESTDIR)${ImageJ_SRC}
#	$(INSTALL_DATA) hocr_java.dll $(DESTDIR)${ImageJ_SRC}/hocr_java.dll
#	$(INSTALL_DATA) Hocr_Plugin.class $(DESTDIR)${ImageJ_SRC}/plugins/Analyze/Hocr_Plugin.class
#else
install-data-local: libhocr_java.so Hocr_Plugin.class
	$(mkinstalldirs) $(DESTDIR)${ImageJ_SRC}/plugins/Analyze
	$(INSTALL_DATA) libhocr_java.so $(DESTDIR)${ImageJ_SRC}/libhocr_java.so
	$(INSTALL_DATA) Hocr_Plugin.class $(DESTDIR)${ImageJ_SRC}/plugins/Analyze/Hocr_Plugin.class

#endif

endif
