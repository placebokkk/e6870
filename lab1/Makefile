
########################################################################
#   Preamble.
########################################################################

ifeq ($(OPTFLAGS),)
	OPTFLAGS = -g
endif

CXX = g++
CPPFLAGS = -I/user1/faculty/stanchen/pub/boost
CXXFLAGS = -Wall -std=c++11
LDFLAGS = $(OPTFLAGS)
LDLIBS = -lm

CXXFLAGS += $(OPTFLAGS)

#   GNU make's default rule uses $(CC) for linking
LINK.o = $(CXX) $(LDFLAGS) $(TARGET_ARCH)


########################################################################
#   Rules.
########################################################################

all : lab1_frontend lab1_dtw

clean:
	rm -f lab1_frontend lab1_dtw *.o

util.o : util.C util.H

front_end.o : front_end.C front_end.H util.H

lab1_frontend.o : lab1_frontend.C util.H front_end.H

lab1_dtw.o : lab1_dtw.C util.H

lab1_frontend : lab1_frontend.o util.o front_end.o

lab1_dtw : lab1_dtw.o util.o


########################################################################
#   
########################################################################


