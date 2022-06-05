#include "main.h"
#include "drawmap.h"
#include "astar.h"
#include "tile.h"

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
	initMap(tiles, false);
	std::cout << "map has height " << tiles.size() << std::endl;
	std::cout << "map has width" << tiles[0].size() << std::endl;
	for (auto row : tiles) {
		for (auto tile : row) {
			printf("(%d,%d)", tile->y_, tile->x_);
		}
	}

	drawMap(winSurface, window, tiles);
	// Debug
	std::cout << "Attempting to initialize tileset" << std::endl;

	// Debug
	std::cout << "Attempted to initalize tileset" << std::endl;
	// Draw initialized map
	drawMap(winSurface, window, tiles);
	// Debug pause
	std::cout << "Attempted to draw init map" << std::endl;
	// Run astar
	std::vector<tile*> path = astar(winSurface, window, tiles);
	for (int i = 0; i < path.size(); i++)
	{
		path[i]->state_ = 5;
	}
	drawMap(winSurface, window, tiles);

	// Event loop
	bool gameRunning = true;
	SDL_Event event;
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
				int mousex;
				int mousey;
				SDL_GetMouseState(&mousex, &mousey);
				int row = mousey / tilesize;
				int column = mousex / tilesize;
				std::cout << "setting new goal to r=" << row << " and c=" << column << std::endl;
				initMap(tiles, true);
				tiles[row][column]->state_ = 3;
				path.clear();
				path = astar(winSurface, window, tiles);
				for (int i = 0; i < path.size(); i++)
				{
					path[i]->state_ = 5;
				}
		}
		drawMap(winSurface, window, tiles);
	}
	// Cleanup
	SDL_FreeSurface(winSurface);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}