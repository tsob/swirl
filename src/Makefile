# Makefile for:
#
#       Swirl
#   The Sonic World
#
# Reza Payami, Tim O'Brien, Haley Sayres
# Fall 2013

# Project subdirectories
PROJECTROOT=$(CURDIR)
COREDIR=core/
MAINDIR=main/
APIDIR=api/
RTAUDIODIR=$(APIDIR)rtaudio/
STKDIR=$(APIDIR)stk/
FLUIDSYNTHDIR=$(APIDIR)fluidsynth/
XAPIDIR=$(APIDIR)x-api/
YAPIDIR=$(APIDIR)y-api/
OSCPACKDIR=$(APIDIR)oscpack_1_1_0/
OSCPACKIPDIR=$(OSCPACKDIR)ip/
OSCPACKIPPDIR=$(OSCPACKDIR)ip/posix/
OSCPACKOSCDIR=$(OSCPACKDIR)osc/


CXX=g++
INCLUDES=-Imain/ -Icore/ -I$(RTAUDIODIR) -I$(STKDIR)\
				 -I$(XAPIDIR) -I$(YAPIDIR) -I$(OSCPACKDIR) -I$(OSCPACKIPDIR) \
				 -I$(OSCPACKIPPDIR) -I$(OSCPACKOSCDIR) -I$(COREDIR)
UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
FLAGS=-D__LINUX_ALSASEQ__ -D__UNIX_JACK__ -c -g
LIBS=-lasound -lpthread -ljack -lstdc++ -lm \
    -lglut -lGL -lGLU -lfluidsynth
endif
ifeq ($(UNAME), Darwin)
FLAGS=-D__MACOSX_CORE__ -c
LIBS=-framework CoreAudio -framework CoreMIDI -framework CoreFoundation \
	-framework IOKit -framework Carbon  -framework opengl \
	-framework GLUT -framework Foundation \
	-framework AppKit -lm -lfluidsynth -lstdc++
endif

OBJS=RtAudio.o \
		 Delay.o DelayL.o Stk.o \
		 x-buffer.o x-fun.o x-gfx.o x-loadlum.o x-loadrgb.o \
		 x-thread.o x-vector3d.o y-charting.o y-echo.o y-entity.o \
		 y-fft.o y-particle.o  y-waveform.o y-fluidsynth.o \
		 $(OSCPACKIPPDIR)NetworkingUtils.o $(OSCPACKIPPDIR)UdpSocket.o \
		 $(OSCPACKIPDIR)IpEndpointName.o \
		 $(OSCPACKOSCDIR)OscOutboundPacketStream.o \
		 $(OSCPACKOSCDIR)OscPrintReceivedElements.o \
		 $(OSCPACKOSCDIR)OscReceivedElements.o $(OSCPACKOSCDIR)OscTypes.o \
		 raka-audio.o raka-entity.o raka-gfx.o raka-globals.o raka-sim.o \
		 main.o
# Note: leaving out y-score-reader.o

swirl: $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LIBS)

# main
main.o: $(MAINDIR)main.cpp
	$(CXX) $(FLAGS) $(INCLUDES) $(MAINDIR)main.cpp

# core
raka-audio.o: y-fluidsynth.o x-audio.o core/raka-audio.h core/raka-audio.cpp \
	RtAudio.o
	$(CXX) $(FLAGS) $(INCLUDES) -I$(FLUIDSYNTHDIR)include/ -L$(FLUIDSYNTHDIR) \
		core/raka-audio.cpp
raka-entity.o: core/raka-entity.h core/raka-entity.cpp
	$(CXX) $(FLAGS) $(INCLUDES) core/raka-entity.cpp
raka-gfx.o: core/raka-gfx.h core/raka-gfx.cpp
	$(CXX) $(FLAGS) $(INCLUDES) core/raka-gfx.cpp
raka-globals.o: core/raka-globals.h core/raka-globals.cpp
	$(CXX) $(FLAGS) $(INCLUDES) core/raka-globals.cpp
raka-sim.o: core/raka-sim.h core/raka-sim.cpp
	$(CXX) $(FLAGS) $(INCLUDES) core/raka-sim.cpp

# rtaudio
RtAudio.o: $(RTAUDIODIR)RtAudio.h $(RTAUDIODIR)RtAudio.cpp
	$(CXX) $(FLAGS) $(RTAUDIODIR)RtAudio.cpp

# stk
Delay.o: $(STKDIR)Delay.h $(STKDIR)Delay.cpp
	$(CXX) $(FLAGS) $(STKDIR)Delay.cpp
DelayL.o: $(STKDIR)DelayL.h $(STKDIR)DelayL.cpp
	$(CXX) $(FLAGS) $(STKDIR)DelayL.cpp
#Filter.o: Filter.h Filter.cpp
#	$(CXX) $(FLAGS) $(INCLUDES) Filter.cpp
#MidiFileIn.o: MidiFileIn.h MidiFileIn.cpp
	#$(CXX) $(FLAGS) $(INCLUDES) MidiFileIn.cpp
Stk.o: $(STKDIR)Stk.h $(STKDIR)Stk.cpp
	$(CXX) $(FLAGS) $(STKDIR)Stk.cpp

# mcd-api - x
x-buffer.o: $(XAPIDIR)x-buffer.h $(XAPIDIR)x-buffer.cpp
	$(CXX) $(FLAGS) $(XAPIDIR)x-buffer.cpp
x-fun.o: $(XAPIDIR)x-fun.h $(XAPIDIR)x-fun.cpp
	$(CXX) $(FLAGS) $(XAPIDIR)x-fun.cpp
x-loadlum.o: $(XAPIDIR)x-loadlum.h $(XAPIDIR)x-loadlum.cpp
	$(CXX) $(FLAGS) $(XAPIDIR)x-loadlum.cpp
x-loadrgb.o: $(XAPIDIR)x-loadrgb.h $(XAPIDIR)x-loadrgb.cpp
	$(CXX) $(FLAGS) $(XAPIDIR)x-loadrgb.cpp
x-thread.o: $(XAPIDIR)x-thread.h $(XAPIDIR)x-thread.cpp
	$(CXX) $(FLAGS) $(XAPIDIR)x-thread.cpp
x-vector3d.o: $(XAPIDIR)x-vector3d.h $(XAPIDIR)x-vector3d.cpp
	$(CXX) $(FLAGS) $(XAPIDIR)x-vector3d.cpp
x-gfx.o: $(XAPIDIR)x-gfx.h $(XAPIDIR)x-gfx.cpp
	$(CXX) $(FLAGS) $(XAPIDIR)x-gfx.cpp
x-audio.o: $(XAPIDIR)x-audio.h $(XAPIDIR)x-audio.cpp RtAudio.o
	$(CXX) $(FLAGS) -I$(RTAUDIODIR) $(XAPIDIR)x-audio.cpp

# mcd-api - y
y-charting.o: $(YAPIDIR)y-charting.h $(YAPIDIR)y-charting.cpp
	$(CXX) $(FLAGS) -I$(XAPIDIR) $(YAPIDIR)y-charting.cpp
y-echo.o: $(YAPIDIR)y-echo.h $(YAPIDIR)y-echo.cpp
	$(CXX) $(FLAGS) -I $(STKDIR) -I$(XAPIDIR) $(YAPIDIR)y-echo.cpp
y-entity.o: $(YAPIDIR)y-entity.h $(YAPIDIR)y-entity.cpp
	$(CXX) $(FLAGS) -I$(XAPIDIR) $(YAPIDIR)y-entity.cpp
y-fft.o: $(YAPIDIR)y-fft.h $(YAPIDIR)y-fft.cpp
	$(CXX) $(FLAGS) -I$(XAPIDIR) $(YAPIDIR)y-fft.cpp

y-fluidsynth.o: fluidsynth $(YAPIDIR)y-fluidsynth.h $(YAPIDIR)y-fluidsynth.cpp
	$(CXX) $(FLAGS) -I$(XAPIDIR) -I$(YAPIDIR) -I$(FLUIDSYNTHDIR)include/ \
		$(YAPIDIR)y-fluidsynth.cpp

y-particle.o: $(YAPIDIR)y-particle.h $(YAPIDIR)y-particle.cpp
	$(CXX) $(FLAGS) -I$(XAPIDIR) $(YAPIDIR)y-particle.cpp
#y-score-reader.o: $(YAPIDIR)y-score-reader.h $(YAPIDIR)y-score-reader.cpp
	#$(CXX) $(FLAGS) $(INCLUDES) $(YAPIDIR)y-score-reader.cpp
y-waveform.o: $(YAPIDIR)y-waveform.h $(YAPIDIR)y-waveform.cpp
	$(CXX) $(FLAGS) -I$(XAPIDIR) $(YAPIDIR)y-waveform.cpp

# OSC and Networking - use makefile that comes with OSCPACK
NetworkingUtils.o:
	make -C $(OSCPACKDIR)
UdpSocket.o:
	make -C $(OSCPACKDIR)
IpEndpointName.o:
	make -C $(OSCPACKDIR)
OscOutboundPacketStream.o:
	make -C $(OSCPACKDIR)
OscPrintReceivedElements.o:
	make -C $(OSCPACKDIR)
OscReceivedElements.o:
	make -C $(OSCPACKDIR)
OscTypes.o:
	make -C $(OSCPACKDIR)

# Fluidsynth
fluidsynth:
	make -C $(FLUIDSYNTHDIR)


clean:
	rm -f *~ *# *.o swirl
