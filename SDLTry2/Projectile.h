#pragma once
#include "GameEntity.h"
#include "MathUtils.h"
#include <iostream>

// TODO: To get the normalized vector from the rotation  Vec2 = {sin(ANGLE),cos(ANGLE)}


class Projectile
{
public:
	GameEntity* m_pProjTex;
	Projectile(int x, int y, int w, int h, double DIR, std::string color, SDL_Renderer* theRenderer, std::string name);
	~Projectile();
};

