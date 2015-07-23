#pragma once
#include "GameEntity.h"
#include "MathUtils.h"
class Particle
{
public:	
	GameEntity* m_pProjTex;
	bool isDead();

	Particle(int x, int y, int w, int h, double DIR, std::string color, SDL_Renderer* theRenderer, std::string name);
	~Particle();
};

