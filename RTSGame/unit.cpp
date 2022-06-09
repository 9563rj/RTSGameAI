#include "unit.h"

unit::unit(const std::vector<std::vector<tile*>>& tiles, const int type, const int row, const int column, SDL_Window* window, SDL_Surface* winSurface)
{
	tileAt_ = tiles[row][column];
	type_ = type;
	window_ = window;
	surface_ = winSurface;
}

void unit::advance(std::vector<std::vector<tile*>>& tiles, std::vector<tile*>& path)
{
	tileAt_->state_ = 0;
	std::cout << "Moving along path of length " << path.size() << std::endl;
	for (int pathiterator = 0; pathiterator < path.size(); pathiterator++)
	{
		int oldx = tileAt_->x_;
		int oldy = tileAt_->y_;
		tileAt_->onpath = true;
		tileAt_ = path[pathiterator];
		drawMap(surface_, window_, tiles);
		std::cout << "Moving to tile r=" << tileAt_->y_ << " and c=" << tileAt_->x_ << " from tile r=" << oldy << " and c=" << oldx << std::endl;
	}
	tileAt_->state_ = 2;
}
