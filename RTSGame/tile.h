#pragma once
#include "main.h"
struct player;
struct unit;
struct tile
{
	tile(const int& state, int& x, int& y);
	int magicflag;
	int state_;
	int factoryType; // corresponds to unit types, except 0 is not a factory
	void spawnUnit(const std::vector<std::vector<tile*>>& tiles, std::list<unit*>& units, SDL_Window* window, SDL_Surface* winSurface);
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
	unit* unitAt_;
};