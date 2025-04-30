#pragma once
#ifndef _TILE_

class Tile
{
public:
	Tile();
	int getId();

	void setId(int);

	int id, depth, idx_x, idx_y, isSelected, hasObj;
	float x, y;
};
#endif // !_TILE_
