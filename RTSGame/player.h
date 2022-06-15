#include "main.h"
#include "unit.h"

struct player
{
	player(int team, unit* commander, SDL_Surface &winSurface);
	int energy_;
	Uint32 color_;
	unit* commander_;
};