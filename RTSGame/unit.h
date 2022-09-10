#pragma once
#include "main.h"
#include "astar.h"
struct tile;
struct player;
struct unit 
{
	unit(player* team, const std::vector<std::vector<tile*>>& tiles, const int type, const int row, const int column, SDL_Window* window, SDL_Surface* winSurface);
	void advance(std::vector<std::vector<tile*>>& tiles);
	void navigate(std::vector<std::vector<tile*>>& tiles, std::list<unit*> &units, tile* goal, SDL_Surface* winSurface, SDL_Window* window);
	bool resourceMineFlag; // whether or not resourceMineRate amount of ms has passed since last resource mined
	bool unitMoveFlag;
	int type_;
	/* Unit Types
	0 = Main Unit
	1 = Fighter
	2 = Builder
	3 = Miner
	*/
	int health_;
	SDL_Window* window_;
	SDL_Surface* surface_;
	tile* tileAt_;
	std::list<tile*> path_;
	player* team_;
};