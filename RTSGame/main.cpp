#include "main.h"
#include "drawmap.h"
#include "astar.h"
#include "tile.h"
#include "unit.h"
#include "player.h"
#include "buildfactory.h"

const int tilesize = 25;



int main(int argc, char** args)
{
	SDL_Surface* winSurface = NULL;
	SDL_Window* window = NULL;

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cout << "Error initializing SDL. " << SDL_GetError() << std::endl;
		std::system("pause");
		return 1;
	}
	int winx = 1600;
	int winy = 900;
	window = SDL_CreateWindow("RTSGame", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, winx, winy, SDL_WINDOW_SHOWN);
	if (!window) 
	{
		std::cout << "Error creating window. " << SDL_GetError() << std::endl;
		std::system("pause");
		return 1;
	}

	winSurface = SDL_GetWindowSurface(window);
	if (!winSurface)
	{
		std::cout << "Error getting surface. " << SDL_GetError() << std::endl;
		std::system("pause");
		return 1;
	}

	// Map init
	std::vector<std::vector<tile*>> tiles;
	initMap(tiles, false, false);
	// std::cout << "map has height " << tiles.size() << std::endl;
	// std::cout << "map has width " << tiles[0].size() << std::endl;
	/*for (auto row : tiles) {
		for (auto tile : row) {
			printf("(%d,%d)", tile->y_, tile->x_);
		}
	}*/
	// init units up here to draw
	int targr;
	int targc;
	int startr;
	int startc;
	for (int r = 0; r < tiles.size(); r++)
	{
		for (int c = 0; c < tiles[0].size(); c++)
		{
			if (tiles[r][c]->state_ == 3)
			{
				targr = r;
				targc = c;
			}
			if (tiles[r][c]->state_ == 2)
			{
				startr = r;
				startc = c;
			}
		}
	}
	std::list<unit*> units;
	// player p1(0, *winSurface);
	std::vector<player*> players;
	// players.push_back(&p1);
	// unit hero(players.back(), tiles, 0, startr, startc, window, winSurface);
	// units.push_back(&hero);
	// Draw initialized map
	// drawMap(winSurface, window, tiles, units, players);

	// Event loop
	bool gameRunning = true;
	SDL_Event event;
	
	// Create unit, initialize, create path variable
	
	std::vector<tile*> path;
	unit* currentunit = NULL;

	// Create list of factories, so that not every tile has to be searched for spawning loop
	std::list<tile*> factories;

	// Initialize resource timer
	int resourceMineInterval = 500;
	Uint64 resourceTimer = SDL_GetTicks64() % resourceMineInterval;

	// Initializing spawning timer
	int unitSpawnInterval = 10000;
	Uint64 unitSpawnTimer = SDL_GetTicks64() % unitSpawnInterval;

	// Init move timer
	int unitMoveInterval = 75;
	Uint64 unitMoveTimer = SDL_GetTicks64() % unitMoveInterval;

	// Init AI acting timer
	int aiActInterval = 1000;
	Uint64 aiActTimer = SDL_GetTicks64() % aiActInterval;

	int playerlimit = 15;

	// Main game loop
	while (gameRunning)
	{
		Uint64 start = SDL_GetPerformanceCounter();
		SDL_PollEvent(&event);
		switch (event.type)
		{
			case(SDL_QUIT):
				gameRunning = false;
				break;
			case(SDL_KEYDOWN):
				switch (event.key.keysym.sym)
				{
					case(SDLK_ESCAPE):
						gameRunning = false;
						break;
					case(SDLK_r):
						for (int i = 0; i < players.size(); i++)
						{
							std::cout << "Player " << i << " has " << players[i]->resources_ << " resources." << std::endl;
						}
						break;
					case(SDLK_f):
					{
						int mousex;
						int mousey;
						SDL_GetMouseState(&mousex, &mousey);
						int row = mousey / tilesize;
						int column = mousex / tilesize;
						if(currentunit != NULL) currentunit->buildFactory(units, tiles, factories, winSurface, window, 1);
						break;
					}
					case(SDLK_b):
					{
						int mousex;
						int mousey;
						SDL_GetMouseState(&mousex, &mousey);
						int row = mousey / tilesize;
						int column = mousex / tilesize;
						if (currentunit != NULL) currentunit->buildFactory(units, tiles, factories, winSurface, window, 2);
						break;
					}
					case(SDLK_m):
					{
						int mousex;
						int mousey;
						SDL_GetMouseState(&mousex, &mousey);
						int row = mousey / tilesize;
						int column = mousex / tilesize;
						if (currentunit != NULL) currentunit->buildFactory(units, tiles, factories, winSurface, window, 3);
						break;
					}
				}
			case(SDL_MOUSEBUTTONUP):
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					// On left click, tell player to move to clicked tile
					int mousex;
					int mousey;
					SDL_GetMouseState(&mousex, &mousey);
					int row = mousey / tilesize;
					int column = mousex / tilesize;
					// Ignore clicks outside the board
					if (row >= tiles.size()) break;
					if (column >= tiles[0].size()) break;
					if (row < 0) break;
					if (column < 0) break;
					bool cont = true;
				if (currentunit == NULL) 
					{
						/*int mousex; deprecated, we don't want a new player on left click
						int mousey;
						SDL_GetMouseState(&mousex, &mousey);
						int row = mousey / tilesize;
						int column = mousex / tilesize;
						players.push_back(new player(players.size(), *winSurface));
						units.push_back(new unit(players.back(), tiles, 0, row, column, window, winSurface));
						players.back()->commander_ = units.back();
						players.back()->units_.push_back(units.back());*/
						cont = false;
					}
					for (auto unit : units)
					{
						if (unit->tileAt_->y_ == row && unit->tileAt_->x_ == column && unit->team_->human_)
						{
							currentunit = unit;
							cont = false;
						}
					}
					if (cont)
					{
						targr = row;
						targc = column;
						if (tiles[row][column]->state_ != 1 && tiles[row][column]->state_ != 3)
						{
							for (int r = 0; r < tiles.size(); r++)
							{
								for (int c = 0; c < tiles[0].size(); c++)
								{
									tiles[r][c]->onpath = false;
								}
							}
							// std::cout << "setting new goal to r=" << row << " and c=" << column << std::endl;
							// tiles[row][column]->state_ = 3;
							currentunit->navigate(tiles, units, tiles[row][column], winSurface, window);
						}
					}
				}
				else if (event.button.button == SDL_BUTTON_RIGHT)
				{
					// On right click, create new player at tile
					int mousex;
					int mousey;
					SDL_GetMouseState(&mousex, &mousey);
					int row = mousey / tilesize;
					int column = mousex / tilesize;
					if (players.size() == 0)
					{
						// std::cout << "Creating human player" << std::endl;
						players.push_back(new player(players.size(), *winSurface, true));
						units.push_back(new unit(players.back(), tiles, 0, row, column, window, winSurface));
						currentunit = units.back();
					}
					else if (players.size() < playerlimit)
					{
						// std::cout << "Creating AI player" << std::endl;
						players.push_back(new player(players.size(), *winSurface, false));
						units.push_back(new unit(players.back(), tiles, 0, row, column, window, winSurface));
					}
					else
					{
						std::cout << "Exceeded player limit, which is " << playerlimit << std::endl;
						std::system("pause");
					}
					players.back()->units_.push_back(units.back());
				}
				else if (event.button.button == SDL_BUTTON_MIDDLE)
				{
					
				}
				break;
		}

		// done separate from searching every unit, so every unit only has to be searched once
		bool miningTimerDone = false;
		if (resourceTimer > SDL_GetTicks64() % resourceMineInterval)
		{
			miningTimerDone = true;
			resourceTimer = SDL_GetTicks64() % resourceMineInterval;
		}
		else
		{
			resourceTimer = SDL_GetTicks64() % resourceMineInterval;
		}

		// spawning timer flag
		bool unitSpawnTimerDone = false;
		if (unitSpawnTimer > SDL_GetTicks64() % unitSpawnInterval)
		{
			unitSpawnTimerDone = true;
			unitSpawnTimer = SDL_GetTicks64() % unitSpawnInterval;
		}
		else
		{
			unitSpawnTimer = SDL_GetTicks64() % unitSpawnInterval;
		}

		for (auto factory : factories)
		{
			if (unitSpawnTimerDone)
			{
				factory->spawnUnit(tiles, units, window, winSurface);
			}
		}

		bool unitMoveTimerDone = false;
		if (unitMoveTimer > SDL_GetTicks64() % unitMoveInterval)
		{
			unitMoveTimerDone = true;
			unitMoveTimer = SDL_GetTicks64() % unitMoveInterval;
		}
		else
		{
			unitMoveTimer = SDL_GetTicks64() % unitMoveInterval;
		}

		bool aiActTimerDone = false;
		if (aiActTimer > SDL_GetTicks64() % aiActInterval)
		{
			aiActTimerDone = true;
			aiActTimer = SDL_GetTicks64() % aiActInterval;
		}
		else
		{
			aiActTimer = SDL_GetTicks64() % aiActInterval;
		}

		// Cycle through every player, tell non-humans to perform AI actions
		if (aiActTimerDone)
		{
			for (auto playerPtr : players)
			{
				if (!playerPtr->human_)
				{
					playerPtr->act(units, factories, tiles, winSurface, window);
				}
			}
		}

		// Cycle through every unit, compute combat, mining, and moving
		std::list<unit*> deadUnits;
		std::list<tile*> deadFactories;
		for(auto unitPtr : units)
		{
			if (unitMoveTimerDone) unitPtr->unitMoveFlag = true;
			if (unitPtr->type_ == 1)
			{
				for (auto targetPtr : units)
				{
					int targetX = targetPtr->tileAt_->x_;
					int targetY = targetPtr->tileAt_->y_;
					int selfX = unitPtr->tileAt_->x_;
					int selfY = unitPtr->tileAt_->y_;
					
					bool above = (targetX == selfX && targetY == selfY - 1);
					bool left = (targetX == selfX - 1 && targetY == selfY);
					bool right = (targetX == selfX + 1 && targetY == selfY);
					bool below = (targetX == selfX && targetY == selfY + 1);
					if ((above || left || right || below) && (targetPtr->team_ != unitPtr->team_))
					{
						targetPtr->health_ -= 1;
						bool alreadyDeadCheck = deadUnits.end() == std::find(deadUnits.begin(), deadUnits.end(), targetPtr);
						if (targetPtr->health_ < 1 && alreadyDeadCheck) deadUnits.push_back(targetPtr);
					}
				}
				//for (auto targetPtr : factories)
				for(std::list<tile*>::iterator it = factories.begin(); it != factories.end(); it++)
				{
					tile* targetPtr = *it;
					int targetX = targetPtr->x_;
					int targetY = targetPtr->y_;
					int selfX = unitPtr->tileAt_->x_;
					int selfY = unitPtr->tileAt_->y_;

					bool above = (targetX == selfX && targetY == selfY - 1);
					bool left = (targetX == selfX - 1 && targetY == selfY);
					bool right = (targetX == selfX + 1 && targetY == selfY);
					bool below = (targetX == selfX && targetY == selfY + 1);
					if ((above || left || right || below) && (targetPtr->claimedBy_ != unitPtr->team_))
					{
						bool alreadyDeadCheck = deadFactories.end() == std::find(deadFactories.begin(), deadFactories.end(), targetPtr);
						if (alreadyDeadCheck)
						{
							targetPtr->claimedBy_ = NULL;
							targetPtr->factoryType = 0;
							targetPtr->state_ = 0;
							it = factories.erase(std::find(factories.begin(), factories.end(), targetPtr));
							if (it == factories.end()) break;
						}
					}
				}
			}
			if (miningTimerDone) unitPtr->resourceMineFlag = true;
			unitPtr->advance(tiles);
		}

		// Kill units that died during this frame, avoids modifying actively iterated lists
		for (auto deadPtr : deadUnits)
		{
			player* deadPtrTeam = deadPtr->team_;
			std::list<unit*>::iterator deadUnit = std::find(deadPtrTeam->units_.begin(), deadPtrTeam->units_.end(), deadPtr);
			// check if dead unit is in its team's unit list
			if (deadUnit != deadPtrTeam->units_.end())
			{
				//std::cout << "Dead unit found in units list. Pointer is " << deadPtr << std::endl;
				deadPtrTeam->units_.erase(deadUnit);
			}
			else std::cout << "Dead unit not found in team's units list. Pointer is " << deadPtr << std::endl;


			units.erase(std::find(units.begin(), units.end(), deadPtr));
			deadPtr->tileAt_->unitAt_ = NULL; // "corpse" removed from tile
			delete deadPtr;
		}


		// Win conditions: if player has no factories or units, it is a dead player, and if only one player left and has units and factories, that player wins
		std::list<player*> deadPlayers;
		for (auto playerPtr : players)
		{
			bool hasNoFactories = true;
			for (auto factoryPtr : factories)
			{
				if (factoryPtr->claimedBy_ == playerPtr) hasNoFactories = false;
			}
			if (playerPtr->units_.size() == 0 && hasNoFactories)
			{
				deadPlayers.push_back(playerPtr);
			}
		}
		for (auto playerPtr : deadPlayers)
		{
			players.erase(std::find(players.begin(), players.end(), playerPtr));
			delete playerPtr;
		}
		if (players.size() == 1)
		{
			for (auto playerPtr : players)
			{
				bool hasNoFactories = true;
				for (auto factoryPtr : factories)
				{
					if (factoryPtr->claimedBy_ == playerPtr) hasNoFactories = false;
				}
				if (playerPtr->units_.size() != 0 && !hasNoFactories)
				{
					std::cout << "Player with color " << playerPtr->color_ << " wins!" << std::endl;
					std::system("pause");
					gameRunning = false;
				}
			}
		}

		drawMap(winSurface, window, tiles, units, players);

		// FPS counter
		Uint64 end = SDL_GetPerformanceCounter();
		double elapsed = (end - start) / double(SDL_GetPerformanceFrequency());
		int FPS = 1 / elapsed;
		// std::cout << "FPS is " << FPS << std::endl;
	}
	// Cleanup
	SDL_FreeSurface(winSurface);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}