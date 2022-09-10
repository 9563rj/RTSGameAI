#pragma once
#include "main.h"
#include "astar.h"
#include "tile.h"
struct player;
struct unit // Parallel definitions in unit.h and tile.cpp
{
	unit(player* team, const std::vector<std::vector<tile*>>& tiles, const int type, const int row, const int column, SDL_Window* window, SDL_Surface* winSurface);
	void advance(std::vector<std::vector<tile*>>& tiles);
	void navigate(std::vector<std::vector<tile*>>& tiles, std::list<unit*> &units, tile* goal, SDL_Surface* winSurface, SDL_Window* window);
	bool resourceMineFlag; // whether or not resourceMineRate amount of ms has passed since last resource mined
	int type_;
	/* Unit Types
	0 = Main Unit
	1 = Fighter
	2 = Builder
	3 = Miner
	*/
	SDL_Window* window_;
	SDL_Surface* surface_;
	tile* tileAt_;
	std::list<tile*> path_;
	player* team_;
};