#
# file: Makefile
#
# This file is part of the NetStreamer software. This file is distributed
# under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
#
# Copyright (C) 1997 Rolf Fokkens
#
#

NR_VERSION = 0.8
INCLUDE = -I/usr/include/g++ -Iinclude -I../libXxStd/include \
-I../libXxWin/include -I../libFksUtil/include -I../libNrStd/include
# DEBUG = -g
CFLAGS = ${DEBUG} ${INCLUDE} -fPIC -O3 -W -DNR_VERSION=\"${NR_VERSION}\"
LDFLAGS = ${DEBUG} -Xlinker -rpath -Xlinker .
SOFLAGS = ${DEBUG} -shared
MAKEFILE =

NRSTDOBJS = \
	src/NrGraphicAnalyzer.o src/NrRecButton.o src/NrPulseButton.o

.SUFFIXES:
.SUFFIXES: .cpp $(SUFFIXES)

.cpp.o:
	g++ -c ${CFLAGS} -o $@ $<

all:	.depend lib/libNrWin.so.${NR_VERSION} lib/libNrWin.a

archive:
	tar czf ../libNrWin-${NR_VERSION}-src.tgz -C .. \
	`cd ..; ls libNrWin/src/*.cpp libNrWin/include/*.h \
	libNrWin/Makefile libNrWin/COPYING`
	tar czf ../libNrWin-${NR_VERSION}-bin.tgz -C .. \
	`cd ..; ls libNrWin/lib/* libNrWin/include/*.h`

clean:
	rm .depend src/*.o lib/*.so

depend:
	makedepend ${INCLUDE} -f- `ls src/*.cpp` > .depend

.depend:
	mkdir lib; test 0
	ln -s ../*.so .; test 0
	makedepend ${INCLUDE} -f- `ls src/*.cpp` > .depend
	@echo "Retrying Now"

lib/libNrWin.a: ${MAKEFILE} \
	${NRSTDOBJS}
	ar crv lib/libNrWin.a \
	${NRSTDOBJS}

lib/libNrWin.so.${NR_VERSION}: ${MAKEFILE} \
	${NRSTDOBJS}
	g++ ${SOFLAGS} -o lib/libNrWin.so.${NR_VERSION} \
	${NRSTDOBJS} libNrStd.so libXxWin.so
	rm lib/libNrWin.so; test 0
	ln -s libNrWin.so.${NR_VERSION} lib/libNrWin.so

include .depend
