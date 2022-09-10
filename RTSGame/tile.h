#pragma once
#include "main.h"
struct player;
struct unit;
struct tile
{
	tile(const int& state);
	int state_;
	/*States
	0 = Open
	1 = Wall
	2 = Resource
	3 = Factory
	*/
	Uint32 getColor(SDL_Surface& winSurface);
	int distTo(tile* dest);
	tile* parent_;
	int openclosed;
	bool onpath;
	int x_;
	int y_;
	int f_;
	int g_;
	int h_;
	player* claimedBy_;
};