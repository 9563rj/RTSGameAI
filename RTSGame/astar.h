#pragma once
#include "main.h"
struct tile;
struct unit;
std::vector<tile*> astar(SDL_Surface* winSurface, SDL_Window* window, std::vector<std::vector<tile*>>& tiles, std::list<unit*> &units, tile* start, tile* finish);