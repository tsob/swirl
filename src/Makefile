# Makefile for:
#
#       Swirl
#   The Sonic World
#
# Reza Payami, Tim O'Brien, Haley Sayres
# Fall 2013

CXX=g++
INCLUDES=
UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
FLAGS=-D__LINUX_ALSASEQ__ -D__UNIX_JACK__ -c -g
LIBS=-lasound -lpthread -ljack -lstdc++ -lm \
    -lglut -lGL -lGLU
endif
ifeq ($(UNAME), Darwin)
FLAGS=-D__MACOSX_CORE__ -c
LIBS=-framework CoreAudio -framework CoreMIDI -framework CoreFoundation \
	-framework IOKit -framework Carbon  -framework opengl \
	-framework GLUT -framework Foundation \
	-framework AppKit -lstdc++ -lm
endif

OBJS=

swirl: $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LIBS) $(INCLUDES)

clean:
	rm -f *~ *# *.o swirl
