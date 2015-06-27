#
# file: Makefile
#
# This file is part of the NetStreamer software. This file is distributed
# under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
#
# Copyright (C) 1997 Rolf Fokkens
#
# Rel  When   who  What
# 0.9  980114 Rolf Fixed a bug in XxBlocker.cpp on handling select ()
# 0.10 980210 Rolf Added support for playback on 8 bit cards and fixed
#                  some small bugs
# 0.10 980314 Rolf Fixed several bugs
# 0.11 980426 Rolf Added I/O Counters to XxStream
# 0.12 980513 Rolf XxSocket -> XxUdpSocket & XxTcpSocket
# 0.13 980802 Rolf Fixed a bug in XxBlocker
# 0.13 980802 Rolf Fixed a bug in XxTimer
# 0.13 980819 Rolf From now on attention will be paid to timers only when
#                  the select-call doesn't return any work to do..
#                  Changes XxBlocker::Select and XxBlocker::HandleBlocker for
#                  this reason.
#

NR_VERSION = 0.13
INCLUDE = -Iinclude -I../libFksUtil/include -I/usr/include/g++/
# DEBUG = -g
CFLAGS = ${DEBUG} ${INCLUDE} -fPIC -O3 -W -DNR_VERSION=\"${NR_VERSION}\"
LDFLAGS = ${DEBUG} -Xlinker -rpath -Xlinker .
SOFLAGS = ${DEBUG} -shared
MAKEFILE =

XXSTDOBJS = \
	src/XxSoundDev.o src/XxSocket.o src/XxStream.o src/XxBlocker.o \
	src/XxMixer.o src/XxTimer.o src/XxBase.o src/XxSerial.o src/XxStdio.o

.SUFFIXES:
.SUFFIXES: .cpp $(SUFFIXES)

.cpp.o:
	g++ -c ${CFLAGS} -o $@ $<

all:	.depend lib/libXxStd.so.${NR_VERSION} lib/libXxStd.a \
	XxStdTest

archive:
	tar czf ../libXxStd-${NR_VERSION}-src.tgz -C .. \
	`cd ..; ls libXxStd/src/*.cpp libXxStd/include/*.h libXxStd/Makefile`
	tar czf ../libXxStd-${NR_VERSION}-bin.tgz -C .. \
	libXxStd/lib/libXxStd.so.${NR_VERSION} \
	`cd ..; ls libXxStd/lib/* libXxStd/include/*.h`

clean:
	rm .depend src/*.o lib/*.so.* lib/*.so

depend:
	makedepend ${INCLUDE} -f- `ls src/*.cpp` > .depend

.depend:
	mkdir lib; test 0
	ln -s ../*.so .; test 0
	makedepend ${INCLUDE} -f- `ls src/*.cpp` > .depend
	@echo "Retrying Now"

lib/libXxStd.a: ${MAKEFILE} \
	${XXSTDOBJS}
	ar crv lib/libXxStd.a \
	${XXSTDOBJS}

lib/libXxStd.so.${NR_VERSION}: ${MAKEFILE} \
	${XXSTDOBJS}
	g++ ${SOFLAGS} -o lib/libXxStd.so.${NR_VERSION} \
	${XXSTDOBJS} libFksUtil.so
	rm lib/libXxStd.so; test 0
	ln -s libXxStd.so.${NR_VERSION} lib/libXxStd.so
	ln -s lib/libXxStd.so libXxStd.so; test 0

XxStdTest: ${MAKEFILE} libXxStd.so \
	src/XxStdTest.o
	g++ ${LDFLAGS} -o XxStdTest \
	libXxStd.so libFksUtil.so \
	src/XxStdTest.o

include .depend
