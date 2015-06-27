#
# file: Makefile
#
# This file is part of the NetStreamer software. This file is distributed
# under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
#
# Copyright (C) 1997 Rolf Fokkens
#
# Who's who?
# Short    Full            e-mail
# Rolf     Rolf Fokkens    rolf@flits102-126.flits.rug.nl
# Dirk-Jan Dirk-Jan Faber  dirk-jan@flits102-126.flits.rug.nl
#
# Rel  When   who      What
# 0.11 980101 Rolf     Added Auto-Detection for 8 / 16 kHz Appeared that 
#                      removing the Recording stuff almost doubles CPU usage.
#                      Strange.
# 0.12 980102 Rolf     Fixed a bug in libXxWin
# 0.12 980118 Rolf     Default are now statically linked executables
# 0.12 980118 Rolf     The Buffer now charges upto 100 % before the stream is
#                      actually played.
# 0.12 980124 Rolf     Added the option to feed the Transmitter with pipe-input
# 0.13 980125 Rolf     Implementation of the Mute Facility
# 0.13 980314 Rolf     Separation of RateAdjust from NrTransStdio, for other use...
# 0.14 980325 Rolf     Fixed "Unable to open test.inp" bug in transmitter code.
# 0.15 980426 Rolf     Added info from trans. about transmitted tape-file
# 0.16 980607 Rolf     Started to design a Logo. I think it's needed to link
#                      The win32 release to this one with a visual "token"
# 0.16 980619 Dirk-Jan Created fancy html pages. Much more up to date
# 0.16 980620 Rolf     Adopted Dirk-Jan's pages, they'll replace the current
#                      ones
# 0.16 980621 Rolf     Started to use C++ exception-handling. I made NetStreamer
#                      using C++, so I'll enjoy the C++ benefits.
# 0.16 980705 Rolf     The competitive looks of the win32 release forced me to
#                      do some cosmetic changes in the frontend.
# 0.16 980707 Rolf     Postponed the C++ gadgets.
# 0.16 980707 Rolf     Finished 0.16.
# 0.17 980718 Rolf     Improved the stream's speed measuring, so lag on the
#                      internet has less effect
# 0.17 980802 Rolf     Fixed a bug in libXxStd, which caused core dumps
#                      with glibc2.
# 0.17 980810 Rolf     Fixed a bug in XxTimer which caused strange behaviour
#                      with long running timers when the time cycled through
#                      the "long" precision after about 49 days.
#                      This caused long running NrServers to disconnect all
#                      transmitters 'cause of "PING TIMEOUTs".
# 0.17 980810 Rolf     Gave up porting NetStreamer to Linux/Alpha after
#                      discovering bugs in egcs-1.0.2. I'll retry after the
#                      bugs have been fixed.
# 0.17 980816 Rolf     Replaced the InitClass stuff by deferring the
#                      initialization of library-parts until needed. This
#                      made NrRecFrontendSh stop core dumping.
# 0.17 980819 Rolf     Got rid of some timing problems in XxBlocker which
#                      caused terrible behaviour on slower hardware.
# 0.17 980819 Rolf     Finished 0.17. (I think)
#
# To Do Stuff:
# What          Explanation
#

LD = g++

#SNAPSHOT = $(shell date +%y%m%d-)
SNAPSHOT=
#DEBUG= -g
DEBUG=

NR_VERSION = 0.17
INCLUDE = -I/usr/include/g++ -IlibXxWin/include -IlibXxStd/include \
	-IlibFksUtil/include -IlibNrWin/include -IlibNrStd/include \
	-IlibAdpcm/include
CFLAGS = ${DEBUG} ${INCLUDE} -O3 -W -DNR_VERSION=\"${NR_VERSION}\"
#LDFLAGS = ${DEBUG} -Xlinker -rpath -Xlinker .
LDFLAGS = ${DEBUG}
SOFLAGS = ${DEBUG} -shared
MAKEFILE =

EXECS = libFksUtil.so libAdpcm.so libXxStd.so libXxWin.so libNrStd.so \
	libNrWin.so NrRecFrontendSh NrServerSh NrTransmitterSh \
	NrReceiverSh NrReceiver NrRecFrontend NrTransmitter NrServer \
	NrEncoder NrEncoderSh

# X11 = -lX11 -lXpm
X11 = -lX11 -lXpm

STATXXUTILLIB  = libFksUtil/lib/libFksUtil.a
STATXXADPCMLIB = libAdpcm/lib/libAdpcm.a
STATXXSTDLIB   = libXxStd/lib/libXxStd.a ${STATXXUTILLIB}
STATXXWINLIB   = libXxWin/lib/libXxWin.a ${STATXXSTDLIB}
STATNRSTDLIB   = libNrStd/lib/libNrStd.a ${STATXXSTDLIB} ${STATXXADPCMLIB}
#STATNRWINLIB   = libNrWin/lib/libNrWin.a ${STATNRSTDLIB} ${STATXXWINLIB}
STATNRWINLIB   = libNrWin/lib/libNrWin.a ${STATNRSTDLIB} libXxWin/lib/libXxWin.a

SUBDIRS  = libAdpcm libFksUtil libXxStd libXxWin libNrStd libNrWin

.SUFFIXES:
.SUFFIXES: .cpp $(SUFFIXES)

.cpp.o:
	g++ -c ${CFLAGS} -o $@ $<

EXECS = libFksUtil.so libAdpcm.so libXxStd.so libXxWin.so libNrStd.so \
	libNrWin.so NrRecFrontendSh NrServerSh NrTransmitterSh \
	NrReceiverSh NrReceiver NrRecFrontend NrTransmitter NrServer \
	NrEncoder NrEncoderSh

all:	.depend SubDirs socktest xtestSh xtest $(EXECS)

SubDirs:
	mkdir -p lib
	for i in $(SUBDIRS); do ln -s $$i/lib/$$i.so .; test 0; done
	for i in $(SUBDIRS); do DEBUG=${DEBUG} make -C $$i; done

archive:
	strip $(EXECS)
	for i in $(SUBDIRS); do make archive -C $$i; done
	cd ..; tar czf NetStreamer-${NR_VERSION}-${SNAPSHOT}src.tgz \
	NetStreamer-${NR_VERSION}/DemoServer \
	NetStreamer-${NR_VERSION}/DemoTransmitter \
	NetStreamer-${NR_VERSION}/DemoReceiver \
	NetStreamer-${NR_VERSION}/DemoFrontend \
	NetStreamer-${NR_VERSION}/DemoEncoder \
	NetStreamer-${NR_VERSION}/*.xbm \
	NetStreamer-${NR_VERSION}/xtest.xpm \
	NetStreamer-${NR_VERSION}/*.cpp \
	NetStreamer-${NR_VERSION}/*.h \
	NetStreamer-${NR_VERSION}/DllTest.c \
	NetStreamer-${NR_VERSION}/Makefile \
	NetStreamer-${NR_VERSION}/README \
	NetStreamer-${NR_VERSION}/COPYING \
	NetStreamer-${NR_VERSION}/COMMANDS \
	NetStreamer-${NR_VERSION}/html/* \
	NetStreamer-${NR_VERSION}/lib*src.tgz
	cd ..; tar czhf NetStreamer-${NR_VERSION}-${SNAPSHOT}bin.tgz \
	NetStreamer-${NR_VERSION}/DemoServer \
	NetStreamer-${NR_VERSION}/DemoTransmitter \
	NetStreamer-${NR_VERSION}/DemoReceiver \
	NetStreamer-${NR_VERSION}/DemoFrontend \
	NetStreamer-${NR_VERSION}/DemoEncoder \
	NetStreamer-${NR_VERSION}/README \
	NetStreamer-${NR_VERSION}/COMMANDS \
	NetStreamer-${NR_VERSION}/NrServer \
	NetStreamer-${NR_VERSION}/NrTransmitter \
	NetStreamer-${NR_VERSION}/NrReceiver \
	NetStreamer-${NR_VERSION}/NrRecFrontend \
	NetStreamer-${NR_VERSION}/NrEncoder \
	NetStreamer-${NR_VERSION}/html/*

clean:
	for i in $(SUBDIRS); do make clean -C $$i; done
	rm *.o *.so NrServerSh NrTransmitterSh NrReceiverSh NrRecFrontendSh \
	NrServer NrTransmitter NrReceiver NrRecFrontend socktest xtest xtestSh

depend:
	gcc -M ${INCLUDE} `ls *.cpp` > .depend
	for i in $(SUBDIRS); do make -C $$i depend; done

.depend:
	gcc -M ${INCLUDE} `ls *.cpp` > .depend
	@echo "Retrying Now"

libFksUtil.so:
	DEBUG=${DEBUG} make -C libFksUtil
	ln -s libFksUtil/lib/libFksUtil.so .; test 0

libAdpcm.so:
	DEBUG=${DEBUG} make -C libAdpcm
	ln -s libAdpcm/lib/libAdpcm.so .; test 0

libXxStd.so:
	DEBUG=${DEBUG} make -C libXxStd
	ln -s libXxStd/lib/libXxStd.so .; test 0

libXxWin.so:
	DEBUG=${DEBUG} make -C libXxWin
	ln -s libXxWin/lib/libXxWin.so .; test 0

libNrStd.so:
	DEBUG=${DEBUG} make -C libNrStd
	ln -s libNrStd/lib/libNrStd.so .; test 0

libNrWin.so:
	DEBUG=${DEBUG} make -C libNrWin
	ln -s libNrWin/lib/libNrWin.so .; test 0

NrDll.so: ${MAKEFILE} \
	NrDll.o
	${LD} ${SOFLAGS} -o NrDll.so \
	NrDll.o libNrStd.so

NrReceiver: ${MAKEFILE} \
	NrRecMain.o ${STATNRSTDLIB}
	${LD} ${LDFLAGS} -o NrReceiver \
 	NrRecMain.o ${STATNRSTDLIB}

NrReceiverSh: ${MAKEFILE} \
	NrRecMain.o
	${LD} ${LDFLAGS} -o NrReceiverSh \
 	NrRecMain.o libXxStd.so libNrStd.so libFksUtil.so libAdpcm.so

NrTransmitter: ${MAKEFILE} \
	NrTransMain.o ${STATNRSTDLIB}
	${LD} ${LDFLAGS} -o NrTransmitter \
	NrTransMain.o ${STATNRSTDLIB}

NrTransmitterSh: ${MAKEFILE} \
	NrTransMain.o
	${LD} ${LDFLAGS} -o NrTransmitterSh \
 	NrTransMain.o libXxStd.so libNrStd.so libFksUtil.so libAdpcm.so

NrEncoder: ${MAKEFILE} \
	NrEncodeMain.o ${STATNRSTDLIB}
	${LD} ${LDFLAGS} -o NrEncoder \
	NrEncodeMain.o ${STATNRSTDLIB}

NrEncoderSh: ${MAKEFILE} \
	NrEncodeMain.o
	${LD} ${LDFLAGS} -o NrEncoderSh \
 	NrEncodeMain.o libXxStd.so libNrStd.so libFksUtil.so libAdpcm.so

DllTest: ${MAKEFILE} \
	DllTest.o
	${LD} ${LDFLAGS} -o DllTest \
 	DllTest.o NrDll.so

NrServerSh: ${MAKEFILE} \
	NrServerMain.o
	${LD} ${LDFLAGS} -o NrServerSh \
 	NrServerMain.o libXxStd.so libNrStd.so libFksUtil.so libAdpcm.so

NrServer: ${MAKEFILE} \
	NrServerMain.o ${STATNRSTDLIB}
	${LD} ${LDFLAGS} -o NrServer \
	NrServerMain.o ${STATNRSTDLIB}

socktest: ${MAKEFILE} \
	socktest.o ${STATXXWINLIB}
	${LD} ${LDFLAGS} -o socktest \
	socktest.o ${STATXXWINLIB} ${X11}

xtest: ${MAKEFILE} \
	xtest.o ${STATXXWINLIB} xtest-xpm.o
	${LD} ${LDFLAGS} -o xtest \
	xtest.o xtest-xpm.o ${STATXXWINLIB} ${X11}

xtestSh: ${MAKEFILE} \
	xtest.o xtest-xpm.o
	${LD} ${LDFLAGS} -o xtestSh \
	xtest.o xtest-xpm.o libXxWin.so libXxStd.so libFksUtil.so

NrRecFrontend: ${MAKEFILE} \
	NrRecFrontend.o ${STATNRWINLIB}
	${LD} ${LDFLAGS} -o NrRecFrontend \
	NrRecFrontend.o ${STATNRWINLIB} ${X11}

NrRecFrontendSh: ${MAKEFILE} \
	NrRecFrontend.o
	${LD} ${LDFLAGS} -o NrRecFrontendSh NrRecFrontend.o libNrWin.so libXxWin.so libNrStd.so libXxStd.so libAdpcm.so libFksUtil.so

include .depend
