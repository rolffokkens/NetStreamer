#
# file: Makefile
#
# This file is part of the NetStreamer software. This file is distributed
# under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
#
# Copyright (C) 1997 Rolf Fokkens
#
#

NR_VERSION = 0.9
INCLUDE = -Iinclude
# DEBUG = -g
OPTFLAGS = -O3
CFLAGS =  ${DEBUG} ${INCLUDE} -fPIC ${OPTFLAGS} -W -DNR_VERSION=\"${NR_VERSION}\"
LDFLAGS = ${DEBUG}
SOFLAGS = ${DEBUG} -shared
MAKEFILE =

UTILOBJS = \
	src/BiLink.o src/EzString.o

.SUFFIXES:
.SUFFIXES: .cpp $(SUFFIXES)

.cpp.o:
	g++ -c ${CFLAGS} -o $@ $<

all:	.depend lib/libFksUtil.so.${NR_VERSION} lib/libFksUtil.a

archive:
	tar czf ../libFksUtil-${NR_VERSION}-src.tgz -C .. \
	`cd ..; ls libFksUtil/src/*.cpp libFksUtil/include/*.h \
	libFksUtil/Makefile`
	tar czf ../libFksUtil-${NR_VERSION}-bin.tgz -C.. \
	libFksUtil/lib/libFksUtil.so.${NR_VERSION} \
	libFksUtil/lib/libFksUtil.a \
	`cd ..; ls libFksUtil/include/*.h`

clean:
	rm -f .depend src/*.o lib/*.so.* lib/*.a

depend:
	gcc -M ${INCLUDE} `ls src/*.cpp` > .depend

.depend:
	mkdir -p lib; test 0
	gcc -M ${INCLUDE} `ls src/*.cpp` > .depend
	@echo "Retrying Now"

lib/libFksUtil.a: ${MAKEFILE} \
	${UTILOBJS}
	ar crv lib/libFksUtil.a \
	${UTILOBJS}

lib/libFksUtil.so.${NR_VERSION}: ${MAKEFILE} \
	${UTILOBJS}
	g++ ${SOFLAGS} -o lib/libFksUtil.so.${NR_VERSION} \
	${UTILOBJS}
	rm lib/libFksUtil.so; test 0
	ln -s libFksUtil.so.${NR_VERSION} lib/libFksUtil.so

ifneq ($(MAKECMDGOALS),clean)
include .depend
endif
