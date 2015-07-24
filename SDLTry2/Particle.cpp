#include "Particle.h"


Particle::Particle(int x, int y, int w, int h, double DIR, std::string color, SDL_Renderer* theRenderer, std::string name)
{
	m_pSlimeTex = new GameEntity(x - (w / 2), y - (h / 2), w, h, 0, 0, name, theRenderer, false); //TRUE IS AN ANIMATION
	m_pSlimeTex->LoadFile("Pics/particle.png"); // Load Up The Full Sprite Sheet
	m_pSlimeTex->SetColorMod(color);
	m_pSlimeTex->m_Roation = DIR;
	m_pSlimeTex->CalculateRotation();
	m_isDead = false;
	m_deadTime = 5;
	m_currAlpha = 255;

}


Particle::~Particle()
{
	m_pSlimeTex->Free();
}

void Particle::Update(float dt)
{
	 m_timeElapsed += dt;
	 fadeOut();
	
	 if (m_timeElapsed >= m_deadTime)
	 {
		 m_isDead = true;
	 }
}

bool Particle::getDead()
{
	return m_isDead;
}

void Particle::fadeOut()
{
	if (m_currAlpha <= 0)
	{
		m_currAlpha = 0;
	}
	else
	{
		m_currAlpha -= m_deadTime;
	}
	m_pSlimeTex->SetAlpha(m_currAlpha);
}