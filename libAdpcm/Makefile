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
INCLUDE = -Iinclude
# DEBUG = -g
# CFLAGS = ${DEBUG} ${INCLUDE} -fno-exceptions -fPIC -O3 -W -DNR_VERSION=\"${NR_VERSION}\"
CFLAGS = ${DEBUG} ${INCLUDE} -fPIC -O3 -W -DNR_VERSION=\"${NR_VERSION}\"
LDFLAGS = ${DEBUG} -Xlinker -rpath -Xlinker .
SOFLAGS = ${DEBUG} -shared
MAKEFILE =

ADPCMOBJS = \
	src/g711.o src/g721.o src/g723_24.o src/g723_40.o src/g72x.o

.SUFFIXES:
.SUFFIXES: .cpp $(SUFFIXES)

.cpp.o:
	g++ -c ${CFLAGS} -o $@ $<

all:	.depend lib/libAdpcm.a lib/libAdpcm.so.${NR_VERSION}

archive:
	tar czf ../libAdpcm-${NR_VERSION}-src.tgz -C .. \
        `cd ..; ls libAdpcm/src/*.cpp libAdpcm/include/*.h libAdpcm/Makefile`
	tar czf ../libAdpcm-${NR_VERSION}-bin.tgz -C .. \
        `cd ..; ls libAdpcm/lib/* libAdpcm/include/*.h`

clean:
	rm -f .depend src/*.o lib/*.a lib/*.so.*; test 0

depend:
	gcc -M ${INCLUDE} `ls src/*.cpp` > .depend

.depend:
	mkdir -p lib; test 0
	gcc -M ${INCLUDE} `ls src/*.cpp` > .depend
	@echo "Retrying Now"

lib/libAdpcm.a: ${MAKEFILE} \
	${ADPCMOBJS}
	ar crv lib/libAdpcm.a \
	${ADPCMOBJS}

lib/libAdpcm.so.${NR_VERSION}: ${MAKEFILE} \
	${ADPCMOBJS}
	g++ ${SOFLAGS} -o lib/libAdpcm.so.${NR_VERSION} \
	${ADPCMOBJS}
	rm lib/libAdpcm.so; test 0
	ln -s libAdpcm.so.${NR_VERSION} lib/libAdpcm.so

ifneq ($(MAKECMDGOALS),clean)
include .depend
endif
