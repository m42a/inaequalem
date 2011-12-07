# Flags to optimize the program
DEFCFLAGS:=-Wall -Wextra -std=gnu++0x -O3 -fomit-frame-pointer -march=native -flto -funsafe-loop-optimizations -Wunsafe-loop-optimizations -Winline
# Enable debugging without optimizations
#DEFCFLAGS:=-Wall -Wextra -std=gnu++0x -ggdb3

UNAME:=$(strip $(shell uname))

ifeq ($(UNAME),Linux)
   CFLAGS:=$(DEFCFLAGS)
   LFLAGS:=-lGL -lGLU -lglut -lrt
endif
ifeq ($(UNAME),CYGWIN_NT-6.1-WOW64)
   CFLAGS:=$(DEFCFLAGS) -isystem /usr/include/w32api
   LFLAGS:=-lopengl32 -lglu32 -lglut32 -L/usr/lib/w32api/
endif

# I don't even know why I did this, this Makefile only works with GNU make anyway
CPP:=g++

OBJECTS:=inaequalem.o entity.o player.o model.o parse.o ai.o texture.o
DEPENDS:=$(OBJECTS:%.o=%.d)
OBJMODELS:=asteroid.obj enemy1.obj player.obj
OBJMATS:=$(OBJMODELS:%.obj=%.mtl)
FILTMODELS:=$(OBJMODELS:%.obj=%.filtobj)

.PHONY: clean run

inaequalem: $(OBJECTS)
	$(CPP) $(CFLAGS) $(OBJECTS) -o inaequalem $(LFLAGS)

# Do magic dependency magic
-include $(DEPENDS)

$(OBJECTS): %.o: %.cpp
	$(CPP) -MMD -MP -c $(CFLAGS) $<

$(FILTMODELS): %.filtobj: %.obj %.mtl
	./filter.sh $*.obj $*.mtl $@

centerscale : centerscale.cpp
	$(CPP) $(CFLAGS) $<

clean:
	rm -f $(OBJECTS) $(DEPENDS) $(FILTMODELS) inaequalem centerscale *~

run: inaequalem $(FILTMODELS)
	./inaequalem data.dat
