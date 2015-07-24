#include "Particle.h"


Particle::Particle(int x, int y, int w, int h, double DIR, std::string color, SDL_Renderer* theRenderer, std::string name)
{
	m_pProjTex = new GameEntity(x - (w / 2), y - (h / 2), w, h, 300, 10, name, theRenderer, true); //TRUE IS AN ANIMATION
	m_pProjTex->LoadFile("Pics/particle.png"); // Load Up The Full Sprite Sheet
	m_pProjTex->SetColorMod(color);

	m_pProjTex->m_Roation = DIR;
	m_pProjTex->CalculateRotation();
	m_isDead = false;
	

}


Particle::~Particle()
{
}

void Particle::setDead(bool isdead)
{

	m_isDead = isdead;
}

bool Particle::getDead()
{
	return m_isDead;
}