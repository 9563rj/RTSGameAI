
DBG=-g

CPPFLAGS=-I/usr/include/SDL2 $(DBG)

all:
	g++ $(CPPFLAGS) main.cpp utils.o unit.o tile.o player.o drawmap.o buildfactory.o astar.o -lSDL2

rebuild:
	g++ -c $(CPPFLAGS) astar.cpp
	g++ -c $(CPPFLAGS) buildfactory.cpp
	g++ -c $(CPPFLAGS) drawmap.cpp
	g++ -std=c++17 -c $(CPPFLAGS) player.cpp
	g++ -c $(CPPFLAGS) tile.cpp
	g++ -c $(CPPFLAGS) unit.cpp
	g++ -c $(CPPFLAGS) utils.cpp

clean:
	rm a.out utils.o unit.o tile.o player.o drawmap.o buildfactory.o astar.o
