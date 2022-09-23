#pragma once
#include "main.h"
struct tile;
struct unit;
std::vector<tile*> astar(std::vector<std::vector<tile*>>& tiles, std::list<unit*> &units, tile* start, tile* finish);
