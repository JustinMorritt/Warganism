#pragma once
#include "SDL.h"
#include <iostream>
#include <string>
#include <Windows.h>
#include <vector>
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "MathUtils.h"
#include "GameEntity.h"
#include "MyWindow.h"
#include "Projectile.h"



//Fonts: ANUDRG__.ttf


class SDLHelper
{
private:

	float m_dt;
	bool m_done;
	
	SDL_Renderer* m_pRenderer		= NULL;				//The window renderer 
	SDL_Surface* m_pScreenSurface	= NULL;				//The surface contained by the window

	SDL_Texture* m_pbackground		= NULL;
	
	GameEntity* m_pPlayer2				= NULL;
	GameEntity* m_pPlayer1			= NULL;

	SDL_Texture* m_pP1BG = NULL;
	SDL_Texture* m_pP2BG = NULL;

	GameEntity* m_pTextTest			= NULL;
	
	//FONTS
	TTF_Font *m_pFont1 = NULL;		//font "newsPaper Clipping One"		BlackCasper.ttf
	TTF_Font *m_pFont2 = NULL;		//font "Black Rose"					BLACKR.ttf
	TTF_Font *m_pFont3 = NULL;		//font "Scratchy Look"				ANUDRG__.ttf


	SDL_Texture* m_pfontTest			= NULL;

	std::vector<Projectile*> m_P1Projectiles;
	std::vector<Projectile*> m_P2Projectiles;

public:
	MyWindow* m_pMyWindow = NULL;
	//Starts up SDL and creates window
	SDLHelper();
	~SDLHelper();

	//Loads media
	void loadMedia();

	SDL_Texture* loadTexture(std::string path);
	SDL_Texture* loadText(std::string text, SDL_Color color, int font);

	bool Done();
	void Update();
	void SetDT(float dt);

	void SpawnProjectile(bool p1,bool p2);
	void UpdateProjectiles();
	void HandleEvents();
	void KeyBoardHandler(SDL_Event &e);
	void MouseHandler(SDL_Event &e);

	//GEOMETRY FUNCTIONS
	void SetRenderColor(std::string color);
	void DrawRect(int x, int y, int w, int h, std::string color);
	void DrawRectOL(int x, int y, int w, int h, std::string color);
	void DrawLine(int X1, int Y1, int X2, int Y2, std::string color);
	void DrawPoint(int x, int y, std::string color);
	void DrawText();

	//Frees media and shuts down SDL
	void close();
};

