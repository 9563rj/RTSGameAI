#pragma once
#include "main.h"
struct tile
{
	tile(const int& state);
	int state_;
	tile* parent_;
	Uint32 getColor(SDL_Surface& winSurface);
	int distTo(tile* dest);
	int openclosed;
	bool onpath;
	int x_;
	int y_;
	int f_;
	int g_;
	int h_;
};