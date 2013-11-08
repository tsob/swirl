
CXX=g++
INCLUDES=

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
FLAGS=-D__LINUX_ALSASEQ__ -D__UNIX_JACK__ -c
#FLAGS=-D__LINUX_JACK__ -D__UNIX_JACK__ -c
LIBS=-lasound -lpthread -ljack -lstdc++ -lm \
    -lglut -lGL -lGLU
endif
ifeq ($(UNAME), Darwin)
FLAGS=-D__MACOSX_CORE__ -c
LIBS=-framework CoreAudio -framework CoreMIDI -framework CoreFoundation \
	-framework IOKit -framework Carbon  -framework OpenGL \
	-framework GLUT -framework Foundation -framework AppKit \
	-lstdc++ -lm
endif

OBJS=   RtAudio.o chuck_fft.o Camera.o main.o Vectors.o Pool.o  Thread.o Stk.o AirFountain.o

SonicFountain: $(OBJS)
	$(CXX) -o SonicFountain $(OBJS) $(LIBS)

main.o: main.cpp 
	$(CXX) $(FLAGS) main.cpp

Camera.o: Camera.h Camera.cpp
	$(CXX) $(FLAGS) Camera.cpp

AirFountain.o: AirFountain.h AirFountain.cpp
	$(CXX) $(FLAGS) AirFountain.cpp

Thread.o: Thread.h Thread.cpp
	$(CXX) $(FLAGS) Thread.cpp

Stk.o: Stk.h Stk.cpp
	$(CXX) $(FLAGS) Stk.cpp
    
Vectors.o: Vectors.h Vectors.cpp
	$(CXX) $(FLAGS) Vectors.cpp

Pool.o: Pool.h Pool.cpp
	$(CXX) $(FLAGS) Pool.cpp

RtAudio.o: RtAudio.h RtAudio.cpp RtError.h
		$(CXX) $(FLAGS) RtAudio.cpp

clean:
	rm -f *~ *# *.o SonicFountain
