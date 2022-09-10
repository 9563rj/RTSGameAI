#include "drawmap.h"
#include "player.h"

void drawMap(SDL_Surface* winSurface, SDL_Window* window, std::vector<std::vector<tile*>> &tiles, std::list<unit*>& units, std::vector<player*>& players)
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
			/* Debug, render openclosed
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
			*/

			// Render units
			// Optimization note: This searches every single unit for every single tile (redundant)
			// Optimize by having each tile know whether or not there's a unit on it, and checking that and the corresponding unit
			for (auto unit : units)
			{
				if (unit->tileAt_->x_ == i && unit->tileAt_->y_ == j)
				{
					drawRect.h -= 10;
					drawRect.w -= 10;
					drawRect.x += 5;
					drawRect.y += 5;
					SDL_FillRect(winSurface, &drawRect, unit->team_->color_);
					drawRect.h = tilesize;
					drawRect.w = tilesize;
				}
			}
		}
	}
	// Start resource bars at corner of border corner tile
	drawRect.x = tilesize;
	drawRect.y = tilesize;

	// Draw resource bars
	for (int i = 0; i < players.size(); i++)
	{
		drawRect.h = 18; // 4 px border around 100x10 resource bar
		drawRect.w = 108;
		SDL_FillRect(winSurface, &drawRect, players[i]->color_);
		double resourcePercent = double(players[i]->resources_) / double(players[i]->maxResources_);
		drawRect.x += 4;
		drawRect.y += 4;
		drawRect.h = 10; // 10 px tall inner bar
		int barProgress = 100 * resourcePercent; // 100 px long
		drawRect.w = barProgress;
		SDL_FillRect(winSurface, &drawRect, SDL_MapRGB(winSurface->format, 0, 255, 0));
		drawRect.x -= 4;
		drawRect.y += 14 + 12; // 14 px down to corner of current bar, 12 px gap between bars
	}

	SDL_UpdateWindowSurface(window);
	//std::cout << "Attempted update to window surface" << std::endl;
}
void initMap(std::vector<std::vector<tile*>> &tiles, bool skiptarg, bool skipstart)
{
	tiles.clear();
	std::ifstream map("map.txt");
	std::string buffer;
	int column = 0;
	int rowIndex = 0;
	while (getline(map, buffer))
	{
		std::cout << "reading line..." << std::endl;
		std::cout << buffer << std::endl;
		std::vector<tile*> row;
		row.clear();
		for (char ch : buffer)
		{
			int state = ch - '0';
			// if (skiptarg && state == 3) { state = 0; }	deprecated
			// if (skipstart && state == 2) { state = 0; }	deprecated
			row.push_back(new tile(state, column, rowIndex));
			column++;
		}
		tiles.push_back(row);
		column = 0;
		rowIndex++;
	}

	for (int i = 0; i < tiles.size(); i++)
	{
		for (int j = 0; j < tiles[0].size(); j++)
		{
			tiles[i][j]->x_ = j;
			tiles[i][j]->y_ = i;
		}
	}
	std::cout << "read map with height " << tiles.size() << std::endl;
	std::cout << "read map with width "<< tiles[0].size() << std::endl;
}
std::vector<int> getNode(std::vector<std::vector<tile*>>& tiles, int state)
{
	for (int r = 0; r < tiles.size(); r++)
	{
		for (int c = 0; c < tiles[0].size(); c++)
		{
			if (tiles[r][c]->state_ == state)
			{
				std::vector<int> values;
				values.push_back(r);
				values.push_back(c);
				return values;
			}
		}
	}
}