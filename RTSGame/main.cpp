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
	std::ifstream map("map.txt");
	std::string buffer;
	std::vector<std::vector<tile>> tiles;
	while (getline(map, buffer))
	{
		std::cout << buffer << std::endl;
		std::vector<tile> row;
		row.clear();
		for (char ch : buffer)
		{
			int state = ch - '0';
			row.push_back(tile(state));
		}
		tiles.push_back(row);
	}
	drawMap(winSurface, window, tiles);
	// Debug
	std::cout << "Attempting to initialize tileset" << std::endl;
	for (int i = 0; i < tiles.size(); i++)
	{
		for (int j = 0; j < tiles[0].size(); j++)
		{
			tiles[i][j].x_ = j;
			tiles[i][j].y_ = i;
		}
	}
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
	std::cout << "Found path" << std::endl;

	// Event loop
	bool gameRunning = true;
	SDL_Event event;
	while (gameRunning)
	{
		SDL_PollEvent(&event);
		SDL_Delay(50);
		drawMap(winSurface, window, tiles);
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
		}
	}
	// Cleanup
	SDL_FreeSurface(winSurface);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}