
DBG=-g

CPPFLAGS=-I/usr/include/SDL2 $(DBG)

OBJS := astar.o buildfactory.o drawmap.o player.o tile.o unit.o utils.o

all: $(OBJS)
	g++ $(CPPFLAGS) main.cpp $(OBJS) -lSDL2

-include $(OBJS:.o=.d)

%.o: %.cpp
	g++ -c -std=c++17 $(CPPFLAGS) $*.cpp
	g++ -MM $(CFLAGS) $*.cpp > $*.d

clean:
	rm a.out $(OBJS) *.d

