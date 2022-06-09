#pragma once
#include "main.h"
#include "tile.h"
void drawMap(SDL_Surface* winSurface, SDL_Window* window, std::vector<std::vector<tile*>> &tiles);
void initMap(std::vector<std::vector<tile*>> &tiles, bool skiptarg, bool skipstart);