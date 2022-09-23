
#include "unit.h"
#include "tile.h"
#include "player.h"
#include "utils.h"

unit::unit(player* team, const std::vector<std::vector<tile*>>& tiles, const int type, const int row, const int column)
{
	tileAt_ = tiles[row][column];
	type_ = type;
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
        team->units_.push_back(this);
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
			if (tileAt_->magicflag != 62)
			{
				std::cout << "Magic flag of unit " << this << " at tile " << tileAt_ << " was not 62 before moving." << std::endl;
				std::system("pause");
			}
			tileAt_->unitAt_ = NULL;
			tileAt_ = path_.front();
			tileAt_->unitAt_ = this;
			path_.pop_front();
			if (tileAt_->magicflag != 62)
			{
				std::cout << "Magic flag of unit " << this << " on tile " << tileAt_ << " was not 62 after moving." << std::endl;
			}
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

void unit::navigate(std::vector<std::vector<tile*>>& tiles, std::list<unit*>& units, tile* goal)
{
	std::vector<tile*> vectorpath;
	vectorpath = astar(tiles, units, tileAt_, goal);
	path_.clear();
	for (int i = 0; i < vectorpath.size(); i++)
	{
		path_.push_back(vectorpath[i]);
	}
}

void unit::buildFactory(std::list<unit*>& units, std::vector<std::vector<tile*>>& tiles, std::list<tile*>& factories, int factoryTypeSelector)
{
	if (units.size() > 0)
	{

		/*std::cout << "There are " << units.size() << " units." << std::endl;
		std::list<unit*>::iterator it;
		for (it = units.begin(); it != units.end(); it++)
		{
			std::cout << "unit pointer is " << *it << std::endl;
			std::system("pause");
		}
		*/

		// Create copy of units to avoid modifying a currently iterated list
		std::list<unit*> unitsCopy = units;
		bool aboveClear = true;
		bool leftClear = true;
		bool rightClear = true;
		bool belowClear = true;
		int yUp = this->tileAt_->y_ - 1;
		int ySame = this->tileAt_->y_;
		int yDown = this->tileAt_->y_ + 1;
		int xSame = this->tileAt_->x_;
		int xLeft = xSame - 1;
		int xRight = xSame + 1;
		bool placementError = false;
		if (!isLegal(yUp, xSame, tiles)) placementError = true;
		if (!isLegal(ySame, xLeft, tiles)) placementError = true;
		if (!isLegal(ySame, xRight, tiles)) placementError = true;
		if (!isLegal(yDown, xSame, tiles)) placementError = true;
		if (placementError)
		{
			std::cout << "Error: Factory placed directly next to map border." << std::endl;
			std::system("pause");
			exit(1);
		}
		if (tiles[yUp][xSame]->state_ == 3) aboveClear = false;
		if (tiles[ySame][xLeft]->state_ == 3) leftClear = false;
		if (tiles[ySame][xRight]->state_ == 3) rightClear = false;
		if (tiles[yDown][xSame]->state_ == 3) belowClear = false;
		bool aboveNoWall = tiles[yUp][xSame]->state_ != 1;
		bool rightNoWall = tiles[ySame][xRight]->state_ != 1;
		bool belowNoWall = tiles[yDown][xSame]->state_ != 1;

		if (aboveClear && leftClear && rightClear && belowClear)
		{
			// If main unit, this can only be a Builder Factory, also spawn Fighter, Builder, Miner
			if (this->type_ == 0)
			{
				if (aboveNoWall && rightNoWall && belowNoWall)
				{
					if ((this->type_ == 0 || this->type_ == 2) && this->tileAt_->state_ == 0)
					{
						// Set this tile to be a factory tile and add it to the list of factory tiles
						this->tileAt_->claimedBy_ = this->team_;
						this->tileAt_->state_ = 3;
						this->tileAt_->factoryType = 2;
						factories.push_back(this->tileAt_);

						// Create fighter, builder, and miner and add to relevant lists
						units.push_back(new unit(this->team_, tiles, 1, tileAt_->y_ - 1, tileAt_->x_));
						//this->team_->units_.push_back(units.back());
						units.push_back(new unit(this->team_, tiles, 2, tileAt_->y_, tileAt_->x_ + 1));
						//this->team_->units_.push_back(units.back());
						units.push_back(new unit(this->team_, tiles, 3, tileAt_->y_ + 1, tileAt_->x_));
						//this->team_->units_.push_back(units.back());

						// Erase from list of units held by this unit's team
						std::list<unit*>& teamUnitList = this->team_->units_;
						teamUnitList.erase(std::find(teamUnitList.begin(), teamUnitList.end(), this));

						// Erase from global list of units
						units.erase(std::find(units.begin(), units.end(), this));

						this->tileAt_->unitAt_ = NULL; // "corpse" removed from tile

						delete this;
					}
				}
			}
			else if (this->type_ == 2)
			{
				if ((this->type_ == 0 || this->type_ == 2) && this->tileAt_->state_ == 0)
				{
					// Set this tile to be a factory tile and add it to the list of factory tiles
					this->tileAt_->claimedBy_ = this->team_;
					this->tileAt_->state_ = 3;
					this->tileAt_->factoryType = factoryTypeSelector;
					factories.push_back(this->tileAt_);

					// Erase from list of units held by this unit's team
					std::list<unit*>& teamUnitList = this->team_->units_;
					teamUnitList.erase(std::find(teamUnitList.begin(), teamUnitList.end(), this));

					// Erase from global list of units
					units.erase(std::find(units.begin(), units.end(), this));

					this->tileAt_->unitAt_ = NULL; // "corpse" removed from tile

					delete this;
				}
			}
		}
	}
}
