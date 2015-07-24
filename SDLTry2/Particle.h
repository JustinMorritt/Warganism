#pragma once
#include "GameEntity.h"
#include "MathUtils.h"
class Particle
{
private:
	bool m_isDead;
	float m_timeElapsed;
	float m_deadTime;
	int m_currAlpha;
public:	
	GameEntity* m_pSlimeTex;
	void Update(float dt);
	void fadeOut();
	bool getDead();
	Particle(int x, int y, int w, int h, double DIR, std::string color, SDL_Renderer* theRenderer, std::string name);
	~Particle();
};

