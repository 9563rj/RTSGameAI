#include "drawmap.h"
#include "player.h"

void drawMap(SDL_Surface* winSurface, SDL_Window* window, std::vector<std::vector<tile*>> &tiles, std::list<unit*>& units, std::vector<player*>& players)
{
        const int boardlox = 100+2*tilesize;
        const int boardloy = 0;

        // Draw tiles
	SDL_Rect drawRect;
	drawRect.h = tilesize;
	drawRect.w = tilesize;
	for (int i = 0; i < tiles[0].size(); i++)
	{
		for (int j = 0; j < tiles.size(); j++)
		{
			//Debug
			//std::cout << "Attempting to draw tile x=" << i << " y=" << j << std::endl;
			drawRect.x = boardlox +i*tilesize;
			drawRect.y = boardloy +j*tilesize;
			SDL_FillRect(winSurface, &drawRect, tiles[j][i]->getColor(*winSurface));
                        
			switch (tiles[j][i]->factoryType) 
			{
			case(0): // Main Unit Factory
				break;
			case(1): // Fighter Factory
				drawRect.h = 25;
				drawRect.w = 5;
				drawRect.x += 10;
				SDL_FillRect(winSurface, &drawRect, SDL_MapRGB(winSurface->format, 0, 255, 0));
				drawRect.h = 5;
				drawRect.w = 25;
				drawRect.x -= 10;
				drawRect.y += 10;
				SDL_FillRect(winSurface, &drawRect, SDL_MapRGB(winSurface->format, 0, 255, 0));
				break;
			case(2): // Builder Factory
				drawRect.h = 25;
				drawRect.w = 6;
				drawRect.x += 10;
				SDL_FillRect(winSurface, &drawRect, SDL_MapRGB(winSurface->format, 0, 255, 0));
				break;
			case(3): // Miner Factory
				drawRect.h = 11;
				drawRect.w = 11;
				drawRect.x += 7;
				drawRect.y += 7;
				SDL_FillRect(winSurface, &drawRect, SDL_MapRGB(winSurface->format, 0, 255, 0));
				break;
			}
			drawRect.x = i * tilesize;
			drawRect.y = j * tilesize;
			drawRect.h = tilesize;
			drawRect.w = tilesize;
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

                }
        }


        // Render units
        for (auto unitPtr : units)
        {
                int i = unitPtr->tileAt_->x_;
                int j = unitPtr->tileAt_->y_;
                drawRect.x = boardlox +i*tilesize;
                drawRect.y = boardloy +j*tilesize;
                drawRect.h -= 10;
                drawRect.w -= 10;
                drawRect.x += 5;
                drawRect.y += 5;
                SDL_FillRect(winSurface, &drawRect, unitPtr->team_->color_);
                switch (unitPtr->type_)
                {
                case(0): // Main Unit
                        break;
                case(1): // Fighter
                        drawRect.h = 15;
                        drawRect.w = 3;
                        drawRect.x += 6;
                        SDL_FillRect(winSurface, &drawRect, SDL_MapRGB(winSurface->format, 0, 255, 0));
                        drawRect.h = 3;
                        drawRect.w = 15;
                        drawRect.x -= 6;
                        drawRect.y += 6;
                        SDL_FillRect(winSurface, &drawRect, SDL_MapRGB(winSurface->format, 0, 255, 0));
                        break;
                case(2): // Builder
                        drawRect.h = 15;
                        drawRect.w = 3;
                        drawRect.x += 6;
                        SDL_FillRect(winSurface, &drawRect, SDL_MapRGB(winSurface->format, 0, 255, 0));
                case(3): // Miner
                        drawRect.h -= 8;
                        drawRect.w -= 8;
                        drawRect.x += 4;
                        drawRect.y += 4;
                        SDL_FillRect(winSurface, &drawRect, SDL_MapRGB(winSurface->format, 0, 255, 0));
                        break;
                }
                drawRect.h = tilesize;
                drawRect.w = tilesize;
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
		//std::cout << "reading line..." << std::endl;
		//std::cout << buffer << std::endl;
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
	//std::cout << "read map with height " << tiles.size() << std::endl;
	//std::cout << "read map with width "<< tiles[0].size() << std::endl;
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
