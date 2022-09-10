#include "buildfactory.h"
#include "player.h"
#include "tile.h"
#include "unit.h"

void buildFactory(int row, int column, std::list<unit*>& units, std::vector<std::vector<tile*>>& tiles, std::list<tile*>& factories, unit* currentunit, SDL_Surface* winSurface, SDL_Window* window, int factoryTypeSelector)
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
		for (auto unitPtr : unitsCopy)
		{
			bool aboveClear = true;
			bool leftClear = true;
			bool rightClear = true;
			bool belowClear = true;
			if (tiles[unitPtr->tileAt_->y_ - 1][unitPtr->tileAt_->x_]->state_ == 3) aboveClear = false;
			if (tiles[unitPtr->tileAt_->y_][unitPtr->tileAt_->x_ - 1]->state_ == 3) leftClear = false;
			if (tiles[unitPtr->tileAt_->y_][unitPtr->tileAt_->x_ + 1]->state_ == 3) rightClear = false;
			if (tiles[unitPtr->tileAt_->y_ + 1][unitPtr->tileAt_->x_]->state_ == 3) belowClear = false;
			if (aboveClear && leftClear && rightClear && belowClear)
			{
				// If main unit, this can only be a Builder Factory, also spawn Fighter, Builder, Miner
				if (unitPtr->type_ == 0)
				{
					if (unitPtr->tileAt_ == tiles[row][column] && (unitPtr->type_ == 0 || unitPtr->type_ == 2) && unitPtr->tileAt_->state_ == 0)
					{
						if (currentunit == unitPtr) currentunit = NULL;

						// Set this tile to be a factory tile and add it to the list of factory tiles
						unitPtr->tileAt_->claimedBy_ = unitPtr->team_;
						unitPtr->tileAt_->state_ = 3;
						unitPtr->tileAt_->factoryType = 2;
						factories.push_back(unitPtr->tileAt_);

						// Create fighter, builder, and miner and add to relevant lists
						units.push_back(new unit(unitPtr->team_, tiles, 1, row - 1, column, window, winSurface));
						unitPtr->team_->units_.push_back(units.back());
						units.push_back(new unit(unitPtr->team_, tiles, 2, row, column + 1, window, winSurface));
						unitPtr->team_->units_.push_back(units.back());
						units.push_back(new unit(unitPtr->team_, tiles, 3, row + 1, column, window, winSurface));
						unitPtr->team_->units_.push_back(units.back());

						// Erase from list of units held by this unit's team
						std::list<unit*>& teamUnitList = unitPtr->team_->units_;
						teamUnitList.erase(std::find(teamUnitList.begin(), teamUnitList.end(), unitPtr));

						// Erase from global list of units
						units.erase(std::find(units.begin(), units.end(), unitPtr));

						unitPtr->tileAt_->unitAt_ = NULL; // "corpse" removed from tile

						delete unitPtr;
					}
				}
				else if (unitPtr->type_ == 2)
				{
					if (unitPtr->tileAt_ == tiles[row][column] && (unitPtr->type_ == 0 || unitPtr->type_ == 2) && unitPtr->tileAt_->state_ == 0)
					{
						if (currentunit == unitPtr) currentunit = NULL;

						// Set this tile to be a factory tile and add it to the list of factory tiles
						unitPtr->tileAt_->claimedBy_ = unitPtr->team_;
						unitPtr->tileAt_->state_ = 3;
						unitPtr->tileAt_->factoryType = factoryTypeSelector;
						factories.push_back(unitPtr->tileAt_);

						// Erase from list of units held by this unit's team
						std::list<unit*>& teamUnitList = unitPtr->team_->units_;
						teamUnitList.erase(std::find(teamUnitList.begin(), teamUnitList.end(), unitPtr));

						// Erase from global list of units
						units.erase(std::find(units.begin(), units.end(), unitPtr));

						unitPtr->tileAt_->unitAt_ = NULL; // "corpse" removed from tile

						delete unitPtr;
					}
				}
			}
		}
	}
}