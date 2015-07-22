#pragma once
#include "GameEntity.h"
#include "MathUtils.h"
class PickUp
{
public:
	GameEntity* m_pPickUpTex;
	PickUp(int x, int y, int w, int h, SDL_Renderer* theRenderer, std::string name, std::string color, double DIR);
	~PickUp();
};

