#pragma once
#include "main.h"
#include "tile.h"
#include "drawmap.h"
#include "unit.h"
#include <cassert>
struct unit;
std::vector<tile*> astar(SDL_Surface* winSurface, SDL_Window* window, std::vector<std::vector<tile*>>& tiles, std::list<unit*> &units, tile* start, tile* finish);