
########################################################################
#   Preamble.
########################################################################

ifeq ($(OPTFLAGS),)
	OPTFLAGS = -g
endif

CXX = g++
CPPFLAGS = -I/user1/faculty/stanchen/pub/boost
CXXFLAGS = -Wall
LDFLAGS = $(OPTFLAGS)
LDLIBS = -lm

CXXFLAGS += $(OPTFLAGS)

#   GNU make's default rule uses $(CC) for linking
LINK.o = $(CXX) $(LDFLAGS) $(TARGET_ARCH)


########################################################################
#   Rules.
########################################################################

all : lab4_vit

clean:
	rm -f lab4_vit front_end.o lab4_vit.o util.o Lab4Vit.class

util.o : util.C util.H

front_end.o : ../../lab1/front_end.C ../../lab1/front_end.H util.H

main.o : main.C

lab4_vit.o : lab4_vit.C lab4_vit.H util.H front_end.H

lab4_vit : lab4_vit.o util.o front_end.o main.o


########################################################################
#   
########################################################################


