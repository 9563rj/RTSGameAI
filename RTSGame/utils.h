#pragma once
#include "main.h"
struct tile;
struct player;
struct unit;
bool isLegal(int r, int c, const std::vector<std::vector<tile*>>& tiles);