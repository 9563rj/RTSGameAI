#include "main.h"
struct unit;
struct player // Any time this is changed, update parallel definition in unit.cpp
{
	player(int team, SDL_Surface &winSurface);
	Uint32 teamColor(int team, SDL_Surface &winSurface);
	int resources_;
	Uint32 color_;
	unit* commander_;
	std::list<unit*> units_;
};