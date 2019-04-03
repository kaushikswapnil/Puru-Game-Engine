#pragma once

class puruRect
{
public:
	float	x1, y1, x2, y2;

	puruRect(float _x1, float _y1, float _x2, float _y2);
	puruRect();
	~puruRect();

	void	Set(float _x1, float _y1, float _x2, float _y2) { x1 = _x1; x2 = _x2; y1 = _y1; y2 = _y2; }
	void	SetRadius(float x, float y, float r) { x1 = x - r; x2 = x + r; y1 = y - r; y2 = y + r; }
	bool	TestPoint(float x, float y);
	//bool	Intersect(const entityRect *rect);

private:
	bool	bClean;
};