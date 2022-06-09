#include "drawmap.h"

void drawMap(SDL_Surface* winSurface, SDL_Window* window, std::vector<std::vector<tile*>> &tiles)
{
	SDL_Rect drawRect;
	drawRect.h = tilesize;
	drawRect.w = tilesize;
	for (int i = 0; i < tiles[0].size(); i++)
	{
		for (int j = 0; j < tiles.size(); j++)
		{
			//Debug
			//std::cout << "Attempting to draw tile x=" << i << " y=" << j << std::endl;
			drawRect.x = i * tilesize;
			drawRect.y = j * tilesize;
			SDL_FillRect(winSurface, &drawRect, tiles[j][i]->getColor(*winSurface));
			if (tiles[j][i]->openclosed == 0)
			{
				drawRect.h -= 10;
				drawRect.w -= 10;
				drawRect.x += 5;
				drawRect.y += 5;
				SDL_FillRect(winSurface, &drawRect, SDL_MapRGB(winSurface->format, 0, 255, 255));
				drawRect.h = tilesize;
				drawRect.w = tilesize;
			}
			if (tiles[j][i]->openclosed == 1)
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
	//std::cout << "Attempted update to window surface" << std::endl;
}
void initMap(std::vector<std::vector<tile*>> &tiles, bool skiptarg, bool skipstart)
{
	tiles.clear();
	std::ifstream map("map.txt");
	std::string buffer;
	while (getline(map, buffer))
	{
		std::cout << buffer << std::endl;
		std::vector<tile*> row;
		row.clear();
		for (char ch : buffer)
		{
			int state = ch - '0';
			if (skiptarg && state == 3) { state = 0; }
			if (skipstart && state == 2) { state = 0; }
			row.push_back(new tile(state));
		}
		tiles.push_back(row);
	}

	for (int i = 0; i < tiles.size(); i++)
	{
		for (int j = 0; j < tiles[0].size(); j++)
		{
			tiles[i][j]->x_ = j;
			tiles[i][j]->y_ = i;
		}
	}
	std::cout << "read map with height" << tiles.size() << std::endl;
	std::cout << "read map with width "<< std::endl;
}