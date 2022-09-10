#include "tile.h"

struct player // Parallel definitions in unit.cpp, tile.cpp, player.h
{
	player(int team, SDL_Surface& winSurface);
	Uint32 teamColor(int team, SDL_Surface& winSurface);
	int resources_;
	int maxResources_;
	Uint32 color_;
	unit* commander_;
	std::list<unit*> units_;
};

struct unit // Parallel definitions in unit.h and tile.cpp
{
	unit(player* team, const std::vector<std::vector<tile*>>& tiles, const int type, const int row, const int column, SDL_Window* window, SDL_Surface* winSurface);
	void advance(std::vector<std::vector<tile*>>& tiles);
	void navigate(std::vector<std::vector<tile*>>& tiles, std::list<unit*>& units, tile* goal, SDL_Surface* winSurface, SDL_Window* window);
	bool resourceMineFlag; // whether or not resourceMineRate amount of ms has passed since last resource mined
	int type_;
	/* Unit Types
	0 = Main Unit
	1 = Fighter
	2 = Builder
	3 = Miner
	*/
	void claimTile();
	SDL_Window* window_;
	SDL_Surface* surface_;
	tile* tileAt_;
	std::list<tile*> path_;
	player* team_;
};

tile::tile(const int& state)
{
	state_ = state;
	openclosed = 2;
	onpath = false;
	claimedBy_ = NULL;
}
/*States
0 = Open
1 = Wall
2 = Resource
3 = Factory
*/

int tile::distTo(tile* dest)
{
	int numHoriMoves;
	int numVertMoves;
	int numLinearMoves;
	int numDiagMoves;
	if (x_ < dest->x_) { numHoriMoves = dest->x_ - x_; }
	else { numHoriMoves = x_ - dest->x_; }
	if (y_ < dest->y_) { numVertMoves = dest->y_ - y_; }
	else { numVertMoves = y_ - dest->y_; }
	if (numHoriMoves < numVertMoves)
	{
		numLinearMoves = numVertMoves - numHoriMoves;
		numDiagMoves = numVertMoves - numLinearMoves;
	}
	else
	{
		numLinearMoves = numHoriMoves - numVertMoves;
		numDiagMoves = numHoriMoves - numLinearMoves;
	}
	return numLinearMoves * 10 + numDiagMoves * 14;
}

Uint32 tile::getColor(SDL_Surface& winSurface)
{
	// if (onpath) return SDL_MapRGB(winSurface.format, 0, 0, 255);
	switch(state_)
	{
		case(0):
			// Empty
			return SDL_MapRGB(winSurface.format, 50, 50, 50);
			break;
		case(1):
			// Wall
			return SDL_MapRGB(winSurface.format, 255, 255, 255);
			break;
		case(2):
			// Resource
			return SDL_MapRGB(winSurface.format, 0, 255, 0);
			break;
		case(3):
			// Factory
			return claimedBy_->color_;
			break;
		default:
			std::cout << "Unknown state. State is " << state_ << std::endl;
			break;
	}
}