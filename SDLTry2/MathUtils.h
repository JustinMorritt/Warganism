#pragma once

#include <windows.h>
#include <xnamath.h>
#include "SDL.h"

const double pi = 3.1415926;

struct Vec2
{
	float x,y;


	Vec2(float x = 0, float y = 0)
	{
		this->x = x;
		this->y = y;
	}

	Vec2 operator + (const Vec2& rhs)
	{
		return Vec2(x + rhs.x, y + rhs.y);
	}

	Vec2 operator - (const Vec2& rhs)
	{
		return Vec2(x - rhs.x, y - rhs.y);
	}

	Vec2 operator * (const Vec2& rhs)
	{
		return Vec2(x * rhs.x, y * rhs.y);
	}

	void operator += (const Vec2& rhs)
	{
		x += rhs.x;
		y += rhs.y;
	}

	void operator -= (const Vec2& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
	}

	Vec2 operator * (const float& rhs)
	{

		return Vec2(x * rhs, y * rhs);
	}

	Vec2 operator / (const float& rhs)
	{
		return Vec2(x / rhs, y / rhs);
	}

	Vec2 operator *= (const float& rhs)
	{

		x *= rhs;
		y *= rhs;
	}

	Vec2 operator /= (const float& rhs)
	{
		x /= rhs;
		y /= rhs;
	}
};



class VectorMath
{
public:
	
	static Vec2& Normalize(const Vec2* vec);
	static float Magnitude(const Vec2* vec);
	
};


struct Circle
{
	int x, y, r;
};


class Collision
{
public:
	static bool CircleVsCircle(Circle& a, Circle& b);
	static bool CircleVsSquare(Circle& a, SDL_Rect& b);
	static double Distance(int x1, int x2, int y1, int y2);
};


