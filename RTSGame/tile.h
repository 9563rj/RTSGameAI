#pragma once
#include "main.h"
struct tile
{
	tile(const int& state);
	int state_;
	Uint32 getColor(SDL_Surface& winSurface);
};