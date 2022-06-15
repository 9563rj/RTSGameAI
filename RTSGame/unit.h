#pragma once
#include "main.h"
#include "astar.h"
#include "tile.h"

struct unit
{
	unit(const std::vector<std::vector<tile*>>& tiles, const int type, const int row, const int column, SDL_Window* window, SDL_Surface* winSurface);
	void advance(std::vector<std::vector<tile*>>& tiles);
	void navigate(std::vector<std::vector<tile*>> &tiles, tile* goal, SDL_Surface* winSurface, SDL_Window* window);
	int type_;
	SDL_Window* window_;
	SDL_Surface* surface_;
	tile* tileAt_;
	std::list<tile*> path_;
};