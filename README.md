# What is it?
It is another streaming application to be used over 24+ kbit/s networks, like LAN's and ISDN. At the moment there are a linux / x86 ELF binary version and a source version available, under the GNU General Public
Licence. NetStreamer offers the possibility to distribute live Audio over network, like Music from internal CD-ROMs or a radio signal on the line-input of a PC.

# Requirements
Well, it may be clear that you need a soundcard, preferably 16 bit. The NetStreamer sound-quality is 16 bit mono at 8 kHz or 16 kHz. Every sample is ADPCM compressed to 3 bits, so the bandwidth is 24 kbit/s or 48 kbit/s. These bandwitdths allowed NetStreamer to run using modems over analogue phone linus in 1998.

Originally in 1998 the Linux OSS sound driver was used. It has been replaced by the Pulsaudio library in 2024, to allow it to run on modern Linux systems.

# How to start it?
NetStreamer consists of a program to "receive" audio (NrReceiver / NrRecFrontend), and a program to "transmit" audio (NrTransmitter). Both programs connect to a NetStreamer Server (NrServer), a kind of reflector that takes care of the distribution of audio. Every transmitter contacts a Server at startup and claims a certain frequency somewhere between 88.0 and 108.0 MHz, and receivers can tune in on one of these transmitter frequencies.

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
