#include "unit.h"
unit::unit(const std::vector<std::vector<tile*>>& tiles, const int type, const int row, const int column, SDL_Window* window, SDL_Surface* winSurface)
{
	tileAt_ = tiles[row][column];
	type_ = type;
	window_ = window;
	surface_ = winSurface;
	path_.clear();
}

void unit::advance(std::vector<std::vector<tile*>>& tiles)
{
	if (path_.size() != 0)
	{
		// tileAt_->state_ = 0;
		int oldx = tileAt_->x_;
		int oldy = tileAt_->y_;
		tileAt_->onpath = true;
		tileAt_ = path_.front();
		path_.pop_front();
		SDL_Delay(75);
		// tileAt_->state_ = 2;
	}
}

void unit::navigate(std::vector<std::vector<tile*>>& tiles, tile* goal, SDL_Surface* winSurface, SDL_Window* window)
{
	std::vector<tile*> vectorpath;
	vectorpath = astar(winSurface, window, tiles, tileAt_, goal);
	path_.clear();
	for (int i = 0; i < vectorpath.size(); i++)
	{
		path_.push_back(vectorpath[i]);
	}
}