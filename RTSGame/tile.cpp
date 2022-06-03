#include "tile.h"

tile::tile(const int& state)
{
	state_ = state;
}

Uint32 tile::getColor(SDL_Surface& winSurface)
{
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
			// Start
			return SDL_MapRGB(winSurface.format, 0, 255, 0);
			break;
		case(3):
			// End
			return SDL_MapRGB(winSurface.format, 255, 0, 0);
			break;
		default:
			std::cout << "Unknown state. State is " << state_ << std::endl;
	}
}