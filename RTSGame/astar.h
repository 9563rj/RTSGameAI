#pragma once
#include "main.h"
#include "tile.h"
#include "drawmap.h"
#include <cassert>
std::vector<tile*> astar(SDL_Surface* winSurface, SDL_Window* window, std::vector<std::vector<tile*>>& tiles, tile* start, tile* finish);