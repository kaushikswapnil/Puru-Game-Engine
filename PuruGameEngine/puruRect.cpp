#include "puruRect.h"

puruRect::puruRect(float _x1, float _y1, float _x2, float _y2) : x1(_x1), y1(_y1), x2(_x2), y2(_y2)
{
}

puruRect::puruRect()
{
}

puruRect::~puruRect()
{
}

bool puruRect::TestPoint(float x, float y)
{
	if (x >= x1 && x <= x2 && y >= y1 && y <= y2)
		return true;
	return false;
}
