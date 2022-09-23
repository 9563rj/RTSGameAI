#include "main.h"
#include "drawmap.h"
#include "astar.h"
#include "tile.h"
#include "unit.h"
#include "player.h"
#include "buildfactory.h"

const int tilesize = 25;
const int playerlimit = 15;

// timing intervals
const int resourceMineInterval = 500;
const int unitSpawnInterval = 10000;
const int unitMoveInterval = 75;
const int aiActInterval = 300;

// board size
int maxr;
int maxc;

bool handle_keydown(SDL_Keycode& key,
                    std::vector<player*>& players,
                    unit* currentunit,
                    std::list<unit*>& units,
                    std::vector<std::vector<tile*>>& tiles,
                    std::list<tile*>& factories,
                    SDL_Surface* winSurface, SDL_Window* window)
{
	switch (key)
	{
	case(SDLK_ESCAPE):
                std::cout << "escape key" << std::endl;
		return false;
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
		if(currentunit != NULL) currentunit->buildFactory(units, tiles, factories, 1);
		currentunit = NULL;
		break;
	}
	case(SDLK_b):
	{
		int mousex;
		int mousey;
		SDL_GetMouseState(&mousex, &mousey);
		int row = mousey / tilesize;
		int column = mousex / tilesize;
		if (currentunit != NULL) currentunit->buildFactory(units, tiles, factories, 2);
		currentunit = NULL;
		break;
	}
	case(SDLK_m):
	{
		int mousex;
		int mousey;
		SDL_GetMouseState(&mousex, &mousey);
		int row = mousey / tilesize;
		int column = mousex / tilesize;
		if (currentunit != NULL) currentunit->buildFactory(units, tiles, factories, 3);
		currentunit = NULL;
		break;
	}
	}

        return true;
}

void handle_mouseup(Uint8 button, std::vector<player*>& players,
                    unit* currentunit,
                    std::list<unit*>& units,
                    std::vector<std::vector<tile*>>& tiles,
                    SDL_Surface* winSurface, SDL_Window* window)
{
    if (button == SDL_BUTTON_LEFT)
    {
        // On left click, tell player to move to clicked tile
        int mousex;
        int mousey;
        SDL_GetMouseState(&mousex, &mousey);
        int row = mousey / tilesize;
        int column = mousex / tilesize;

        // Ignore clicks outside the board
        if (row >= tiles.size()) return;
        if (column >= tiles[0].size()) return;
        if (row < 0) return;
        if (column < 0) return;

        bool cont = true;
        for (auto unit : units)
        {
            if (unit->tileAt_->y_ == row && unit->tileAt_->x_ == column && unit->team_->human_)
            {
                currentunit = unit;
                cont = false;
            }
        }

        if (cont) // Only if currentunit is valid, then attempt to set a new path for currentunit
        {
            int targr = row;
            int targc = column;

            if (tiles[row][column]->state_ != 1 && tiles[row][column]->state_ != 3 && tiles[row][column]->magicflag == 62)
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
                currentunit->navigate(tiles, units, tiles[row][column]);
            }
            else if (tiles[row][column]->magicflag != 62)
            {
                std::cout << "attempting to read data from a nonexistent tile" << std::endl;
            }
        }
    }
    else if (button == SDL_BUTTON_RIGHT)
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
            units.push_back(new unit(players.back(), tiles, 0, row, column));
            currentunit = units.back();
        }
        else if (players.size() < playerlimit)
        {
            // std::cout << "Creating AI player" << std::endl;
            players.push_back(new player(players.size(), *winSurface, false));
            units.push_back(new unit(players.back(), tiles, 0, row, column));
        }
        else
        {
            std::cout << "Exceeded player limit, which is " << playerlimit << std::endl;
            std::system("pause");
            exit(1);
        }
        //players.back()->units_.push_back(units.back());
    }
    else if (button == SDL_BUTTON_MIDDLE)
    {
        // no-op
    }
}

int updateState(std::vector<player*>& players, unit* currentunit, std::list<unit*>& units, std::vector<std::vector<tile*>>& tiles, std::list<tile*>& factories, SDL_Surface* winSurface, SDL_Window* window)
{
	static Uint64 resourceTimer = SDL_GetTicks() % resourceMineInterval;
	static Uint64 unitSpawnTimer = SDL_GetTicks() % unitSpawnInterval;
	static Uint64 unitMoveTimer = SDL_GetTicks() % unitMoveInterval;
	static Uint64 aiActTimer = SDL_GetTicks() % aiActInterval;
        
        bool miningTimerDone = false;
        if (resourceTimer > SDL_GetTicks() % resourceMineInterval)
        {
                miningTimerDone = true;
        }
        resourceTimer = SDL_GetTicks() % resourceMineInterval;

        bool unitSpawnTimerDone = false;
        if (unitSpawnTimer > SDL_GetTicks() % unitSpawnInterval)
        {
                unitSpawnTimerDone = true;
        }
        unitSpawnTimer = SDL_GetTicks() % unitSpawnInterval;
                
        bool unitMoveTimerDone = false;
        if (unitMoveTimer > SDL_GetTicks() % unitMoveInterval)
        {
                unitMoveTimerDone = true;
        }
        unitMoveTimer = SDL_GetTicks() % unitMoveInterval;

        bool aiActTimerDone = false;
        if (aiActTimer > SDL_GetTicks() % aiActInterval)
        {
                aiActTimerDone = true;
        }
        aiActTimer = SDL_GetTicks() % aiActInterval;

        // Spawn units
        if (unitSpawnTimerDone)
        {
                for (auto factory : factories)
                {
                        factory->spawnUnit(tiles, units);
                }
        }

        // Cycle through every player, tell non-humans to perform AI actions
        if (aiActTimerDone)
        {
                for (auto playerPtr : players)
                {
                        if (!playerPtr->human_)
                        {
                                playerPtr->act(units, factories, tiles);
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
                if (deadPtr == currentunit) currentunit = NULL;
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
                                std::cout << "Player " << playerPtr->team_id_ << " with color " << playerPtr->color_ << " wins!" << std::endl;
                                return playerPtr->team_id_;
                        }
                }
        }

        return 0; // no winner
}

int initSDL(SDL_Surface*& winSurface, SDL_Window*& window)
{
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

        return 0;
}

int runMatch(std::vector<player*>& players, SDL_Surface* winSurface, SDL_Window* window)
{
	// Map init
	std::vector<std::vector<tile*>> tiles;
	initMap(tiles, false, false);

        maxr = tiles.size();
        maxc = tiles[0].size();
        printf("maxr = %d\n",maxr);
        printf("maxc = %d\n",maxc);
        
	std::list<unit*> units;

        // Each player starts with 1 unit
        double ra = rand()/double(RAND_MAX);
        double rb = rand()/double(RAND_MAX);

        int maxr0 = maxr/2;
        int minr0 = 1;

        int maxc0 = maxc/2;
        int minc0 = 1;
        
        int row0 = minr0 +ra*maxr0;
        int col0 = minc0 +rb*maxc0;
        
        int row1 = maxr-row0-1;
        int col1 = maxc-col0-1;

        printf("(%d,%d) for player 0\n",row0,col0);
        printf("(%d,%d) for player 1\n",row1,col1);
        units.push_back(new unit(players[0], tiles, 0, row0, col0));
        units.push_back(new unit(players[1], tiles, 0, row1, col1));

	// Create unit, initialize, create path variable
	
	std::vector<tile*> path;
	unit* currentunit = NULL;

	// Create list of factories, so that not every tile has to be searched for spawning loop
	std::list<tile*> factories;

	// Event loop
	bool gameRunning = true;
	SDL_Event event;

        int winner = -1;
        
	// Main game loop
	while (gameRunning)
	{
		Uint64 start = SDL_GetPerformanceCounter();

                // Process events
		while (SDL_PollEvent(&event)) {

			switch (event.type)
			{
			case(SDL_QUIT):
                                goto gameover;
			case(SDL_KEYDOWN):
                            gameRunning = handle_keydown(
                                event.key.keysym.sym, players, currentunit,
                                units, tiles, factories, winSurface, window);
                            if (!gameRunning) goto gameover;
			case(SDL_MOUSEBUTTONUP):
                            // handle_mouseup(event.button.button, players, currentunit,
                            //                units, tiles, winSurface, window);
                            break;
			}
		}

                winner = updateState(players, currentunit, units, tiles,
                                     factories, winSurface, window);

		drawMap(winSurface, window, tiles, units, players);

                if (winner) goto gameover;
                
		// FPS counter
		// Uint64 end = SDL_GetPerformanceCounter();
		// double elapsed = (end - start) / double(SDL_GetPerformanceFrequency());
		// int FPS = 1 / elapsed;
                // std::cout << "FPS is " << FPS << std::endl;
	}

gameover:
        std::cout << "Game Over" << std::endl;

        return winner;
}

int main(int argc, char** args)
{
	SDL_Surface* winSurface = NULL;
	SDL_Window* window = NULL;

        int err = initSDL(winSurface, window);
        if (err) exit(err);

        // 1 vs 1
	std::vector<player*> players;
        players.push_back(new player(0, *winSurface, false));
        players.push_back(new player(1, *winSurface, false));

        // Run tournament
        int N = 2;
        for (int n = 0; n < N; n++) {
                int winner = runMatch(players, winSurface, window);
                printf("%d wins match %d/%d\n",winner,n+1,N);
        }

	// Cleanup
	SDL_FreeSurface(winSurface);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
