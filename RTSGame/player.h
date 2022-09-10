#include "main.h"
struct unit;
struct player // Parallel definitions in unit.cpp, tile.cpp, player.h
{
	player(int team, SDL_Surface &winSurface);
	Uint32 teamColor(int team, SDL_Surface &winSurface);
	int resources_;
	int maxResources_;
	Uint32 color_;
	unit* commander_;
	std::list<unit*> units_;
};