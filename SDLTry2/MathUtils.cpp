#include "MathUtils.h"
#include <math.h>

//USE BY CALLING:    VectorMath::Normalize(const Vec* vec);


Vec2& VectorMath::Normalize(const Vec2* vec)
{
	Vec2 retval;
	XMVECTOR xmVec = XMLoadFloat2((_XMFLOAT2*)vec);
	xmVec = XMVector2Normalize(xmVec);
	XMStoreFloat2((XMFLOAT2*)&retval, xmVec);
	return retval;
}

float VectorMath::Magnitude(const Vec2* vec)
{
	XMVECTOR xmVec = XMLoadFloat2((_XMFLOAT2*)vec);
	return XMVector2Length(xmVec).m128_f32[0];
}

bool Collision::CircleVsCircle(Circle& a, Circle& b)
{
	//CALCULATE TOTAL RADII
	int totalRadiusSquared = a.r + b.r;
	totalRadiusSquared = totalRadiusSquared * totalRadiusSquared;

	if ((Distance(a.x, b.x, a.y, b.y)) < (totalRadiusSquared) )
	{
		return true;
	}
	return false;
}

bool Collision::CircleVsSquare(Circle& a, SDL_Rect& b)
{
	int cX, cY;

	//FIND CLOSEST X OFFEST
	if (a.x < b.x){ cX = b.x; }						//CIRCLE ON LEFT
	else if (a.x > b.x + b.w){ cX = b.x + b.w; }	//CIRCLE ON RIGHT
	else { cX = a.x; }								//CIRCLE BETWEEN

	//FIND CLOSEST Y OFFSET
	if (a.y < b.y){ cY = b.y; }						//CIRCLE ON TOP
	else if (a.y > b.y + b.h){ cY = b.y; }			//CIRCLE ON BOT
	else { cY = a.y; }								//CIRCLE BETWEEN

	//IF CLOSEST POINT INSIDE CIRCLE...COLLISION
	if (Distance(a.x, cX, a.y, cY)){ return true; }	//COLLISION
	return false;									//NO COLLISION
}
double Collision::Distance(int x1, int x2, int y1, int y2)
{
	int deltaX = x2 - x1;
	int deltaY = y2 - y1;
	return deltaX*deltaX + deltaY*deltaY;
}
