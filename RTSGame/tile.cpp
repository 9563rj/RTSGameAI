#include "tile.h"

tile::tile(const int& state)
{
	state_ = state;
	openclosed = 2;
	onpath = false;
}
/*States
0 = Open
1 = Wall
2 = Resource
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
		default:
			std::cout << "Unknown state. State is " << state_ << std::endl;
			break;
	}
}