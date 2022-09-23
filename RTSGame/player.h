#include "main.h"
struct unit;
struct tile;
enum Strategy { rng, turtle, balanced, aggro };

struct player // Parallel definitions in unit.cpp, tile.cpp, player.h
{
	bool human_;
        int team_id_;
	player(int team, SDL_Surface &winSurface, bool human);
	Uint32 teamColor(int team, SDL_Surface &winSurface);
	Strategy strat_;
	void act(std::list<unit*>& units, std::list<tile*>& factories, std::vector<std::vector<tile*>>& tiles, SDL_Surface* winSurface, SDL_Window* window);
	int resources_;
	int maxResources_;
	Uint32 color_;
	std::list<unit*> units_;
};
