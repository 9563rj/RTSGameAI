#include "main.h"
struct unit;
struct tile;
enum Strategy { rng, rwa, turtle, balanced, aggro };

struct player // Parallel definitions in unit.cpp, tile.cpp, player.h
{
	bool human_;
        int team_id_;
	player(int team, SDL_Surface &winSurface, bool human);
	Uint32 teamColor(int team, SDL_Surface &winSurface);
	Strategy strat_;
	void act(std::list<unit*>& units, std::list<tile*>& factories, std::vector<std::vector<tile*>>& tiles);
	int resources_;
	int maxResources_;
	Uint32 color_;
	std::list<unit*> units_;

private:
	void rwa_AI(std::list<unit*>& units, std::list<tile*>& factories, std::vector<std::vector<tile*>>& tiles);
        
};
