#include "player.h"

player::player(int team, unit* commander, SDL_Surface &winSurface)
{
	energy_ = 0;
	commander_ = commander;
	switch (team)
	{
	case(0):
		color_ = SDL_MapRGB(winSurface.format, 0, 0, 200);
		break;
	case(1):
		color_ = SDL_MapRGB(winSurface.format, 200, 0, 0);
		break;
	default:
		std::cout << "Attempted to set player to unknown team" << std::endl;
		std::system("pause");
		exit(1);
	}
}