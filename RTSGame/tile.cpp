#include "unit.h"
#include "tile.h"
#include "player.h"
#include "utils.h"

tile::tile(const int& state, int& x, int& y)
{
	state_ = state;
	magicflag = 62;
	openclosed = 2;
	onpath = false;
	claimedBy_ = NULL;
	unitAt_ = NULL;
	factoryType = 0;
	x_ = x;
	y_ = y;
}
void tile::spawnUnit(const std::vector<std::vector<tile*>>& tiles, std::list<unit*>& units)
{
	if (claimedBy_->resources_ > 9)
	{
		bool validSpawnUp = true;
		bool validSpawnLeft = true;
		bool validSpawnRight = true;
		bool validSpawnDown = true;

		// Make sure factories are never directly against map border past boundary walls!
		tile* aboveTile = tiles[y_ - 1][x_];
		tile* leftTile = tiles[y_][x_ - 1];
		tile* rightTile = tiles[y_][x_ + 1];
		tile* belowTile = tiles[y_ + 1][x_];

		if (aboveTile->unitAt_ != NULL || aboveTile->state_ == 1) validSpawnUp = false;
		if (leftTile->unitAt_ != NULL || leftTile->state_ == 1) validSpawnLeft = false;
		if (rightTile->unitAt_ != NULL || rightTile->state_ == 1) validSpawnRight = false;
		if (belowTile->unitAt_ != NULL || belowTile->state_ == 1) validSpawnDown = false;

		if (validSpawnUp)
		{
			claimedBy_->resources_ -= 10;
			units.push_back(new unit(claimedBy_, tiles, factoryType, y_ - 1, x_));
			//claimedBy_->units_.push_back(units.back());
		}
		else if (validSpawnLeft)
		{
			claimedBy_->resources_ -= 10;
			units.push_back(new unit(claimedBy_, tiles, factoryType, y_, x_ - 1));
			//claimedBy_->units_.push_back(units.back());
		}
		else if (validSpawnRight)
		{
			claimedBy_->resources_ -= 10;
			units.push_back(new unit(claimedBy_, tiles, factoryType, y_, x_ + 1));
			//claimedBy_->units_.push_back(units.back());
		}
		else if (validSpawnDown)
		{
			claimedBy_->resources_ -= 10;
			units.push_back(new unit(claimedBy_, tiles, factoryType, y_ + 1, x_));
			//claimedBy_->units_.push_back(units.back());
		}
		else
		{
			//std::cout << "Warning: factory at " << x_ << ", " << y_ << " is surrounded, no unit produced this cycle" << std::endl;
		}
		
	}
	else
	{
		//std::cout << "Not enough resources to produce a unit from factory at " << x_ << ", " << y_ << std::endl;
	}
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

