#include "astar.h"
#include "tile.h"
#include "drawmap.h"
#include "unit.h"
#include <cassert>

std::vector<tile*> astar(SDL_Surface* winSurface, SDL_Window* window, std::vector<std::vector<tile*>>& tiles, std::list<unit*>& units, tile* start, tile* finish)
{
	// Initialization
	std::list<tile*> open;
	std::list<tile*> closed;
	std::vector<tile*> empty;
	tile* current = start;
	tile* goal = finish;
	tile* begin = start;
	/*
	for (int i = 0; i < tiles.size(); i++)
	{
		for (int j = 0; j < tiles[0].size(); j++)
		{
			if (tiles[i][j]->state_ == 3) 
			{
				goal = tiles[i][j];
			}
			if (tiles[i][j]->state_ == 2)
			{
				current = tiles[i][j];
				begin = current;
			}
			tiles[i][j]->openclosed = 2;
		}
	}
	if (goal == NULL)
	{
		std::cout << "Goal is null" << std::endl;
		system("pause");
		exit(1);
	}
	if (current == NULL)
	{
		std::cout << "Current is null" << std::endl;
		system("pause");
		exit(1);
	}
	*/
	// Begin algorithm
	// calculate start point f,g,h
	current->h_=current->distTo(goal);
	current->g_ = 0;
	current->f_ = current->h_;
	open.push_back(current);
	while (open.size()>0)
	{
		// find lowest f value, init to very big
		int lowestf = INT_MAX;
		std::vector<tile*> fvalues;
		int index;
		for (auto tile:open)
		{
			tile->f_ = tile->g_ + tile->h_;
			fvalues.push_back(tile);
		}
		for (int i = 0; i < fvalues.size(); i++)
		{
			if (fvalues[i]->f_ < lowestf) 
			{ 
				lowestf = fvalues[i]->f_; 
				index = i;
			}
		}
		current = fvalues[index];


		if (current == goal) { goto GET_MOVE_SEQUENCE; }
		// generate successors
		std::list<tile*> successors;
		int maph = tiles.size();
		int mapw = tiles[0].size();

		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				if (i == 0 && j == 0) continue;

				// neighbor coords
				int ni = current->y_ + i;
				int nj = current->x_ + j;

				if (ni < 0) continue;
				if (nj < 0) continue;
				if (ni >= maph) continue;
				if (nj >= mapw) continue;
				if (tiles[ni][nj]->state_ == 1 || tiles[ni][nj]->state_ == 3) continue;
				for (auto unit : units)
				{
					if (unit->tileAt_->x_ == nj && unit->tileAt_->y_ == ni)
					{
						goto BREAK_LOOPS;
					}

				}
				successors.push_back(tiles[ni][nj]);
			BREAK_LOOPS:
				continue;
			}
		}
		// compute successor f,g,h and open/closed
		for (auto tile:successors)
		{
			int successorcurrentcost = current->g_ + tile->distTo(current);
			if (std::find(open.begin(), open.end(), tile) != open.end())
			{
				if (tile->g_ <= successorcurrentcost) { continue; }
			}
			else if (std::find(closed.begin(), closed.end(), tile) != closed.end())
			{
				if (tile->g_ <= successorcurrentcost) { continue; }
				closed.remove(tile);
				open.push_back(tile);
			}
			else 
			{
				open.push_back(tile);
				tile->h_ = tile->distTo(goal);
			}
			tile->g_ = successorcurrentcost;
			tile->parent_ = current;
		}


		// Set current tile to closed and synchronize openclosedlist with tile openclosed
		open.remove(current);
		closed.push_back(current);
		for (auto tile : open)
		{
			tile->openclosed = 0;
		}
		for (auto tile : closed)
		{
			tile->openclosed = 1;
		}
		/*
		// Render piecewise
		drawMap(winSurface, window, tiles);
		*/
	}
	std::cout << "Error: Open list became empty, could not find path" << std::endl;
	return empty;

	// system("pause");
	// exit(1);
GET_MOVE_SEQUENCE:
	std::vector<tile*> path;
	while (current != begin)
	{
		path.push_back(current);
		current = current->parent_;
	}
	std::reverse(path.begin(), path.end());
	// std::cout << "Found path" << std::endl;
	return path;
}