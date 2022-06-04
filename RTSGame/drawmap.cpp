#include "drawmap.h"

void drawMap(SDL_Surface* winSurface, SDL_Window* window, std::vector<std::vector<tile>> &tiles)
{
	SDL_Rect drawRect;
	drawRect.h = tilesize;
	drawRect.w = tilesize;
	for (int i = 0; i < tiles[0].size(); i++)
	{
		for (int j = 0; j < tiles.size(); j++)
		{
			//Debug
			std::cout << "Attempting to draw tile x=" << i << " y=" << j << std::endl;
			drawRect.x = i * tilesize;
			drawRect.y = j * tilesize;
			SDL_FillRect(winSurface, &drawRect, tiles[j][i].getColor(*winSurface));
			if (tiles[j][i].openclosed == 0)
			{
				drawRect.h -= 10;
				drawRect.w -= 10;
				drawRect.x += 5;
				drawRect.y += 5;
				SDL_FillRect(winSurface, &drawRect, SDL_MapRGB(winSurface->format, 0, 255, 255));
				drawRect.h = tilesize;
				drawRect.w = tilesize;
			}
			if (tiles[j][i].openclosed == 1)
			{
				drawRect.h -= 10;
				drawRect.w -= 10;
				drawRect.x += 5;
				drawRect.y += 5;
				SDL_FillRect(winSurface, &drawRect, SDL_MapRGB(winSurface->format, 255, 255, 0));
				drawRect.h = tilesize;
				drawRect.w = tilesize;
			}
		}
	}
	SDL_UpdateWindowSurface(window);
	std::cout << "Attempted update to window surface" << std::endl;
	std::system("pause");
}