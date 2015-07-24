#pragma once
#include "GameEntity.h"
#include "MathUtils.h"
class Particle
{
private:
	bool m_isDead;
public:	
	GameEntity* m_pProjTex;
	
	void setDead(bool isdead);
	bool getDead();
	Particle(int x, int y, int w, int h, double DIR, std::string color, SDL_Renderer* theRenderer, std::string name);
	~Particle();
};

