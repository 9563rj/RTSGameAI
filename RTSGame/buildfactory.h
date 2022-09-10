#pragma once
#include "main.h"
struct unit;
struct player;
struct tile;
void buildFactory(int row, int column, std::list<unit*>& units, std::vector<std::vector<tile*>>& tiles, std::list<tile*>& factories, unit* currentunit, SDL_Surface* winSurface, SDL_Window* window, int factoryTypeSelector);