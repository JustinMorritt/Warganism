#include "PickUp.h"


PickUp::PickUp(int x, int y, int w, int h, SDL_Renderer* theRenderer, std::string name, std::string color)
{
	m_pPickUpTex = new GameEntity(x, y, w, h, 0, 0, name, theRenderer, false);
	m_pPickUpTex->LoadFile("Pics/projectile.png");
	m_pPickUpTex->SetColorMod(color);

}


PickUp::~PickUp()
{
	m_pPickUpTex->Free();
}
