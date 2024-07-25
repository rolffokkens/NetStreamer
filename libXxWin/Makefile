#
# file: Makefile
#
# This file is part of the NetStreamer software. This file is distributed
# under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
#
# Copyright (C) 1997 Rolf Fokkens
#
# Rel  When   who  What
# 0.10 980104 Rolf Removed bug in XxLcdNumDigit
# 0.11 980207 Rolf ...fonts ... ? ...
# 0.12 980816 Rolf Improvements in XxLcdAlphaDigit
# 0.12 980817 Rolf Got rid of lots of InitClass stuff
#
#

NR_VERSION = 0.12
INCLUDE = -Iinclude -I../libFksUtil/include -I../libXxStd/include \
-I/usr/include/g++/
# DEBUG = -g
OPTFLAGS = -O3
CFLAGS =  ${DEBUG} ${INCLUDE} -fPIC ${OPTFLAGS} -W -DNR_VERSION=\"${NR_VERSION}\"
LDFLAGS = ${DEBUG} -Xlinker -rpath -Xlinker .
SOFLAGS = ${DEBUG} -shared
MAKEFILE =

X11 = -lX11 -lXpm

XXWINOBJS = \
	src/XxMaxMeter.o src/XxMeter.o src/XxPanel.o src/XxButton.o \
	src/XxVirtual.o src/XxPixmap.o src/XxBitmap.o src/XxWindow.o \
	src/XxDrawable.o src/XxFont.o src/XxCore.o src/XxGC.o \
	src/XxColor.o src/XxApplication.o src/XxLcdNumDigit.o \
	src/XxLcdNumDisplay.o src/XxLcdAlphaDigit.o \
	src/XxLcdAlphaDisplay.o src/XxLcdScrollDisplay.o src/XxFlashLight.o

.SUFFIXES:
.SUFFIXES: .cpp $(SUFFIXES)

.cpp.o:
	g++ -c ${CFLAGS} -o $@ $<

all:	.depend lib/libXxWin.so.${NR_VERSION} lib/libXxWin.a

archive:
	tar czf ../libXxWin-${NR_VERSION}-src.tgz -C .. \
	`cd ..; ls libXxWin/src/*.xbm libXxWin/src/*.cpp libXxWin/include/*.h \
	libXxWin/Makefile`
	tar czf ../libXxWin-${NR_VERSION}-bin.tgz -C .. \
	`cd ..; ls libXxWin/lib/* libXxWin/include/*.h`

clean:
	rm -f .depend src/*.o lib/*.so.*

depend:
	gcc -M ${INCLUDE} `ls src/*.cpp` > .depend

.depend:
	mkdir -p lib; test 0
	ln -s ../*.so .; test 0
	gcc -M ${INCLUDE} `ls src/*.cpp` > .depend
	@echo "Retrying Now"

lib/libXxWin.a: ${MAKEFILE} \
	${XXWINOBJS}
	ar crv lib/libXxWin.a \
	${XXWINOBJS}

lib/libXxWin.so.${NR_VERSION}: ${MAKEFILE} \
	${XXWINOBJS}
	g++ ${SOFLAGS} -o lib/libXxWin.so.${NR_VERSION} \
	${XXWINOBJS} libXxStd.so ${X11}
	rm lib/libXxWin.so; test 0
	ln -s libXxWin.so.${NR_VERSION} lib/libXxWin.so

ifneq ($(MAKECMDGOALS),clean)
include .depend
endif
