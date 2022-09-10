#include "main.h"
#include "drawmap.h"
#include "astar.h"
#include "tile.h"
#include "unit.h"
#include "player.h"

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
	std::cout << "map has height " << tiles.size() << std::endl;
	std::cout << "map has width " << tiles[0].size() << std::endl;
	for (auto row : tiles) {
		for (auto tile : row) {
			printf("(%d,%d)", tile->y_, tile->x_);
		}
	}
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

	int playerlimit = 15;

	// Main game loop
	while (gameRunning)
	{
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
						int mousex;
						int mousey;
						SDL_GetMouseState(&mousex, &mousey);
						int row = mousey / tilesize;
						int column = mousex / tilesize;
						for (auto unitPtr : units)
						{
							// If main unit, also spawn Fighter, Builder, Miner
							if (unitPtr->type_ == 0)
							{
								if (unitPtr->tileAt_ == tiles[row][column] && (unitPtr->type_ == 0 || unitPtr->type_ == 2) && unitPtr->tileAt_->state_ == 0)
								{
									if (currentunit == unitPtr) currentunit = NULL;

									// Set this tile to be a factory tile and add it to the list of factory tiles
									unitPtr->tileAt_->claimedBy_ = unitPtr->team_;
									unitPtr->tileAt_->state_ = 3;
									unitPtr->tileAt_->factoryType = 1;
									factories.push_back(unitPtr->tileAt_);

									// Create fighter, builder, and miner and add to relevant lists
									units.push_back(new unit(unitPtr->team_, tiles, 1, row - 1, column, window, winSurface));
									unitPtr->team_->units_.push_back(units.back());
									units.push_back(new unit(unitPtr->team_, tiles, 2, row, column + 1, window, winSurface));
									unitPtr->team_->units_.push_back(units.back());
									units.push_back(new unit(unitPtr->team_, tiles, 3, row + 1, column, window, winSurface));
									unitPtr->team_->units_.push_back(units.back());

									// Erase from list of units held by this unit's team
									std::list<unit*>& teamUnitList = unitPtr->team_->units_;
									teamUnitList.erase(std::find(teamUnitList.begin(), teamUnitList.end(), unitPtr));

									// Erase from global list of units
									units.erase(std::find(units.begin(), units.end(), unitPtr));

									delete unitPtr;
								}
							}
							else if (unitPtr->type_ == 2)
							{
								if (unitPtr->tileAt_ == tiles[row][column] && (unitPtr->type_ == 0 || unitPtr->type_ == 2) && unitPtr->tileAt_->state_ == 0)
								{
									if (currentunit == unitPtr) currentunit = NULL;

									// Set this tile to be a factory tile and add it to the list of factory tiles
									unitPtr->tileAt_->claimedBy_ = unitPtr->team_;
									unitPtr->tileAt_->state_ = 3;
									unitPtr->tileAt_->factoryType = 1;
									factories.push_back(unitPtr->tileAt_);

									// Erase from list of units held by this unit's team
									std::list<unit*>& teamUnitList = unitPtr->team_->units_;
									teamUnitList.erase(std::find(teamUnitList.begin(), teamUnitList.end(), unitPtr));

									// Erase from global list of units
									units.erase(std::find(units.begin(), units.end(), unitPtr));

									delete unitPtr;
								}
							}
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
						if (unit->tileAt_->y_ == row && unit->tileAt_->x_ == column)
						{
							currentunit = unit;
							cont = false;
						}
					}
					if (cont)
					{
						targr = row;
						targc = column;
						if (tiles[row][column]->state_ != 1)
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
					if (players.size() < playerlimit)
					{
						players.push_back(new player(players.size(), *winSurface));
						units.push_back(new unit(players.back(), tiles, 0, row, column, window, winSurface));
					}
					else
					{
						std::cout << "Exceeded player limit, which is " << playerlimit << std::endl;
						std::system("pause");
					}
					players.back()->commander_ = units.back();
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

		for(auto unit : units)
		{
			if (miningTimerDone)
			{
					unit->resourceMineFlag = true;
			}
			unit->advance(tiles);
		}

		drawMap(winSurface, window, tiles, units, players);
	}
	// Cleanup
	SDL_FreeSurface(winSurface);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}