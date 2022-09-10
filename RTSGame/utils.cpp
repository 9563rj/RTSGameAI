#include "utils.h"
bool isLegal(int r, int c, const std::vector<std::vector<tile*>>& tiles)
{
	int maxr = tiles.size();
	int maxc = tiles[0].size();
	if (r >= maxr || c >= maxc || r < 0 || c < 0) return false;
	else return true;
}