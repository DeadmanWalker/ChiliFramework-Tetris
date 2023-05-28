#pragma once

class Location
{
public:
	bool operator==(const Location& rhs) const
	{
		return x == rhs.x && y == rhs.y;
	}
	Location operator+(const Location& rhs) const
	{
		return { x + rhs.x,y + rhs.y };
	}
	void Add(const Location& val)
	{
		x += val.x;
		y += val.y;
	}
	int x;
	int y;
};