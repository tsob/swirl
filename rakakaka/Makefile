
RTAUDIODIR=api/rtaudio/
STKDIR=api/stk/
XAPIDIR=api/x-api/
YAPIDIR=api/y-api/
IPDIR=api/ip/
OSCDIR=api/osc/

CXX=g++
INCLUDES=-Imain/ -Icore/ -I$(RTAUDIODIR) -I$(STKDIR) -I$(XAPIDIR) -I$(YAPIDIR)\
	 -I$(IPDIR) -I$(OSCDIR)

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
FLAGS=-pthread -D__LINUX_ALSASEQ__ -D__UNIX_JACK__ -c -g
#FLAGS=-D__LINUX_JACK__ -D__UNIX_JACK__ -c
LIBS=-lasound -lpthread -ljack -lstdc++ -lm \
    -lglut -lGL -lGLU
endif
ifeq ($(UNAME), Darwin)
FLAGS=-D__MACOSX_CORE__ -c -g
LIBS=-framework CoreAudio -framework CoreMIDI -framework CoreFoundation \
     -framework IOKit -framework Carbon  -framework OpenGL -framework Cocoa \
     -framework GLUT -framework Foundation -framework AppKit \
     -lstdc++ -lm
endif


OBJS= main.o raka-audio.o raka-entity.o raka-gfx.o raka-globals.o raka-sim.o \
      RtAudio.o Delay.o DelayL.o Stk.o x-buffer.o x-fun.o \
      x-gfx.o x-loadlum.o x-loadrgb.o x-thread.o x-vector3d.o y-charting.o \
      y-echo.o y-entity.o y-fft.o y-particle.o y-score-reader.o y-waveform.o \
      NetworkingUtils.o UdpSocket.o IpEndpointName.o

Rakakaka: $(OBJS)
	$(CXX) -o Rakakaka $(OBJS) $(LIBS)

#main
main.o: main/main.cpp
	$(CXX) $(FLAGS) $(INCLUDES) main/main.cpp

#core
raka-audio.o: core/raka-audio.h core/raka-audio.cpp
	$(CXX) $(FLAGS) $(INCLUDES) core/raka-audio.cpp
raka-entity.o: core/raka-entity.h core/raka-entity.cpp
	$(CXX) $(FLAGS) $(INCLUDES) core/raka-entity.cpp
raka-gfx.o: core/raka-gfx.h core/raka-gfx.cpp
	$(CXX) $(FLAGS) $(INCLUDES) core/raka-gfx.cpp
raka-globals.o: core/raka-globals.h core/raka-globals.cpp
	$(CXX) $(FLAGS) $(INCLUDES) core/raka-globals.cpp
raka-sim.o: core/raka-sim.h core/raka-sim.cpp
	$(CXX) $(FLAGS) $(INCLUDES) core/raka-sim.cpp

RtAudio.o: $(RTAUDIODIR)RtAudio.h $(RTAUDIODIR)RtAudio.cpp
	$(CXX) $(FLAGS) $(INCLUDES) $(RTAUDIODIR)RtAudio.cpp

Delay.o: $(STKDIR)Delay.h $(STKDIR)Delay.cpp
	$(CXX) $(FLAGS) $(INCLUDES) $(STKDIR)Delay.cpp
DelayL.o: $(STKDIR)DelayL.h $(STKDIR)DelayL.cpp
	$(CXX) $(FLAGS) $(INCLUDES) $(STKDIR)DelayL.cpp
#Filter.o: Filter.h Filter.cpp
#	$(CXX) $(FLAGS) $(INCLUDES) Filter.cpp
#MidiFileIn.o: MidiFileIn.h MidiFileIn.cpp
	#$(CXX) $(FLAGS) $(INCLUDES) MidiFileIn.cpp
Stk.o: $(STKDIR)Stk.h $(STKDIR)Stk.cpp
	$(CXX) $(FLAGS) $(INCLUDES) $(STKDIR)Stk.cpp

#api
x-buffer.o: $(XAPIDIR)x-buffer.h $(XAPIDIR)x-buffer.cpp
	$(CXX) $(FLAGS) $(INCLUDES) $(XAPIDIR)x-buffer.cpp
x-fun.o: $(XAPIDIR)x-fun.h $(XAPIDIR)x-fun.cpp
	$(CXX) $(FLAGS) $(INCLUDES) $(XAPIDIR)x-fun.cpp
x-loadlum.o: $(XAPIDIR)x-loadlum.h $(XAPIDIR)x-loadlum.cpp
	$(CXX) $(FLAGS) $(INCLUDES) $(XAPIDIR)x-loadlum.cpp
x-loadrgb.o: $(XAPIDIR)x-loadrgb.h $(XAPIDIR)x-loadrgb.cpp
	$(CXX) $(FLAGS) $(INCLUDES) $(XAPIDIR)x-loadrgb.cpp
x-thread.o: $(XAPIDIR)x-thread.h $(XAPIDIR)x-thread.cpp
	$(CXX) $(FLAGS) $(INCLUDES) $(XAPIDIR)x-thread.cpp
x-vector3d.o: $(XAPIDIR)x-vector3d.h $(XAPIDIR)x-vector3d.cpp
	$(CXX) $(FLAGS) $(INCLUDES) $(XAPIDIR)x-vector3d.cpp
#FUCK APPLE PROPRIETARY SHIT
ifeq ($(UNAME), Linux)
x-gfx.o: $(XAPIDIR)x-gfx.h $(XAPIDIR)x-gfx.mm
	$(CXX) `gnustep-config --objc-flags` `gnustep-config --gui-libs` \
		-std=c++0x $(FLAGS) -lgnustep-base \
		$(INCLUDES) $(XAPIDIR)x-gfx.mm
endif
ifeq ($(UNAME), Darwin)
x-gfx.o: $(XAPIDIR)x-gfx.h $(XAPIDIR)x-gfx.mm
	$(CXX) $(FLAGS) $(INCLUDES) $(XAPIDIR)x-gfx.mm
endif

y-charting.o: $(YAPIDIR)y-charting.h $(YAPIDIR)y-charting.cpp
	$(CXX) $(FLAGS) $(INCLUDES) $(YAPIDIR)y-charting.cpp
y-echo.o: $(YAPIDIR)y-echo.h $(YAPIDIR)y-echo.cpp
	$(CXX) $(FLAGS) $(INCLUDES) $(YAPIDIR)y-echo.cpp
y-entity.o: $(YAPIDIR)y-entity.h $(YAPIDIR)y-entity.cpp
	$(CXX) $(FLAGS) $(INCLUDES) $(YAPIDIR)y-entity.cpp
y-fft.o: $(YAPIDIR)y-fft.h $(YAPIDIR)y-fft.cpp
	$(CXX) $(FLAGS) $(INCLUDES) $(YAPIDIR)y-fft.cpp
#y-fluidsynth.o: y-fluidsynth.h y-fluidsynth.cpp
#  $(CXX) $(FLAGS) $(INCLUDES) y-fluidsynth.cpp
y-particle.o: $(YAPIDIR)y-particle.h $(YAPIDIR)y-particle.cpp
	$(CXX) $(FLAGS) $(INCLUDES) $(YAPIDIR)y-particle.cpp
y-score-reader.o: $(YAPIDIR)y-score-reader.h $(YAPIDIR)y-score-reader.cpp
	$(CXX) $(FLAGS) $(INCLUDES) $(YAPIDIR)y-score-reader.cpp
y-waveform.o: $(YAPIDIR)y-waveform.h $(YAPIDIR)y-waveform.cpp
	$(CXX) $(FLAGS) $(INCLUDES) $(YAPIDIR)y-waveform.cpp


NetworkingUtils.o: $(IPDIR)NetworkingUtils.h
	$(CXX) $(FLAGS) $(INCLUDES) $(IPDIR)posix/NetworkingUtils.cpp
UdpSocket.o: $(IPDIR)UdpSocket.h
	$(CXX) $(FLAGS) $(INCLUDES) $(IPDIR)posix/UdpSocket.cpp
IpEndpointName.o: $(IPDIR)IpEndpointName.h
	$(CXX) $(FLAGS) $(INCLUDES) $(IPDIR)IpEndpointName.cpp


clean:
	rm -f *~ *# *.o Rakakaka
