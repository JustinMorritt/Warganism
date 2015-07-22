#include "PickUp.h"


PickUp::PickUp(int x, int y, int w, int h, SDL_Renderer* theRenderer, std::string name, std::string color, double DIR)
{
	m_pPickUpTex = new GameEntity(x - (w / 2), y - (h / 2), w, h, 0, 0, name, theRenderer, false);
	m_pPickUpTex->LoadFile("Pics/PickUp.png");
	m_pPickUpTex->SetColorMod(color);
	

	Vec2 goVec;
	goVec.x = -sin(DIR*pi / 180);
	goVec.y = cos(DIR*pi / 180);

	//SET VELOCITYS FOR DIR
	m_pPickUpTex->GoOneDirForever(goVec.x, goVec.y);
	m_pPickUpTex->CalculateRotation(); //Rotate it to the current direction 
	m_pPickUpTex->UpdateColliders();
}


PickUp::~PickUp()
{
	m_pPickUpTex->Free();
}
