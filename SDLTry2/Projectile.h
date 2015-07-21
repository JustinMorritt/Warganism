#pragma once
#include "GameEntity.h"
#include "MathUtils.h"

class Projectile
{
public:
	GameEntity* m_pProjTex;
	Projectile(int x, int y, int w, int h, Vec2* DIR, SDL_Color* color);
	~Projectile();
};

