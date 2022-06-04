#include "astar.h"

std::vector<tile*> astar(SDL_Surface* winSurface, SDL_Window* window, std::vector<std::vector<tile>> &tiles)
{
	// Initialization
	std::list<tile*> open;
	std::list<tile*> closed;
	tile* current = NULL;
	tile* goal = NULL;
	for (int i = 0; i < tiles.size(); i++)
	{
		for (int j = 0; j < tiles[0].size(); j++)
		{
			if (tiles[i][j].state_ == 3) 
			{
				goal = &tiles[i][j];
			}
			if (tiles[i][j].state_ == 2)
				current = &tiles[i][j];
		}
	}
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

		/*
		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				if (current->x_+j>-1 && current->y_+i > -1 && current->y_+i<=tiles.size() && current->x_+j<=tiles[0].size() && current->state_ != 1)
				{
					if (!(j == 0 && i == 0)) 
					{ 
						successors.push_back(&tiles[current->x_ + j][current->y_ + i]);
						std::cout << "Successor found at " << current->x_ + j << " " << current->y_ + i << std::endl;
					}
				}
			}
		}
		*/

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
				if (tiles[ni][nj].state_ == 1) continue;

				successors.push_back(&tiles[ni][nj]);
				std::cout << "Successor found at " << ni << " " << nj << std::endl;
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
		// Draw map each iteration
		drawMap(winSurface, window, tiles);
	}
	std::cout << "Error: Open list became empty" << std::endl;
	system("pause");
	exit(1);
GET_MOVE_SEQUENCE:
	int pathcurrentx = current->x_;
	int pathcurrenty = current->y_;
	int pathnextx = tiles[current->y_][current->x_].parent_->x_;
	int pathnexty = tiles[current->y_][current->x_].parent_->y_;
	std::vector<tile*> path;
	while (tiles[pathcurrenty][pathcurrentx].state_ != 2)
	{
		path.push_back(&tiles[pathcurrenty][pathcurrentx]);
		pathcurrentx = pathnextx;
		pathcurrenty = pathnexty;
		pathnextx = tiles[pathcurrenty][pathcurrentx].parent_->x_;
		pathnexty = tiles[pathcurrenty][pathcurrentx].parent_->y_;
	}
	return path;
}