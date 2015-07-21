#include "Projectile.h"



//													DIR BEING THE ANGLE THE PLAYER IS FACING 
Projectile::Projectile(int x, int y, int w, int h, double DIR, std::string color, SDL_Renderer* theRenderer, std::string name)
{
	m_pProjTex = new GameEntity(x-(w/2), y-(h/2), w, h, 100, 10, name, theRenderer, false);
	m_pProjTex->LoadFile("Pics/projectile.png"); // Load Up The Full Sprite Sheet
	m_pProjTex->SetColor(color);
	m_pProjTex->UseGoOneDir(true);
	

	//CALC DIR
	Vec2 goVec; 
	goVec.x = sin(DIR*180/pi);
	goVec.y = cos(DIR*180/pi);

	//SET VELOCITYS FOR DIR
	m_pProjTex->GoOneDirForever(goVec.x, goVec.y);
	m_pProjTex->CalculateRotation(); //Rotate it to the current direction 
}

Projectile::~Projectile()
{
	m_pProjTex->Free();
}
