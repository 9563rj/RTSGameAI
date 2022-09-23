
DBG=-g

CPPFLAGS=-I/usr/include/SDL2 $(DBG)

OBJS := astar.o buildfactory.o drawmap.o player.o tile.o unit.o utils.o

%.o: %.cpp
	g++ -c -std=c++17 $(CPPFLAGS) $*.cpp

all: $(OBJS)
	g++ $(CPPFLAGS) main.cpp $(OBJS) -lSDL2

clean:
	rm a.out utils.o unit.o tile.o player.o drawmap.o buildfactory.o astar.o
