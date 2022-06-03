#include "main.h"
#include "tile.h"

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
	window = SDL_CreateWindow("Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, winx, winy, SDL_WINDOW_SHOWN);
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
	std::vector<std::vector<tile>> tiles = { {0,1,2},{0,0,0},{3,1,0} };
	SDL_Rect drawRect;
	drawRect.h = tilesize;
	drawRect.w = tilesize;
	// Draw init map
	for (int j = 0; j<tiles.size(); j++)
	{
		for (int i = 0; i<tiles.size(); i++)
		{
			drawRect.x = i * tilesize;
			drawRect.y = j * tilesize;
			SDL_FillRect(winSurface, &drawRect, tiles[i][j].getColor(*winSurface));
		}
	}
	SDL_UpdateWindowSurface(window);

	// Main game loop
	bool gameRunning = true;
	while (gameRunning)
	{
		gameRunning = false;
		SDL_UpdateWindowSurface(window);
		std::system("pause");
	}

	// Cleanup
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}