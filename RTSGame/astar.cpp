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
	while (true)
	{
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

				successors.push_back(&tiles[ni][nj]);
				std::cout << "Successor found at " << ni << " " << nj << std::endl;
			}
		}
#if 0
		// calculate successor h values
		for (int i = 0; i <= 2; i++)
		{
			for (int j = 0; j <= 2; j++)
			{
				int numHoriMoves;
				int numVertMoves;
				int numLinearMoves;
				int numDiagMoves;
				if (successors[i][j].x_ < goal->x_) { numHoriMoves = goal->x_ - successors[i][j].x_; }
				else { numHoriMoves = successors[i][j].x_ - goal->x_; }
				if (successors[i][j].y_ < goal->y_) { numVertMoves = goal->y_ - successors[i][j].y_; }
				else { numVertMoves = successors[i][j].y_ - goal->y_; }
				if (numHoriMoves < numVertMoves)
				{
					numLinearMoves = numVertMoves - numHoriMoves;
					numDiagMoves = numVertMoves - numLinearMoves;
				}
				else
				{
					numLinearMoves = numHoriMoves - numVertMoves;
					numDiagMoves = numHoriMoves - numLinearMoves;
				}
				successors[i][j].h_ = numLinearMoves * 10 + numDiagMoves * 14;
				tiles[j + current->x_ - 1][i + current->y_ - 1].h_ = successors[i][j].h_;
			}
		}
		// calculate successor f,g values
		for (int i = 0; i <= 2; i++)
		{
			for (int j = 0; j <= 2; j++)
			{
				if (j == 0 && i == 0) { successors[i][j].g_ = tiles[current->x_][current->y_].g_ + 14; }
				else if (j == 2 && i == 0) { successors[i][j].g_ = tiles[current->x_][current->y_].g_ + 14; }
				else if (j == 0 && i == 2) { successors[i][j].g_ = tiles[current->x_][current->y_].g_ + 14; }
				else if (j == 2 && i == 2) { successors[i][j].g_ = tiles[current->x_][current->y_].g_ + 14; }
				else { successors[i][j].g_ = tiles[current->x_][current->y_].g_ + 10; }
				successors[i][j].f_ = successors[i][j].g_ + successors[i][j].h_;
				// synchronise tiles and successors
				tiles[current->x_ + j - 1][current->y_ + i - 1].f_ = successors[i][j].f_;
				tiles[current->x_ + j - 1][current->y_ + i - 1].g_ = successors[i][j].g_;
			}
		}
		// move successor nodes to open or closed depending on g value
		for (int i = 0; i <= 2; i++)
		{
			for (int j = 0; j <= 2; j++)
			{
				if (openclosedlist[j - 1 + current->x_][i - 1 + current->y_] == 1)
				{
					if (successors[i][j].g_ <= successors[1][1].g_ && j != 1 && i != 1)
					{
						openclosedlist[j - 1 + current->x_][i - 1 + current->x_] = 0;
					}
				}
			}
		}

		// find lowest f value, init to very big
		int lowestf = INT_MAX;
		std::vector<tile> candidates;
		for (int i = 0; i<tiles.size(); i++)
		{
			for (int j = 0; j<tiles[0].size(); j++)
			{
				if (openclosedlist[i][j] == 0)
				{
					if (tiles[i][j].f_ < lowestf) 
					{ 
						lowestf = tiles[i][j].f_; 
						candidates.push_back(tiles[i][j]);
					}
				}
			}
		}
		// Set current tile to closed and synchronize openclosedlist with tile openclosed
		openclosedlist[current->x_][current->y_] = 1;
		for (int i = 0; i < openclosedlist.size(); i++)
		{
			for (int j = 0; j < tiles[0].size(); j++)
			{
				tiles[i][j].openclosed = openclosedlist[i][j];
			}
		}
#endif
		// Explore next tile
		// Debug- Draw map each iteration
		drawMap(winSurface, window, tiles);
		int prevx = current->x_;
		int prevy = current->y_;
		/*current->x_ = candidates.back().x_;
		current->y_ = candidates.back().y_;
		*/
		current->parent_ = &tiles[prevx][prevy];
	}
GET_MOVE_SEQUENCE:
	int pathcurrentx = current->x_;
	int pathcurrenty = current->y_;
	int pathnextx = tiles[current->y_][current->x_].parent_->x_;
	int pathnexty = tiles[current->y_][current->x_].parent_->y_;
	std::vector<tile*> path;
	while (tiles[pathnexty][pathnextx].state_ != 2)
	{
		path.push_back(&tiles[pathcurrenty][pathcurrentx]);
		pathcurrentx = pathnextx;
		pathcurrenty = pathnexty;
		pathnextx = tiles[pathcurrenty][pathcurrentx].parent_->x_;
		pathnexty = tiles[pathcurrenty][pathcurrentx].parent_->y_;
	}
	return path;
}