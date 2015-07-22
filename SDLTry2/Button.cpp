#include "Button.h"




Button::Button(int x, int y, int w, int h, double DIR, SDL_Renderer* theRenderer, std::string name, int bulge)
{
	m_pButtonTex = new GameEntity(x - (w / 2), y - (h / 2), w, h, 300, 10, name, theRenderer, false);
	m_pButtonTex->LoadFile("Pics/projectile.png"); // Load Up The Full Sprite Sheet
	m_pButtonTex->UseMouseEffects(true, bulge);
}

Button::~Button()
{
	m_pButtonTex->Free();
}
