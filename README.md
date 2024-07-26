# What is it?
It is another streaming application to be used over 24+ kbit/s networks, like LAN's and ISDN. At the moment there are a linux / x86 ELF binary version and a source version available, under the GNU General Public
Licence. NetStreamer offers the possibility to distribute live Audio over network, like Music from internal CD-ROMs or a radio signal on the line-input of a PC.

# Requirements
Well, it may be clear that you need a soundcard, preferably 16 bit. The NetStreamer sound-quality is 16 bit mono at 8 kHz or 16 kHz. Every sample is ADPCM compressed to 3 bits, so the bandwidth is 24 kbit/s or 48 kbit/s. These bandwitdths allowed NetStreamer to run using modems over analogue phone linus in 1998.

Originally in 1998 the Linux OSS sound driver was used. It has been replaced by the Pulsaudio library in 2024, to allow it to run on modern Linux systems.

# How to start it?
NetStreamer consists of a program to "receive" audio (NrReceiver / NrRecFrontend), and a program to "transmit" audio (NrTransmitter). Both programs connect to a NetStreamer Server (NrServer), a kind of reflector that takes care of the distribution of audio. Every transmitter contacts a Server at startup and claims a certain frequency somewhere between 88.0 and 108.0 MHz, and receivers can tune in on one of these transmitter frequencies.

Every NetStreamer program has a parameter that specifies to which IP-Address & Port to listen to or connect to. It's like this:

    example.com:8888

This specifies the host example.com, port 8888. When you specify only :8888, this means port 8888 on local address(es).

## Starting The Server
Port 8888 is the default NetStreamer port. See the file COMMANDS for the commands the server understands when you connect to it. The Server has only this address:port parameter. So you can start the server by entering:

>    NrServer :8888

or

>    NrServer localhost:8888

Your machine is now ready to accept NetStreamer connections

## Starting the transmitter
You can start the Transmitter by entering:

>    NrTransmitter Device 995 "Station 1" example.com:8888

or

>    NrTransmitter Device 995 "Station 1" :8888

This means that the transmitter will claim 99.5 MHz, being labeled Station 1, and it will connect to the specified server. The label is passed to the receiver when it tunes in on 99.5 MHz.

You can also let the transmitter playback prerecorded "tapes". Those are files with the ".tape" extension that contain CCITT_ADPCM compressed data. They can be recorded by using the record button on the Frontend. The way to playback them is like this:

>    NrTransmitter Direcory 995 "Station 1" example.com:8888 /mnt/tape

This makes the transmitter play tape files in the directory /mnt/tape in random order.

## Starting The Receiver
The Receiver can be started by entering:

>    NrReceiver 995 example.com:8888

This means that you want to tune in on 99.5 MHz, after connecting to the specified server. The X-Window receiver can be started by entering:

>    NrRecFrontend example.com:8888

You only specify where to connect, because it has a button to tune into any frequency you like. 

# How to build it?
Clone NetStreamer from https://github.com/rolffokkens/NetStreamer, and do:

>     cd NetStreamer
>     make

Some requirements:

- gcc-c++
- libX11-devel
- libXpm-devel
- pulseaudio-libs-devel


# Win32_Client
There also was a win32 version under development for Win95 and WinNT in 1998.

# Latest release
NetStreamer 0.18 (2024-07-25)
