GLFLAGS=-isystem /usr/include/w32api -lopengl32 -lglu32 -lglut32 -L/usr/lib/w32api/
CPPFLAGS=-Wall -Wextra -O2 -std=gnu++0x
CPP=g++

OBJECTS=touhou.o enemy.o bullet.o
DEPENDS=$(OBJECTS:%.o=%.d)

.PHONY: clean

nottouhou: $(OBJECTS)
	$(CPP) $(CPPFLAGS) $(OBJECTS) -o nottouhou $(GLFLAGS)

-include $(DEPENDS)

$(OBJECTS): %.o: %.cpp
	$(CPP) -MMD -MP -c $(CPPFLAGS) $< $(GLFLAGS)
#$(CPP) -MMD -MP $(CPPFLAGS)

clean:
	rm -f $(OBJECTS) $(DEPENDS) nottouhou *~

