#pragma once
#include "GameEntity.h"
#include <iostream>

class Button
{
public:
	GameEntity* m_pButtonTex;
	Button(int x, int y, int w, int h, double DIR, SDL_Renderer* theRenderer, std::string name, int bulge);
	~Button();
};

