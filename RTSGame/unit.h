#pragma once
#include "main.h"
#include "tile.h"
#include "drawmap.h"

struct unit
{
	unit(const std::vector<std::vector<tile*>>& tiles, const int type, const int row, const int column, SDL_Window* window, SDL_Surface* winSurface);
	void advance(std::vector<std::vector<tile*>>& tiles, std::vector<tile*>& path);
	int type_;
	SDL_Window* window_;
	SDL_Surface* surface_;
	tile* tileAt_;
};