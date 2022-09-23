#pragma once
#include "main.h"
#include "astar.h"
struct tile;
struct player;
struct unit 
{
	unit(player* team, const std::vector<std::vector<tile*>>& tiles, const int type, const int row, const int column);
	void advance(std::vector<std::vector<tile*>>& tiles);
	void navigate(std::vector<std::vector<tile*>>& tiles, std::list<unit*> &units, tile* goal);
	void buildFactory(std::list<unit*>& units, std::vector<std::vector<tile*>>& tiles, std::list<tile*>& factories, int factoryTypeSelector);
	bool resourceMineFlag; // whether or not resourceMineRate amount of ms has passed since last resource mined
	bool unitMoveFlag;
	int type_;
	/* Unit Types
	0 = Main Unit
	1 = Fighter
	2 = Builder
	3 = Miner
	*/
	int health_;
	tile* tileAt_;
	std::list<tile*> path_;
	player* team_;
        bool on_mission_;
};
