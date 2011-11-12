# This is how GLUT works in Cygwin.  Saner environments may have a smaller set of gflags
GLFLAGS=-isystem /usr/include/w32api -lopengl32 -lglu32 -lglut32 -L/usr/lib/w32api/
# Enable all the warnings, plus extra warnings
CPPFLAGS=-Wall -Wextra -Weffc++ -O2 -std=gnu++0x
# I don't even know why I did this, this Makefile only works with GNU make anyway
CPP=g++

OBJECTS=inaequalem.o enemy.o bullet.o player.o model.o
DEPENDS=$(OBJECTS:%.o=%.d)

.PHONY: clean run

inaequalem: $(OBJECTS)
	$(CPP) $(CPPFLAGS) $(OBJECTS) -o inaequalem $(GLFLAGS)

# Do magic dependency magic
-include $(DEPENDS)

$(OBJECTS): %.o: %.cpp
	$(CPP) -MMD -MP -c $(CPPFLAGS) $< $(GLFLAGS)

clean:
	rm -f $(OBJECTS) $(DEPENDS) inaequalem *~

run: inaequalem
	./inaequalem
