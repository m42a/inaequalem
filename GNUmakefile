# This is how GLUT works in Cygwin.  Saner environments may have a smaller set of gflags
GLFLAGS=-isystem /usr/include/w32api -lopengl32 -lglu32 -lglut32 -L/usr/lib/w32api/
LGLFLAGS=-lGL -lGLU -lglut -lrt
# Flags to optimize the program
OPTFLAGS=-Wall -Wextra -std=gnu++0x -O3 -fomit-frame-pointer -march=native -flto -funsafe-loop-optimizations -Wunsafe-loop-optimizations -Winline
# Enable all the warnings, plus extra warnings
DEBUGFLAGS=-Wall -Wextra -std=gnu++0x -ggdb3

CPPFLAGS=$(OPTFLAGS)
#CPPFLAGS=$(DEBUGFLAGS)
# I don't even know why I did this, this Makefile only works with GNU make anyway
CPP=g++

OBJECTS=inaequalem.o entity.o player.o model.o parse.o ai.o
DEPENDS=$(OBJECTS:%.o=%.d)

.PHONY: clean run Linux Cygwin

inaequalem: $(OBJECTS)
	$(MAKE) `uname`

Cygwin:
	$(CPP) $(CPPFLAGS) $(OBJECTS) -o inaequalem $(GLFLAGS)

Linux:
	$(CPP) $(CPPFLAGS) $(OBJECTS) -o inaequalem $(LGLFLAGS)

# Do magic dependency magic
-include $(DEPENDS)

$(OBJECTS): %.o: %.cpp
	$(CPP) -MMD -MP -c $(CPPFLAGS) $<

clean:
	rm -f $(OBJECTS) $(DEPENDS) inaequalem *~

run: inaequalem
	./inaequalem data.dat
