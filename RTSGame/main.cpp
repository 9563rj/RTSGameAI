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
	unit hero(tiles, 0, startr, startc, window, winSurface);
	units.push_back(&hero);
	// Draw initialized map
	drawMap(winSurface, window, tiles, units);

	// Event loop
	bool gameRunning = true;
	SDL_Event event;
	
	// Create unit, initialize, create path variable
	player p1(0, &hero, *winSurface);
	std::vector<player*> players;
	players.push_back(&p1);
	std::vector<tile*> path;
	unit* currentunit = &hero;
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
				}
			case(SDL_MOUSEBUTTONUP):
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					int mousex;
					int mousey;
					SDL_GetMouseState(&mousex, &mousey);
					int row = mousey / tilesize;
					int column = mousex / tilesize;
					bool cont = true;
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
							std::cout << "setting new goal to r=" << row << " and c=" << column << std::endl;
							// tiles[row][column]->state_ = 3;
							currentunit->navigate(tiles, tiles[row][column], winSurface, window);
						}
					}
				}
				else if (event.button.button == SDL_BUTTON_RIGHT)
				{
					int mousex;
					int mousey;
					SDL_GetMouseState(&mousex, &mousey);
					int row = mousey / tilesize;
					int column = mousex / tilesize;
					units.push_back(new unit(tiles, 0, row, column, window, winSurface));
					players.push_back(new player(players.size(), units.back(), *winSurface));
				}
				break;
		}
		for(auto unit : units)
		{
			unit->advance(tiles);
		}
		drawMap(winSurface, window, tiles, units);
	}
	// Cleanup
	SDL_FreeSurface(winSurface);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}