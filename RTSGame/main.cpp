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
	std::vector<std::vector<tile>> tiles = {
		{3,0,0,0,1},
		{1,1,0,1,1},
		{0,1,1,0,0},
		{0,0,0,1,1},
		{2,1,0,0,0}
	};
	// Debug
	std::cout << "Attempting to initialize tileset" << std::endl;
	std::system("pause");
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
	std::system("pause");
	/* Load bmpand write to tiles
	SDL_Surface* mapsurface = NULL;
	mapsurface = SDL_LoadBMP("map.bmp");
	if (mapsurface == NULL)
	{
		std::cout << "Failed to load map.bmp" << std::endl;
		std::system("pause");
	}
	else
	{
		std::cout << "Successfully loaded map.bmp" << std::endl;
		std::cout << "Format is " << SDL_PIXELTYPE(mapsurface->format->format)<< std::endl;
	}
	Uint32* pixelsvar = NULL;
	std::cout << "Debug: Attempting to init pixelsvar" << std::endl;
	std::system("pause");
	pixelsvar = (Uint32 *)mapsurface->pixels;
	if (pixelsvar == NULL)
	{
		std::cout << "Debug: Failed to init pixelsvar" << std::endl;
		std::system("pause");
	}
	else
	{
		std::cout << "Debug: Successfully init pixelsvar" << std::endl;
		std::system("pause");
	}
	std::vector<std::vector<Uint32>> mapcolors;
	std::cout << "Debug: Code block 1 starting" << std::endl;
	std::system("pause");
	for (int i = 0; i < mapsurface->h; i++)
	{
		std::vector<Uint32> row;
		row.clear();
		for (int j = 0; j < mapsurface->w; j++)
		{
			row.push_back(pixelsvar[(i * mapsurface->w) + j]);
		}
		mapcolors.push_back(row);
	}
	std::cout << "Debug: Code block 2 starting" << std::endl;
	std::system("pause");
	for (int i = 0; i < mapcolors.size(); i++)
	{
		for (int j = 0; j < mapcolors[0].size(); j++)
		{
			if (mapcolors[j][i] == SDL_MapRGB(mapsurface->format, 0, 0, 0)) 
			{
				tiles[i][j] = 0; 
				std::cout << "Successfully set state 0 at x=" << j << " y=" << i << std::endl;
			}
			else if (mapcolors[j][i] == SDL_MapRGB(mapsurface->format, 255, 255, 255)) 
			{ 
				tiles[i][j] = 1; 
				std::cout << "Successfully set state 1 at x=" << j << " y=" << i << std::endl;
			}
			else if (mapcolors[j][i] == SDL_MapRGB(mapsurface->format, 0, 255, 0)) 
			{ 
				tiles[i][j] = 2; 
				std::cout << "Successfully set state 2 at x=" << j << " y=" << i << std::endl;
			}
			else if (mapcolors[j][i] == SDL_MapRGB(mapsurface->format, 255, 0, 0)) 
			{
				tiles[i][j] = 3; 
				std::cout << "Successfully set state 3 at x=" << j << " y=" << i << std::endl;
			}
			else 
			{ 
				std::cout << "Error reading map.bmp at x=" << j << " and y=" << i << std::endl;
				std::cout << "Actual value was " << mapcolors[j][i] << std::endl;
				std::cout << "Expected value was probably " << SDL_MapRGB(mapsurface->format, 255, 255, 255) << std::endl;
				std::system("pause");
			}
			
		}
	}
	*/
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
	std::system("pause");
	// Cleanup
	SDL_FreeSurface(winSurface);
	//SDL_FreeSurface(mapsurface); 
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}