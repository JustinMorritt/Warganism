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
#include "PickUp.h"
#include "randgen.h"
#include "Button.h"
#include "SDL_mixer.h"
#include "Particle.h"
#include "StateMachine.h"



//Fonts: ANUDRG__.ttf


class SDLHelper
{
private:
	int m_MouseX;
	int m_MouseY;


	RandGen m_RG;
	float m_dt;
	bool m_done;
	float m_LPickUpSpawnTimeElapsed;
	float m_RPickUpSpawnTimeElapsed;
	float m_LPickUpSpawnTime;
	float m_RPickUpSpawnTime;

	float m_LSlimeSpawnTimeElapsed;
	float m_RSlimeSpawnTimeElapsed;
	float m_LSlimepawnTime;
	float m_RSlimeSpawnTime;

	float m_LSlimeDeadTimeElapsed;
	float m_RSlimeDeadTimeElapsed;
	float m_LSlimeDeadTime;
	float m_RSlimeTime;

	SDL_Renderer* m_pRenderer		= NULL;				//The window renderer 
	SDL_Surface* m_pScreenSurface	= NULL;				//The surface contained by the window

	SDL_Texture* m_pbackground		= NULL;
	
	//PLAYERS
	GameEntity* m_pPlayer2			= NULL;
	GameEntity* m_pPlayer1			= NULL;

	//BACKGROUNDS
	SDL_Texture* m_pP1BG			= NULL;
	SDL_Texture* m_pP2BG			= NULL;

	GameEntity* m_pInGameTitle		= NULL;
	GameEntity* m_pInGameAmmoP1		= NULL;
	GameEntity* m_pInGameAmmoP2	    = NULL;
	GameEntity* m_pInGameSizeP1		= NULL;
	GameEntity* m_pInGameSizeP2		= NULL;
	GameEntity* m_pPaused			= NULL;

	//FONTS
	TTF_Font *m_pFont1 = NULL;		//font "newsPaper Clipping One"		BlackCasper.ttf
	TTF_Font *m_pFont2 = NULL;		//font "Black Rose"					BLACKR.ttf
	TTF_Font *m_pFont3 = NULL;		//font "Scratchy Look"				ANUDRG__.ttf


	SDL_Texture* m_pfontTest			= NULL;

	std::vector<Projectile*> m_P1Projectiles;
	std::vector<Projectile*> m_P2Projectiles;
	std::vector<PickUp*> m_P1PickUps;
	std::vector<PickUp*> m_P2PickUps;
	std::vector<Button*> m_Buttons;
	std::vector<Particle*> m_RParticles;
	std::vector<Particle*> m_LParticles;


	//Music files and Sound Effects
	Mix_Music* m_pSong1               = NULL;
	Mix_Music* m_pSong2               = NULL;

	Mix_Chunk *m_pP1Fire			  = NULL;
	Mix_Chunk* m_pP2Fire			  = NULL;
	Mix_Chunk* m_pP1Hit				  = NULL;
	Mix_Chunk* m_pP2Hit				  = NULL;
	Mix_Chunk* m_pGrow				  = NULL;

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
	void SpawnRPickUp();
	void SpawnLPickUp();
	void UpdateProjectiles();
	void UpdatePickUp();
	void HandleEvents();
	void KeyBoardHandler(SDL_Event &e);
	void MouseHandler(SDL_Event &e);

	//GAME FUNCTIONS (SHOW)
	void ShowMainMenu();
	void ShowPaused();
	void ShowP1Win();
	void ShowP2Win();
	void ShowCharSelection();
	void ShowGameOn();

	//GAME FUNCTIONS (LOAD)
	void LoadMainMenu();
	void LoadPaused();
	void LoadP1Win();
	void LoadP2Win();
	void LoadCharSelection();
	void LoadGameOn();



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

