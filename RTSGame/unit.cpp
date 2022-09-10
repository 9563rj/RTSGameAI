#pragma once
#include "unit.h"
#include "tile.h"
#include "player.h"

unit::unit(player* team, const std::vector<std::vector<tile*>>& tiles, const int type, const int row, const int column, SDL_Window* window, SDL_Surface* winSurface)
{
	tileAt_ = tiles[row][column];
	type_ = type;
	window_ = window;
	surface_ = winSurface;
	path_.clear();
	team_ = team;
	resourceMineFlag = true;
	unitMoveFlag = true;
	switch (type_)
	{
	case(0):
		health_ = 10;
		break;
	case(1):
		health_ = 50;
		break;
	case(2):
		health_ = 10;
		break;
	case(3):
		health_ = 100;
		break;
	}
}

void unit::advance(std::vector<std::vector<tile*>>& tiles)
{
	if (path_.size() != 0 && unitMoveFlag)
	{
		if (path_.front()->unitAt_ == NULL)
		{
			// tileAt_->state_ = 0;
			// int oldx = tileAt_->x_;
			// int oldy = tileAt_->y_;
			// tileAt_->onpath = true;
			tileAt_->unitAt_ = NULL;
			tileAt_ = path_.front();
			tileAt_->unitAt_ = this;
			path_.pop_front();
			// SDL_Delay(75);
			unitMoveFlag = false;
			// tileAt_->state_ = 2;
		}
		else
		{
			path_.clear();
			std::cout << "Unit " << this << " was blocked at " << tileAt_->x_ << ", " << tileAt_->y_ << std::endl;
		}

	}
	else if (tileAt_->state_ == 2 && resourceMineFlag && team_->resources_ < team_->maxResources_ && type_ == 3)
	{
		team_->resources_++;
		resourceMineFlag = false;
	}
}

void unit::navigate(std::vector<std::vector<tile*>>& tiles, std::list<unit*>& units, tile* goal, SDL_Surface* winSurface, SDL_Window* window)
{
	std::vector<tile*> vectorpath;
	vectorpath = astar(winSurface, window, tiles, units, tileAt_, goal);
	path_.clear();
	for (int i = 0; i < vectorpath.size(); i++)
	{
		path_.push_back(vectorpath[i]);
	}
}
