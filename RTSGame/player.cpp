#include "player.h"

Uint32 player::teamColor(int team, SDL_Surface &winSurface)
{
	// Decide what color this new team should be
	// Five different colors, specific shade decided by team%5

	int five = 5;
	int index = team / five;
	int subindex = team % five;

	switch (subindex)
	{
	case(0):
		return(SDL_MapRGB(winSurface.format, 0, 0, 240 - index * 80));
		break;
	case(1):
		return(SDL_MapRGB(winSurface.format, 240 - index * 80, 0, 0));
		break;
	case(2):
		return(SDL_MapRGB(winSurface.format, 240 - index * 80, 0, 240 - index * 80));
		break;
	case(3):
		return(SDL_MapRGB(winSurface.format, 240 - index * 80, 240 - index * 80, 0));
		break;
	case(4):
		return(SDL_MapRGB(winSurface.format, 240 - index * 80, 240 - index * 80, 240 - index * 80));
		break;
	}
	/*int teamModulo = team;
	for (int i = 0; teamModulo > 0 && i < 4; i++)
	{
		if (1 <= teamModulo && teamModulo <= 3)
		{
			switch (teamModulo)-6
			{
			case(0):
				return(SDL_MapRGB(winSurface.format, 0, 0, 200 - i * 50));
				break;
			case(1):
				return(SDL_MapRGB(winSurface.format, 200 - i * 50, 0, 0));
				break;
			case(2):
				return(SDL_MapRGB(winSurface.format, 200 - i * 50, 0, 200 - i * 50));
				break;
			}
		}
	}
	// This for loop should never be passed, valid paths call return()
	std::cout << "New team color for loop exceeded" << std::endl;
	std::system("pause");
	exit(1);*/
}

player::player(int team, SDL_Surface &winSurface)
{
	resources_ = 0;
	maxResources_ = 100;
	/*switch (team)
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
	}*/
	color_ = teamColor(team, winSurface);
}