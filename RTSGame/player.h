#include "main.h"
struct unit;
struct player
{
	player(int team, SDL_Surface &winSurface);
	Uint32 teamColor(int team, SDL_Surface &winSurface);
	int energy_;
	Uint32 color_;
	unit* commander_;
	std::list<unit*> units_;
};