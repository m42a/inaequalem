GLFLAGS=-isystem /usr/include/w32api -lopengl32 -lglu32 -lglut32 -L/usr/lib/w32api/
CPPFLAGS=-Wall -Wextra -O2 -std=gnu++0x
CPP=g++

OBJECTS=inaequalem.o enemy.o bullet.o player.o
DEPENDS=$(OBJECTS:%.o=%.d)

.PHONY: clean

inaequalem: $(OBJECTS)
	$(CPP) $(CPPFLAGS) $(OBJECTS) -o inaequalem $(GLFLAGS)

-include $(DEPENDS)

$(OBJECTS): %.o: %.cpp
	$(CPP) -MMD -MP -c $(CPPFLAGS) $< $(GLFLAGS)

clean:
	rm -f $(OBJECTS) $(DEPENDS) inaequalem *~

