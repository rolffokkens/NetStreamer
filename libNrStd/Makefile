#
# file: Makefile
#
# This file is part of the NetStreamer software. This file is distributed
# under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
#
# Copyright (C) 1997 Rolf Fokkens
#
# Rel  When   who  What
# 0.11 980101 Rolf Added Auto-Detection for 8 / 16 kHz Appeared that removing 
#                  the Recording stuff almost doubles CPU usage. Strange.
# 0.12 980118 Rolf The Buffer now charges upto 100 % before the stream is
#                  actually played.
# 0.12 980124 Rolf Added the option to feed the Transmitter with pipe-input
# 0.13 980125 Rolf Implementation of the Mute Facility
# 0.13 980314 Rolf Separation of RateAdjust from NrTransStdio, for other use...
# 0.14 980322 Rolf Fixed the "Unable to open test.inp" bug: if so, try the next
# 0.15 980322 Rolf messages?? Title? Artist...
# 0.15 980521 Rolf Changed NrTransFiles to prevent it from playing the same
#                  song twice
# 0.16 980607 Rolf Additional debugging info on lost connections
# 0.17 980718 Rolf Rewrote NrSpeedCalc. It hopefully measures the stream's
#                  speed better now.
#

NR_VERSION = 0.17
FEATURE_LEVEL = 1
INCLUDE = -I/usr/include/g++ -Iinclude -I../libXxStd/include \
	-I../libXxWin/include -I../libFksUtil/include -I../libAdpcm/include
# DEBUG = -g
CFLAGS = ${DEBUG} ${INCLUDE} -fPIC -O3 -W \
	-DFEATURE_LEVEL=${FEATURE_LEVEL} -DNR_VERSION=\"${NR_VERSION}\"
LDFLAGS = ${DEBUG} -Xlinker -rpath -Xlinker .
SOFLAGS = ${DEBUG} -shared
MAKEFILE =

NRSTDOBJS = \
	src/NrRecPump.o src/NrAnalyzer.o src/NrTimeLevelBuf.o \
	src/NrSpeedBuf.o src/NrFifo.o src/NrSpeedCalc.o src/NrComp.o \
	src/NrDecomp.o src/NrReceiver.o src/NrTransmitter.o \
	src/NrTransSoundDev.o src/NrTransFiles.o src/NrTransStdIn.o \
	src/NrServer.o src/NrNetRadio.o src/NrMessages.o src/Nr8Log.o \
	src/NrHistSoundBuf.o src/NrRateAdjust.o src/NrPresetMemory.o

.SUFFIXES:
.SUFFIXES: .cpp $(SUFFIXES)

.cpp.o:
	g++ -c ${CFLAGS} -o $@ $<

all:	.depend lib/libNrStd.so.${NR_VERSION} lib/libNrStd.a

archive:
	tar czf ../libNrStd-${NR_VERSION}-src.tgz -C .. \
	`cd ..; ls libNrStd/src/*.cpp libNrStd/include/*.h \
	libNrStd/Makefile libNrStd/COPYING`
	tar czf ../libNrStd-${NR_VERSION}-bin.tgz -C .. \
	`cd ..; ls libNrStd/lib/* libNrStd/include/*.h`

clean:
	rm src/*.o lib/*.so.* lib/*.a

depend:
	makedepend ${INCLUDE} -f- `ls src/*.cpp` > .depend

.depend:
	mkdir lib; test 0
	ln -s ../*.so .; test 0
	makedepend ${INCLUDE} -f- `ls src/*.cpp` > .depend
	@echo "Retrying Now"

lib/libNrStd.a: ${MAKEFILE} \
	${NRSTDOBJS}
	ar cr lib/libNrStd.a \
	${NRSTDOBJS}

lib/libNrStd.so.${NR_VERSION}: ${MAKEFILE} \
	${NRSTDOBJS}
	g++ ${SOFLAGS} -o lib/libNrStd.so.${NR_VERSION} \
	${NRSTDOBJS} libXxStd.so libAdpcm.so
	rm lib/libNrStd.so; test 0
	ln -s libNrStd.so.${NR_VERSION} lib/libNrStd.so

include .depend
