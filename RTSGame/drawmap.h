#pragma once
#include "main.h"
#include "unit.h"
#include "tile.h"
struct unit;
struct player;
void drawMap(SDL_Surface* winSurface, SDL_Window* window, std::vector<std::vector<tile*>> &tiles, std::list<unit*>& units, std::vector<player*> &players);
void initMap(std::vector<std::vector<tile*>> &tiles, bool skiptarg, bool skipstart);
std::vector<int> getNode(std::vector<std::vector<tile>>& tiles, int state);
