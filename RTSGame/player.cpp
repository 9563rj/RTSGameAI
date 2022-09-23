#include "player.h"
#include "tile.h"
#include "unit.h"
#include "buildfactory.h"
#include "main.h"
#include <random>
#include <algorithm>

auto gen = std::mt19937{ std::random_device{}() };

player::player(int team, SDL_Surface& winSurface, bool human)
{
	resources_ = 0;
	maxResources_ = 100;
	strat_ = rng;
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

enum moveTypes {moveFighter, moveBuilder, buildFactory, moveMiner};

void player::act(std::list<unit*>& units, std::list<tile*>& factories, std::vector<std::vector<tile*>>& tiles, SDL_Surface* winSurface, SDL_Window* window)
{
	switch (strat_) 
	{
	case(rng):
		double r = rand() / double(RAND_MAX);
		// Possible moves are:
		// Move fighter (randomly): if this team has a fighter, if there is a valid destination
		// Move builder (randomly): if this team has a builder, if there is a valid destination
		// Builder builds factory: if there are not more factories than 2x active miners and valid builder to build a factory
		// Miner moves to resource: if there is a valid miner and a valid resource
		std::list<unit*> fighters;
		std::list<unit*> builders;
		std::list<unit*> miners;
		std::list<unit*> activeMiners;
		std::list<tile*> openTiles;
		std::list<tile*> openResources;
		std::list<tile*> teamFactories;
		for (auto unitPtr : units_)
		{
			if (unitPtr->type_ == 1) fighters.push_back(unitPtr);
			if (unitPtr->type_ == 2) builders.push_back(unitPtr);
			if (unitPtr->type_ == 3) miners.push_back(unitPtr);
			if (unitPtr->type_ == 3 && unitPtr->tileAt_->state_ == 2) activeMiners.push_back(unitPtr);
		}
		for (std::vector<std::vector<tile*>>::iterator tilesRowIt = tiles.begin(); tilesRowIt != tiles.end(); tilesRowIt++)
		{
			for (std::vector<tile*>::iterator tilesColIt = (*tilesRowIt).begin(); tilesColIt != (*tilesRowIt).end(); tilesColIt++)
			{
				if (((*tilesColIt)->state_ == 0 || (*tilesColIt)->state_ == 2) && (*tilesColIt)->unitAt_ == NULL) openTiles.push_back(*tilesColIt);
				if ((*tilesColIt)->claimedBy_ == this) teamFactories.push_back(*tilesColIt);
				if ((*tilesColIt)->state_ == 2 && (*tilesColIt)->unitAt_ == NULL) openResources.push_back(*tilesColIt);
			}
		}

		bool canMoveFighter = fighters.size() > 0 && openTiles.size() > 0;
		bool canMoveBuilder = builders.size() > 0 && openTiles.size() > 0;
		bool canBuildFactory = builders.size() > 0 && (teamFactories.size() * 2) < activeMiners.size();
		bool canMoveMiner = miners.size() > 0 && miners.size() > activeMiners.size() && openResources.size() > 0;

		int numValidMoves = 0;
		if (canMoveFighter) numValidMoves++;
		if (canMoveBuilder) numValidMoves++;
		if (canBuildFactory) numValidMoves++;
		if (canMoveMiner) numValidMoves++;
		std::uniform_int_distribution<> factoryTypeDistrib(1, 3);

PICK_A_MOVE:
		std::uniform_int_distribution<> distrib(0, 3);
		int movePicker = distrib(gen);
		switch (movePicker) 
		{
		case(moveFighter):
			if (canMoveFighter)
			{
				std::list<unit*> pickedFighter;
				std::list<tile*> pickedTile;
				std::sample(fighters.begin(), fighters.end(), std::back_inserter(pickedFighter), 1, gen);
				std::sample(openTiles.begin(), openTiles.end(), std::back_inserter(pickedTile), 1, gen);
				if(pickedFighter.size() > 0 && pickedTile.size() > 0) pickedFighter.back()->navigate(tiles, units, pickedTile.back(), winSurface, window);
				if (pickedFighter.size() == 0) std::cout << "Could not pick a fighter when trying to move fighter" << std::endl;
				if (pickedTile.size() == 0) std::cout << "Could not pick a tile when trying to move fighter" << std::endl;
			}
			else goto PICK_A_MOVE;
			break;
		case(moveBuilder):
			if (canMoveBuilder)
			{
				std::list<unit*> pickedBuilder;
				std::list<tile*> pickedTile;
				std::sample(builders.begin(), builders.end(), std::back_inserter(pickedBuilder), 1, gen);
				std::sample(openTiles.begin(), openTiles.end(), std::back_inserter(pickedTile), 1, gen);
				if(pickedBuilder.size() > 0 && pickedTile.size() > 0) pickedBuilder.back()->navigate(tiles, units, pickedTile.back(), winSurface, window);
				if (pickedBuilder.size() == 0) std::cout << "Could not pick a builder while trying to move builder" << std::endl;
				if (pickedTile.size() == 0) std::cout << "Could not pick a tile while trying to move builder" << std::endl;
			}
			else goto PICK_A_MOVE;
			break;
		case(buildFactory):
			if (canBuildFactory)
			{
				std::list<unit*> pickedBuilder;
				std::sample(builders.begin(), builders.end(), std::back_inserter(pickedBuilder), 1, gen);
				int factoryType = factoryTypeDistrib(gen);
				if(pickedBuilder.size()>0) pickedBuilder.back()->buildFactory(units, tiles, factories, winSurface, window, factoryType);
				if (pickedBuilder.size() == 0) std::cout << "Could not pick a builder while trying to build factory" << std::endl;
			}
			else goto PICK_A_MOVE;
			break;
		case(moveMiner):
			if (canMoveMiner)
			{
				std::list<unit*> pickedMiner;
				std::list<tile*> pickedOpenResource;
				std::sample(miners.begin(), miners.end(), std::back_inserter(pickedMiner), 1, gen);
				std::sample(openResources.begin(), openResources.end(), std::back_inserter(pickedOpenResource), 1, gen);
				if(pickedMiner.size() > 0 && pickedOpenResource.size() > 0) pickedMiner.back()->navigate(tiles, units, pickedOpenResource.back(), winSurface, window);
				if (pickedMiner.size() == 0) std::cout << "Could not pick a miner while trying to move miner" << std::endl;
				if (pickedOpenResource.size() == 0) std::cout << "Could not pick an open resource while trying to move miner" << std::endl;
			}
		}
		if (units_.size() == 1)
		{
			unit* unitPtr = units_.back();
			if (unitPtr->type_ == 0) unitPtr->buildFactory(units, tiles, factories, winSurface, window, 2);
		}

		/*
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
				if ((*it)->tileAt_->state_ != 2)
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
				// otherwise don't move
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
					unitPtr->buildFactory(units, tiles, factories, winSurface, window, factoryTypeSelector);
					break;
				}
				
			}
		}
		else
		{
			std::cout << "Impossible selector value while trying to execute AI action" << std::endl;
			std::system("pause");
			exit(1);
		}*/
		break;
	}
}

