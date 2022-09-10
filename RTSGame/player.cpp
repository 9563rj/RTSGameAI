#include "player.h"
#include "tile.h"
#include "unit.h"
#include "buildfactory.h"
#include <random>


player::player(int team, SDL_Surface& winSurface, bool human)
{
	resources_ = 0;
	maxResources_ = 100;
	strat_ = random;
	human_ = human;
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

void player::act(std::list<unit*>& units, std::list<tile*>& factories, std::vector<std::vector<tile*>>& tiles, SDL_Surface* winSurface, SDL_Window* window)
{
	switch (strat_) 
	{
	case(random):
		double r = rand() / double(RAND_MAX);
		if (r < .8)
		{
			//std::cout << "Decided to move, picker was " << r << std::endl;
			double unitPicker = rand() / double(RAND_MAX);
			int unitIndex = units_.size() * unitPicker;
			std::list<unit*>::iterator it = units_.begin();
			std::advance(it, unitIndex);
			double rowPicker = rand() / double(RAND_MAX);
			double columnPicker = rand() / double(RAND_MAX);
			int row = rowPicker * tiles.size();
			int column = columnPicker * tiles[0].size();
			if (it == units_.end())
			{
				//std::cout << "Player with pointer " << this << " attempted to move a unit, but has no units left" << std::endl;
			}
			else if ((*it)->type_ == 3)
			{
				std::list<tile*> resourceTiles;
				for (std::vector<std::vector<tile*>>::iterator rowit = tiles.begin(); rowit != tiles.end(); rowit++)
				{
					for (std::vector<tile*>::iterator columnit = rowit->begin(); columnit != rowit->end(); columnit++)
					{
						if ((*columnit)->state_ == 2 && (*columnit)->unitAt_ == NULL)
						{
							resourceTiles.push_back(*columnit);
						}
					}
				}
				if (resourceTiles.size() > 0)
				{
					//std::cout << resourceTiles.size() << " valid resources found" << std::endl;
					std::vector<int> pathCosts;
					std::vector<std::vector<tile*>> paths;
					for (std::list<tile*>::iterator resourceit = resourceTiles.begin(); resourceit != resourceTiles.end(); resourceit++)
					{
						std::vector<tile*> path = astar(winSurface, window, tiles, units, (*it)->tileAt_, *resourceit);
						if (path.size() == 0)
						{
							//std::cout << "Invalid AI miner path: empty" << std::endl;
						}
						else
						{
							paths.push_back(path);
							pathCosts.push_back(path.size());
						}
					}
					std::vector<int>::iterator lowestPathCostIt = min_element(pathCosts.begin(), pathCosts.end());
					int pathIndex = lowestPathCostIt - pathCosts.begin();
					//std::cout << "There are " << pathCosts.size() << " path costs." << std::endl;
					//std::cout << "There are " << paths.size() << " paths." << std::endl;
					//std::cout << "Attempting to access index " << pathIndex << std::endl;
 					if (pathCosts.size() > 0) (*it)->navigate(tiles, units, paths[pathIndex].back(), winSurface, window); // Must be at least one valid path to navigate 
				}
			}
			else (*it)->navigate(tiles, units, tiles[row][column], winSurface, window);
		}
		else if (.8 <= r <= 1)
		{
			//std::cout << "Decided to build, picker was " << r << std::endl;
			for (auto unitPtr : units_)
			{
				if (unitPtr->type_ == 0 || unitPtr->type_ == 2)
				{
					double factoryTypePicker = rand() / double(RAND_MAX);
					int factoryTypeSelector = factoryTypePicker * 3 + 1;
					//std::cout << "Attempting to build factory of type " << factoryTypeSelector << " at " << unitPtr->tileAt_->x_ << ", " << unitPtr->tileAt_->y_ << std::endl;
					buildFactory(unitPtr->tileAt_->y_, unitPtr->tileAt_->x_, units, tiles, factories, unitPtr, winSurface, window, factoryTypeSelector);
					break;
				}
				
			}
		}
		else
		{
			std::cout << "Impossible selector value while trying to execute AI action" << std::endl;
			std::system("pause");
			exit(1);
		}
		break;
	}
}

