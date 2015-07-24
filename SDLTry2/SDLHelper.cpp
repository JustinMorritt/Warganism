#include "SDLHelper.h"
//TODO re colour all the colour buttons to be the little dude



SDLHelper::SDLHelper() : m_dt(0.0f), m_done(false)
{
	//SET INITIAL GAMESTATE

	*StateMachine::pGameState = GameState::MAINMENU;
	if (*StateMachine::pGameState == GameState::MAINMENU){ std::cout << "MAINMENU ON" << std::endl; }
	if (*StateMachine::pGameState == GameState::GAMEON){ std::cout << "GAMESCREEN ON" << std::endl; }

	//SET LOADED STATE
	*StateMachine::pLoadedState = LoadedState::NONE; //ININTIALLY NONE

	//SET MUSIC AND SOUND STATE
	*StateMachine::pMusicState = MusicState::MUSICON;
	*StateMachine::pSoundState = SoundState::SOUNDON;

	//SET GAME MODE
	*StateMachine::pGameMode = GameMode::PVP;

	//SDL INIT
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("SDL_Init Error:", SDL_GetError());
	}

	// load support for the OGG and MOD sample/music formats
	int flags2 = MIX_INIT_OGG | MIX_INIT_MOD | MIX_INIT_MP3;
	int initted2 = Mix_Init(flags2);
	if (initted2&flags2 != flags2) {
		printf("Mix_Init: Failed to init required ogg and mod support!\n");
		printf("Mix_Init: %s\n", Mix_GetError());
		// handle error
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) { printf("SDL_mixer could not initialize! SDL_mixer Error: \n", Mix_GetError()); }


	m_pMyWindow = new MyWindow();
	m_pMyWindow->init();

	//Create renderer for window
	m_pRenderer = m_pMyWindow->createRenderer();
	if (m_pRenderer == NULL)
	{
		std::cout << "Renderer could not be created! SDL Error: \n" << SDL_GetError() << std::endl;
	}

	//Initialize renderer color
	SDL_SetRenderDrawBlendMode(m_pRenderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(m_pRenderer, 0x00, 0xFF, 0x9A, 0xFF);

	//Initialize PNG loading
	int flags = IMG_INIT_JPG | IMG_INIT_PNG;
	int initted = IMG_Init(flags);
	if (initted & flags != flags)
	{
		std::cout << "could not init SDL_Image" << std::endl;
		std::cout << "Reason: " << IMG_GetError() << std::endl;
	}

	//Initialize SDL_ttf
	if (TTF_Init() == -1)
	{
		printf("SDL_ttf could not initialize! SDL_ttf Error:\n", TTF_GetError());
	}

	
	
	//Get window surface
	m_pScreenSurface = SDL_GetWindowSurface(m_pMyWindow->m_pWindow);

	
	m_RPickUpSpawnTime = m_RG(15 - 7) + 7;
	m_LPickUpSpawnTime = m_RG(15 - 7) + 7;
	m_LSlimeSpawnTime = 0.01;
	m_RSlimeSpawnTime = 0.01;
	loadMedia();
	
}


SDLHelper::~SDLHelper()
{
}

void SDLHelper::loadMedia()
{
	m_pFont1 = TTF_OpenFont("Fonts/BlackCasper.ttf", 300);
	if (m_pFont1 == NULL){ std::cout << "ERROR FONT NOT LOADED ... NULL" << std::endl; }

	m_pFont2 = TTF_OpenFont("Fonts/alpha_echo.ttf", 300);
	if (m_pFont2 == NULL){ std::cout << "ERROR FONT NOT LOADED ... NULL" << std::endl; }

	m_pFont3 = TTF_OpenFont("Fonts/ANUDRG__.ttf", 300);
	if (m_pFont3 == NULL){ std::cout << "ERROR FONT NOT LOADED ... NULL" << std::endl; }


	m_pSong1 = Mix_LoadMUS("Sound/mainfunk.ogg");
	if (m_pSong1 == NULL){ std::cout << "ERROR MUSIC NOT LOADED ... NULL" << std::endl; }
	m_pSong2 = Mix_LoadMUS("Sound/funk.ogg");
	if (m_pSong2 == NULL){ std::cout << "ERROR MUSIC NOT LOADED ... NULL" << std::endl; }
	m_pP1Fire = Mix_LoadWAV("Sound/woosh1.ogg");
	if (m_pP1Fire == NULL){ std::cout << "ERROR SOUND NOT LOADED ... NULL" << std::endl; }
	m_pP2Fire = Mix_LoadWAV("Sound/woosh3.ogg");
	if (m_pP2Fire == NULL){ std::cout << "ERROR SOUND NOT LOADED ... NULL" << std::endl; }
	m_pP1Hit = Mix_LoadWAV("Sound/splat2.ogg");
	if (m_pP1Hit == NULL){ std::cout << "ERROR SOUND NOT LOADED ... NULL" << std::endl; }
	m_pP2Hit = Mix_LoadWAV("Sound/splat1.ogg");
	if (m_pP2Hit == NULL){ std::cout << "ERROR SOUND NOT LOADED ... NULL" << std::endl; }
	m_pGrow = Mix_LoadWAV("Sound/grow1.ogg");
	if (m_pGrow == NULL){ std::cout << "ERROR SOUND NOT LOADED ... NULL" << std::endl; }

	*StateMachine::pMusicState = MusicState::MUSICON;

	m_pPaused = new GameEntity(0, 0, 300, 80, 0, 0, "Paused", m_pRenderer, false);
}


SDL_Texture* SDLHelper::loadTexture(std::string path)
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(m_pRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}
	return newTexture;
}

SDL_Texture* SDLHelper::loadText(std::string text, SDL_Color color, int font)
{
	//Render text surface
	SDL_Texture* retText = NULL;
	SDL_Surface* textSurface = TTF_RenderText_Solid(m_pFont1, text.c_str(), color);
	if (textSurface == NULL)
	{
		printf("Unable to load music, SDL_Mixer Error: %s\n", TTF_GetError());
	}
	else
	{
		//Create texture from surface pixels
		retText = SDL_CreateTextureFromSurface(m_pRenderer, textSurface);
		if (retText == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: \n", SDL_GetError());
		}
	}
	SDL_FreeSurface(textSurface);
	return retText;

}
bool SDLHelper::Done()
{
	return m_done;
}


void SDLHelper::SetDT(float dt)
{
	m_dt = dt;
}



//UPDATE *************************************************************
void SDLHelper::Update()
{

	HandleEvents();

	//Clear screen
	SDL_RenderClear(m_pRenderer);


	if (*StateMachine::pGameState == GameState::MAINMENU){ ShowMainMenu(); }
	if (*StateMachine::pGameState == GameState::CHARACTERSELECT){ ShowCharSelection(); }
	if (*StateMachine::pGameState == GameState::GAMEON){ ShowGameOn(); }
	if (*StateMachine::pGameState == GameState::PAUSED){ ShowPaused(); }
	if (*StateMachine::pGameState == GameState::P1WIN){ ShowP1Win(); }
	if (*StateMachine::pGameState == GameState::P2WIN){ ShowP2Win(); }

	//Update screen
	SDL_RenderPresent(m_pRenderer);
}








//GAME FUNCTIONS**************************************************

void SDLHelper::SpawnProjectile(bool p1, bool p2)
{
	if (p1)
	{

	
		if (*StateMachine::pSoundState == SoundState::SOUNDON){ Mix_PlayChannel(-1, m_pP1Fire, 0); }
		
		
		Projectile* proj = new Projectile(m_pPlayer1->getCenter().x, m_pPlayer1->getCenter().y, m_pPlayer1->getWidth() / 2, m_pPlayer1->getHeight() / 2, m_pPlayer1->m_Roation, GameEntity::m_P1color, m_pRenderer, "P1projectile");
		m_P1Projectiles.push_back(proj);
	}
	else if (p2)
	{
	
		if (*StateMachine::pSoundState == SoundState::SOUNDON){ Mix_PlayChannel(-1, m_pP2Fire, 0); }
		
		//std::cout << "Made a Projectile" << std::endl;
		Projectile* proj = new Projectile(m_pPlayer2->getCenter().x, m_pPlayer2->getCenter().y, m_pPlayer2->getWidth() / 2, m_pPlayer2->getHeight() / 2, m_pPlayer2->m_Roation, GameEntity::m_P2color, m_pRenderer, "P2projectile");
		m_P2Projectiles.push_back(proj);
	}

}

void SDLHelper::SpawnRPickUp()
{

	int randomX = m_RG(MyWindow::getWidth() - MyWindow::getWidth() / 2) + MyWindow::getWidth() / 2;
	int randomY = m_RG(MyWindow::getHeight()) + 1;
	int randomDir = m_RG(360);
	int randomSize = m_RG(70 - 35) + 35;
	if (randomX > MyWindow::getWidth() / 2)
	{
		PickUp* pickUp = new PickUp(randomX, randomY, randomSize, randomSize, m_pRenderer, "PickUp", GameEntity::m_P1color, randomDir);
		m_P2PickUps.push_back(pickUp);
	}


}

void SDLHelper::SpawnLSlime()
{
	int x = m_pPlayer1->getCenter().x;
	int y = m_pPlayer1->getCenter().y;
	int dir = m_pPlayer1->m_Roation;
	int size = 32;
	std::string color = m_pPlayer1->m_color;
	Particle* slime = new Particle(x, y, size, size, dir, color, m_pRenderer, "Slime");
	m_LSlime.push_back(slime);

}

void SDLHelper::SpawnRSlime()
{
	int x = m_pPlayer2->getCenter().x;
	int y = m_pPlayer2->getCenter().y;
	int dir = m_pPlayer2->m_Roation;
	int size = 32;
	std::string color = m_pPlayer2->m_color;
	Particle* slime = new Particle(x, y, size, size, dir, color, m_pRenderer, "Slime");
	m_RSlime.push_back(slime);

}

void SDLHelper::SpawnLPickUp()
{

	int randomX = m_RG(MyWindow::getWidth() / 2) + 1;
	int randomY = m_RG(MyWindow::getHeight()) + 1;
	int randomDir = m_RG(360);

	int randomSize = m_RG(70 - 35) + 35;


	if (randomX < MyWindow::getWidth() / 2)
	{
		PickUp* pickUp = new PickUp(randomX, randomY, randomSize, randomSize, m_pRenderer, "PickUp", GameEntity::m_P2color, randomDir);
		m_P1PickUps.push_back(pickUp);
	}


}

void SDLHelper::UpdatePickUp()
{
	if (m_P1PickUps.size() > 0)
	{
		for (int i = 0; i < m_P1PickUps.size(); ++i)
		{
			m_P1PickUps[i]->m_pPickUpTex->Render();

			if (Collision::CircleVsCircle(m_P1PickUps[i]->m_pPickUpTex->GetCircleCollider(), m_pPlayer1->GetCircleCollider()))
			{
				m_P1PickUps.erase(m_P1PickUps.begin() + i);
				if (*StateMachine::pSoundState == SoundState::SOUNDON){ Mix_PlayChannel(-1, m_pGrow, 0); }
				m_pPlayer1->GetBigger();
				m_pPlayer1->increaseCurrAmmo();
				break;
			}
		}
	}

	if (m_P2PickUps.size() > 0)
	{
		for (int i = 0; i < m_P2PickUps.size(); ++i)
		{
			m_P2PickUps[i]->m_pPickUpTex->Render();

			if (Collision::CircleVsCircle(m_P2PickUps[i]->m_pPickUpTex->GetCircleCollider(), m_pPlayer2->GetCircleCollider()))
			{
				m_P2PickUps.erase(m_P2PickUps.begin() + i);
				if (*StateMachine::pSoundState == SoundState::SOUNDON){ Mix_PlayChannel(-1, m_pGrow, 0); }
				m_pPlayer2->GetBigger();
				m_pPlayer2->increaseCurrAmmo();
				break;
			}
		}
	}
}

void SDLHelper::UpdateProjectiles()
{

	if (m_P1Projectiles.size() > 0)
	{
		for (int i = 0; i < m_P1Projectiles.size(); i++)
		{
			if (*StateMachine::pGameState != GameState::PAUSED &&
				*StateMachine::pGameState != GameState::P1WIN &&
				*StateMachine::pGameState != GameState::P2WIN)
			{
				m_P1Projectiles[i]->m_pProjTex->Update(m_dt);
			}
			m_P1Projectiles[i]->m_pProjTex->Render();

			if (Collision::CircleVsCircle(m_P1Projectiles[i]->m_pProjTex->GetCircleCollider(), m_pPlayer2->GetCircleCollider()) && m_P1Projectiles[i]->m_pProjTex->UsingCircleCollider())
			{
				//std::cout << "Erased a P1 Projectile -- COLLISION" << std::endl;
				if (*StateMachine::pSoundState == SoundState::SOUNDON){ Mix_PlayChannel(-1, m_pP2Hit, 0); }

				
				m_pPlayer2->GetSmaller();
				m_P1Projectiles[i]->m_pProjTex->SetCurrentAnimation(0);
				m_P1Projectiles[i]->m_pProjTex->SetAnimate(true);
				m_P1Projectiles[i]->m_pProjTex->setVel(0.0, 0.0);
				m_P1Projectiles[i]->m_pProjTex->TurnOnCollider(false, false);
				//m_P1Projectiles.erase(m_P1Projectiles.begin() + i);
				break;
			}

			if (m_P1Projectiles[i]->m_pProjTex->IsProjectileDone() || m_P1Projectiles[i]->m_pProjTex->GetAnimDone())
			{
				//std::cout << "Erased a P1 Projectile -- OFF SCREEN" << std::endl;
				m_P1Projectiles.erase(m_P1Projectiles.begin() + i);
			}
		}
	}

	if (m_P2Projectiles.size() > 0)
	{
		for (int i = 0; i < m_P2Projectiles.size(); i++)
		{
			if (*StateMachine::pGameState != GameState::PAUSED &&
				*StateMachine::pGameState != GameState::P1WIN &&
				*StateMachine::pGameState != GameState::P2WIN)
			{
				m_P2Projectiles[i]->m_pProjTex->Update(m_dt);
			}
			m_P2Projectiles[i]->m_pProjTex->Render();
			if (Collision::CircleVsCircle(m_P2Projectiles[i]->m_pProjTex->GetCircleCollider(), m_pPlayer1->GetCircleCollider()) && m_P2Projectiles[i]->m_pProjTex->UsingCircleCollider())
			{
				//std::cout << "Erased a P2 Projectile --COLLISION" << std::endl;
				if (*StateMachine::pSoundState == SoundState::SOUNDON){ Mix_PlayChannel(-1, m_pP1Hit, 0); }

				m_pPlayer1->GetSmaller();
				m_P2Projectiles[i]->m_pProjTex->SetCurrentAnimation(0);
				m_P2Projectiles[i]->m_pProjTex->SetAnimate(true);
				m_P2Projectiles[i]->m_pProjTex->setVel(0.0, 0.0);
				m_P2Projectiles[i]->m_pProjTex->TurnOnCollider(false, false);
				
				//m_P2Projectiles.erase(m_P2Projectiles.begin() + i);
				break;
			}
			if (m_P2Projectiles[i]->m_pProjTex->IsProjectileDone() || m_P2Projectiles[i]->m_pProjTex->GetAnimDone())
			{
				//std::cout << "Erased a P2 Projectile  -- OFF SCREEN" << std::endl;
				m_P2Projectiles.erase(m_P2Projectiles.begin() + i);
			}
		}
	}

}

void SDLHelper::UpdateSlime()
{
	if (!m_RSlime.empty())
	{
		for (unsigned int i = 0; i < m_RSlime.size(); ++i)
		{
			m_RSlime[i]->Update(m_dt);
			m_RSlime[i]->m_pSlimeTex->Render();
			if (m_RSlime[i]->getDead())
			{
				m_RSlime.erase(m_RSlime.begin() + i);
			}
		}
	}

	if (!m_LSlime.empty())
	{
		for (unsigned int i = 0; i < m_LSlime.size(); ++i)
		{
			m_LSlime[i]->Update(m_dt);
			m_LSlime[i]->m_pSlimeTex->Render();
			if (m_LSlime[i]->getDead())
			{
				m_LSlime.erase(m_LSlime.begin() + i);
			}
		}
	}
}





//HANDLING FUNCTIONS**************************************************

void SDLHelper::HandleEvents() //Returns true if Quit is Clicked
{
	SDL_Event e; //Event handler
	//Handle events on queue
	while (SDL_PollEvent(&e) != 0)
	{
		m_pMyWindow->handleEvent(e);
		if (e.type == SDL_QUIT)
		{
			m_done = true;
		}
		else if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) //KEYBOARD EVENTS
		{
			if (*StateMachine::pGameState == GameState::GAMEON &&
				*StateMachine::pLoadedState == LoadedState::GAMEON ||
				*StateMachine::pGameState == GameState::PAUSED &&
				*StateMachine::pLoadedState == LoadedState::PAUSED)
			{
				KeyBoardHandler(e);
			}
		}
		else if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP || e.type == SDL_MOUSEWHEEL) //MOUSE EVENTS
		{
			MouseHandler(e);
		}
	}
}

void SDLHelper::KeyBoardHandler(SDL_Event &e)
{
	if (e.type == SDL_KEYDOWN) //KEY DOWN EVENTS
	{
		switch (e.key.keysym.sym)
		{
			//PLAYER 1
		case SDLK_UP:
			m_pPlayer1->addForce("up");
			break;

		case SDLK_DOWN:
			m_pPlayer1->addForce("down");
			break;

		case SDLK_LEFT:
			m_pPlayer1->addForce("left");
			break;

		case SDLK_RIGHT:
			m_pPlayer1->addForce("right");
			break;

		case SDLK_SPACE:
			if (*StateMachine::pGameState == GameState::GAMEON && *StateMachine::pLoadedState == LoadedState::GAMEON)
			{
				if (m_pPlayer1->getCurrAmmo() > 0 && *StateMachine::pGameState != GameState::PAUSED)
				{
					SpawnProjectile(true, false);
					m_pPlayer1->decreaseCurrAmmo();
				}
			}
			break;

			//PLAYER 2
		case SDLK_w:
			//m_pPlayer1->addForce("up");
			break;
		case SDLK_a:
			//m_pPlayer1->addForce("left");
			break;
		case SDLK_s:
			//m_pPlayer1->addForce("down");
			break;
		case SDLK_d:
			//m_pPlayer1->addForce("right");
			break;
		case SDLK_LCTRL:
			break;
		default:
			break;
		}
	}
	else if (e.type == SDL_KEYUP) //KEY UP EVENTS
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_UP:
			//m_pPlayer1->SetAnimate(false);
			m_pPlayer1->removeForce("up");
			break;

		case SDLK_DOWN:
			//m_pPlayer1->SetAnimate(false);
			m_pPlayer1->removeForce("down");
			break;

		case SDLK_LEFT:
			//m_pPlayer1->SetAnimate(false);
			m_pPlayer1->removeForce("left");
			break;

		case SDLK_RIGHT:
			//m_pPlayer1->SetAnimate(false);
			m_pPlayer1->removeForce("right");
			break;
		case SDLK_p:
			if (*StateMachine::pGameState != GameState::PAUSED)
			{
				*StateMachine::pGameState = GameState::PAUSED;
			}
			else
			{
				*StateMachine::pGameState = GameState::GAMEON;
				*StateMachine::pLoadedState = LoadedState::GAMEON;
			}
		case SDLK_SPACE:
			break;

		case SDLK_w:
			//m_pPlayer1->removeForce("up");
			break;
		case SDLK_a:
			//m_pPlayer1->removeForce("left");
			break;
		case SDLK_s:
			//m_pPlayer1->removeForce("down");
			break;
		case SDLK_d:
			//m_pPlayer1->removeForce("right");
			break;
		case SDLK_LCTRL:
			break;

		default:
			break;
		}
	}
}

void SDLHelper::MouseHandler(SDL_Event &e)
{
	SDL_GetMouseState(&m_MouseX, &m_MouseY);

	if (*StateMachine::pGameState == GameState::GAMEON && *StateMachine::pLoadedState == LoadedState::GAMEON)
	{
		m_pPlayer2->SetMousePos(m_MouseX, m_MouseY);
	}

	//std::cout << "Mouse X: " << x << " Mouse Y: " << y << std::endl;	//CHECK BOUNDS OF WHAT MOUSE IS IN ...

	switch (e.type)
	{
	case SDL_MOUSEMOTION:

		break;

	case SDL_MOUSEBUTTONDOWN:
		if (*StateMachine::pGameState == GameState::GAMEON && *StateMachine::pLoadedState == LoadedState::GAMEON)
		{
			if (m_pPlayer2->getCurrAmmo() > 0 && *StateMachine::pGameState != GameState::PAUSED && *StateMachine::pGameMode == GameMode::PVP)
			{
				SpawnProjectile(false, true);
				m_pPlayer2->decreaseCurrAmmo();
			}
		}

		if (*StateMachine::pGameState == GameState::MAINMENU &&			*StateMachine::pLoadedState == LoadedState::MAINMENU ||
		   *StateMachine::pGameState == GameState::P2WIN &&				*StateMachine::pLoadedState == LoadedState::P2WIN || 
		   *StateMachine::pGameState == GameState::P1WIN &&				*StateMachine::pLoadedState == LoadedState::P1WIN ||
		   *StateMachine::pGameState == GameState::CHARACTERSELECT &&	*StateMachine::pLoadedState == LoadedState::CHARACTERSELECT ||
		   *StateMachine::pGameState == GameState::PAUSED &&			*StateMachine::pLoadedState == LoadedState::PAUSED)
		{
			if (m_Buttons.size() > 0)
			{
				for (int i = 0; i < m_Buttons.size(); i++)
				{
					m_Buttons[i]->m_pButtonTex->m_ClickDown = true;
				}
			}
		}
		//std::cout << "Mouse CLICK DOWN "<< std::endl;
		break;

	case SDL_MOUSEBUTTONUP:

		if (*StateMachine::pGameState == GameState::MAINMENU &&			*StateMachine::pLoadedState == LoadedState::MAINMENU || 
			*StateMachine::pGameState == GameState::P2WIN &&			*StateMachine::pLoadedState == LoadedState::P2WIN || 
			*StateMachine::pGameState == GameState::P1WIN &&			*StateMachine::pLoadedState == LoadedState::P1WIN ||
			*StateMachine::pGameState == GameState::CHARACTERSELECT &&	*StateMachine::pLoadedState == LoadedState::CHARACTERSELECT || 
			*StateMachine::pGameState == GameState::PAUSED &&			*StateMachine::pLoadedState == LoadedState::PAUSED)
		{
			if (m_Buttons.size() > 0)
			{
				for (int i = 0; i < m_Buttons.size(); i++)
				{
					m_Buttons[i]->m_pButtonTex->m_ClickDown = false;
					if (m_Buttons[i]->m_pButtonTex->m_Clicked && m_Buttons[i]->m_pButtonTex->m_name == "playButt")
					{
						*StateMachine::pGameState = GameState::CHARACTERSELECT;
					}
					else if (m_Buttons[i]->m_pButtonTex->m_Clicked && m_Buttons[i]->m_pButtonTex->m_name == "retryButt")
					{
						*StateMachine::pGameState = GameState::GAMEON;
						*StateMachine::pLoadedState = LoadedState::NONE;
					}
					else if (m_Buttons[i]->m_pButtonTex->m_Clicked && m_Buttons[i]->m_pButtonTex->m_name == "quitButt")
					{
						*StateMachine::pGameState = GameState::MAINMENU;
						if (*StateMachine::pMusicState == MusicState::MUSICON){ Mix_PlayMusic(m_pSong1, -1); }
						
					}
					else if (m_Buttons[i]->m_pButtonTex->m_Clicked && m_Buttons[i]->m_pButtonTex->m_name == "colourButt")
					{
						if (GameEntity::m_P1color == "")
						{
							GameEntity::m_P1color = m_Buttons[i]->m_pButtonTex->m_color;
							m_Buttons.erase(m_Buttons.begin() + i);
							continue;
						}
						if (GameEntity::m_P2color == "")
						{
							GameEntity::m_P2color = m_Buttons[i]->m_pButtonTex->m_color;
							m_Buttons.erase(m_Buttons.begin() + i);
						}
						
					}
					else if (m_Buttons[i]->m_pButtonTex->m_Clicked && m_Buttons[i]->m_pButtonTex->m_name == "soundButt")
					{
						if (*StateMachine::pSoundState == SoundState::SOUNDON)
						{
							*StateMachine::pSoundState = SoundState::SOUNDOFF;
							*StateMachine::pLoadedState = LoadedState::NONE;
							
						}
						else
						{
							*StateMachine::pSoundState = SoundState::SOUNDON;
							*StateMachine::pLoadedState = LoadedState::NONE;
							
						}
					}
					else if (m_Buttons[i]->m_pButtonTex->m_Clicked && m_Buttons[i]->m_pButtonTex->m_name == "musicButt")
					{
						if (*StateMachine::pMusicState == MusicState::MUSICON)
						{
							*StateMachine::pMusicState = MusicState::MUSICOFF;
							*StateMachine::pLoadedState = LoadedState::NONE;
							Mix_PauseMusic();
						}
						else
						{
							*StateMachine::pMusicState = MusicState::MUSICON;
							*StateMachine::pLoadedState = LoadedState::NONE;
							Mix_ResumeMusic();
						}
					}
					else if (m_Buttons[i]->m_pButtonTex->m_Clicked && m_Buttons[i]->m_pButtonTex->m_name == "modeButt")
					{
						if (*StateMachine::pGameMode == GameMode::PVP)
						{
							*StateMachine::pGameMode = GameMode::PVCPU;
							*StateMachine::pLoadedState = LoadedState::NONE;
							
						}
						else
						{
							*StateMachine::pGameMode = GameMode::PVP;
							*StateMachine::pLoadedState = LoadedState::NONE;
							
						}
					}

				}
			}
		}
		//std::cout << "Mouse CLICK UP " << std::endl;
		break;
	}

}






//RENDERING FUNCTIONS**************************************************
void SDLHelper::SetRenderColor(std::string color)
{
	if (color == "pink"){ SDL_SetRenderDrawColor(m_pRenderer, 0xFF, 0x99, 0xFF, 0xFF); }
	else if (color == "white"){ SDL_SetRenderDrawColor(m_pRenderer, 0xFF, 0xFF, 0xFF, 0xFF); }
	else if (color == "black"){ SDL_SetRenderDrawColor(m_pRenderer, 0x00, 0x00, 0x00, 0xFF); }
	else if (color == "red"){ SDL_SetRenderDrawColor(m_pRenderer, 0xFF, 0x00, 0x00, 0xFF); }
	else if (color == "green"){ SDL_SetRenderDrawColor(m_pRenderer, 0x00, 0xFF, 0x00, 0xFF); }
	else if (color == "mintgreen"){ SDL_SetRenderDrawColor(m_pRenderer, 0x00, 0xFF, 0x9A, 0xFF); }
	else if (color == "darkgreen"){ SDL_SetRenderDrawColor(m_pRenderer, 0x00, 0x66, 0x00, 0xFF); }
	else if (color == "blue"){ SDL_SetRenderDrawColor(m_pRenderer, 0x00, 0x00, 0xFF, 0xFF); }
	else if (color == "royalblue"){ SDL_SetRenderDrawColor(m_pRenderer, 0x00, 0x00, 0x66, 0xFF); }
	else if (color == "babyblue"){ SDL_SetRenderDrawColor(m_pRenderer, 0x00, 0xFF, 0xFF, 0xFF); }
	else if (color == "orange"){ SDL_SetRenderDrawColor(m_pRenderer, 0xFF, 0x77, 0x00, 0xFF); }
	else if (color == "yellow"){ SDL_SetRenderDrawColor(m_pRenderer, 0xFF, 0xFF, 0x00, 0xFF); }
	else if (color == "gold"){ SDL_SetRenderDrawColor(m_pRenderer, 0xFF, 0xBB, 0x00, 0xFF); }
	else if (color == "fuchsia"){ SDL_SetRenderDrawColor(m_pRenderer, 0x66, 0x00, 0x66, 0xFF); }
	else if (color == "purple"){ SDL_SetRenderDrawColor(m_pRenderer, 0x66, 0x00, 0x66, 0xFF); }
	else if (color == "brown"){ SDL_SetRenderDrawColor(m_pRenderer, 0x66, 0x33, 0x00, 0xFF); }
	else if (color == "darkgray"){ SDL_SetRenderDrawColor(m_pRenderer, 0x66, 0x66, 0x66, 0xFF); }
	else if (color == "lightgray"){ SDL_SetRenderDrawColor(m_pRenderer, 0x99, 0x99, 0x99, 0xFF); }
	else if (color == "halfblack"){ SDL_SetRenderDrawColor(m_pRenderer, 0x00, 0x00, 0x00, 0x99); }

	/*0xFF, 0x99, 0xFF, 0xFF*///Pink
	/*0xFF, 0xFF, 0xFF, 0xFF*///White
	/*0x00, 0x00, 0x00, 0xFF*///Black
	/*0xFF, 0x00, 0x00, 0xFF*///Red
	/*0x00, 0xFF, 0x00, 0xFF*///Green
	/*0x00, 0xFF, 0x9A, 0xFF*///MintGreen
	/*0x00, 0x66, 0x00, 0xFF*///DarkGreen
	/*0x00, 0x00, 0xFF, 0xFF*///Blue
	/*0x00, 0x00, 0x66, 0xFF*///RoyalBlue
	/*0x00, 0xFF, 0xFF, 0xFF*///BabyBlue
	/*0xFF, 0x77, 0x00, 0xFF*///Orange
	/*0xFF, 0xFF, 0x00, 0xFF*///Yellow
	/*0xFF, 0xBB, 0x00, 0xFF*///Gold
	/*0xFF, 0x00, 0xFF, 0xFF*///Fuchsia
	/*0x66, 0x00, 0x66, 0xFF*///Purple
	/*0x66, 0x33, 0x00, 0xFF*///Brown
	/*0x66, 0x66, 0x66, 0xFF*///DarkGray
	/*0x99, 0x99, 0x99, 0xFF*///LightGray
}

void SDLHelper::DrawRect(int x, int y, int w, int h, std::string color)
{
	SDL_Rect rect = { x, y, w, h };
	SetRenderColor(color);
	SDL_RenderFillRect(m_pRenderer, &rect);
}

void SDLHelper::DrawRectOL(int x, int y, int w, int h, std::string color)
{
	SDL_Rect rect = { x, y, w, h };
	SetRenderColor(color);
	SDL_RenderDrawRect(m_pRenderer, &rect);
}

void SDLHelper::DrawLine(int X1, int Y1, int X2, int Y2, std::string color)
{
	SetRenderColor(color);
	SDL_RenderDrawLine(m_pRenderer, X1, Y1, X2, Y2);
}

void SDLHelper::DrawPoint(int x, int y, std::string color)
{
	SetRenderColor(color);
	SDL_RenderDrawPoint(m_pRenderer, x, y);
}








//SCREENS**************************************************

void SDLHelper::ShowMainMenu()
{
	if (*StateMachine::pLoadedState != LoadedState::MAINMENU){ LoadMainMenu(); }

	

	SDL_Rect rec = { 0, 0, MyWindow::m_Width, MyWindow::m_Height };
	SDL_RenderCopy(m_pRenderer, m_pbackground, NULL, &rec);



	if (m_Buttons.size() > 0)
	{
		for (int i = 0; i < m_Buttons.size(); i++)
		{
			m_Buttons[i]->m_pButtonTex->SetMousePos(m_MouseX, m_MouseY);
			m_Buttons[i]->m_pButtonTex->Update(m_dt);
			m_Buttons[i]->m_pButtonTex->Render();
		}
	}
}

void SDLHelper::ShowPaused()
{
	if (*StateMachine::pLoadedState != LoadedState::PAUSED){ LoadPaused(); }

	DrawRect(0, 0, MyWindow::getWidth(), MyWindow::getHeight(), "halfblack");
	ShowGameOn();

		if (m_Buttons.size() > 0)
	{
		for (int i = 0; i < m_Buttons.size(); i++)
		{
			m_Buttons[i]->m_pButtonTex->SetMousePos(m_MouseX, m_MouseY);
			m_Buttons[i]->m_pButtonTex->Update(m_dt);
			m_Buttons[i]->m_pButtonTex->Render();
		}
	}


	m_pPaused->setPos(MyWindow::getWidth() / 2 - 150, MyWindow::getHeight() / 2 - 50);
	m_pPaused->Render();

}

void SDLHelper::ShowP1Win()
{
	if (*StateMachine::pLoadedState != LoadedState::P1WIN){ LoadP1Win(); }
	DrawRect(0, 0, MyWindow::getWidth(), MyWindow::getHeight(), "halfblack");
	ShowGameOn();
	if (m_Buttons.size() > 0)
	{
		for (int i = 0; i < m_Buttons.size(); i++)
		{
			m_Buttons[i]->m_pButtonTex->SetMousePos(m_MouseX, m_MouseY);
			m_Buttons[i]->m_pButtonTex->Update(m_dt);
			m_Buttons[i]->m_pButtonTex->Render();
		}
	}
}

void SDLHelper::ShowP2Win()
{
	if (*StateMachine::pLoadedState != LoadedState::P2WIN){ LoadP2Win(); }
	DrawRect(0, 0, MyWindow::getWidth(), MyWindow::getHeight(), "halfblack");
	ShowGameOn();
	if (m_Buttons.size() > 0)
	{
		for (int i = 0; i < m_Buttons.size(); i++)
		{
			m_Buttons[i]->m_pButtonTex->SetMousePos(m_MouseX, m_MouseY);
			m_Buttons[i]->m_pButtonTex->Update(m_dt);
			m_Buttons[i]->m_pButtonTex->Render();
		}
	}

}

void SDLHelper::ShowCharSelection()
{


	//CHECK IF LOADED
	if (*StateMachine::pLoadedState != LoadedState::CHARACTERSELECT){ LoadCharSelection(); }

	SDL_Rect rec = { 0, 0, MyWindow::m_Width, MyWindow::m_Height };
	SDL_RenderCopy(m_pRenderer, m_pbackground, NULL, &rec);

	if (m_Buttons.size() > 0)
	{
		for (int i = 0; i < m_Buttons.size(); i++)
		{
			if (m_Buttons[i]->m_pButtonTex->m_name == "Player1" && GameEntity::m_P1color != "")
			{
				m_Buttons[i]->m_pButtonTex->SetName("Player2");
				if (*StateMachine::pGameMode == GameMode::PVCPU){ m_Buttons[i]->m_pButtonTex->LoadFile("Pics/CPU.png"); }
				else{ m_Buttons[i]->m_pButtonTex->LoadFile("Pics/player2.png"); }
			}
			if (m_Buttons[i]->m_pButtonTex->m_name == "Player2" && GameEntity::m_P2color != "")
			{
				*StateMachine::pGameState = GameState::GAMEON;
			}

			m_Buttons[i]->m_pButtonTex->SetMousePos(m_MouseX, m_MouseY);
			m_Buttons[i]->m_pButtonTex->Update(m_dt);
			m_Buttons[i]->m_pButtonTex->Render();
		}
	}

}

void SDLHelper::ShowGameOn()
{
	//CHECK IF LOADED
	if (*StateMachine::pLoadedState != LoadedState::GAMEON && *StateMachine::pLoadedState != LoadedState::PAUSED && *StateMachine::pLoadedState != LoadedState::P1WIN && *StateMachine::pLoadedState != LoadedState::P2WIN){ LoadGameOn(); }
	if (*StateMachine::pGameState != GameState::PAUSED && *StateMachine::pGameState != GameState::P1WIN && *StateMachine::pGameState != GameState::P2WIN)
	{
		m_RPickUpSpawnTimeElapsed += m_dt;
		m_LPickUpSpawnTimeElapsed += m_dt;
		m_RSlimeSpawnTimeElapsed += m_dt;
		m_LSlimeSpawnTimeElapsed += m_dt;




		if (m_RPickUpSpawnTimeElapsed > m_RPickUpSpawnTime)
		{
			SpawnRPickUp();
			m_RPickUpSpawnTimeElapsed = 0;
			m_RPickUpSpawnTime = m_RG(6) + 1;
		}
		if (m_LPickUpSpawnTimeElapsed > m_LPickUpSpawnTime)
		{
			SpawnLPickUp();
			m_LPickUpSpawnTimeElapsed = 0;
			m_LPickUpSpawnTime = m_RG(6) + 1;
		}
		if (m_LSlimeSpawnTimeElapsed > m_LSlimeSpawnTime)
		{
			SpawnLSlime();
			m_LSlimeSpawnTimeElapsed = 0;
		}
		if (m_RSlimeSpawnTimeElapsed > m_RSlimeSpawnTime)
		{
			SpawnRSlime();
			m_RSlimeSpawnTimeElapsed = 0;

		}

		m_pPlayer2->Update(m_dt);
		m_pPlayer1->Update(m_dt);
		UpdateSlime();


		
	}
	
	
	//BACKGROUND DRAWING
	SetRenderColor("white");
	SDL_Rect rec = { 0, 0, MyWindow::m_Width, MyWindow::m_Height };
	SDL_SetTextureColorMod(m_pP1BG, m_pPlayer1->getColor().r, m_pPlayer1->getColor().g, m_pPlayer1->getColor().b);
	SDL_SetTextureColorMod(m_pP2BG, m_pPlayer2->getColor().r, m_pPlayer2->getColor().g, m_pPlayer2->getColor().b);
	SDL_RenderCopy(m_pRenderer, m_pP1BG, NULL, &rec);
	SDL_RenderCopy(m_pRenderer, m_pP2BG, NULL, &rec);

	SDL_RenderCopy(m_pRenderer, m_pbackground, NULL, &rec);
	
	//PLAYER 1
	m_pPlayer1->Render();
	//PLAYER 2	
	m_pPlayer2->Render();
	UpdateProjectiles();
	UpdatePickUp();


	//UI STUFF 

	m_pInGameSizeP1->LoadTextFile(m_pFont2, "Size: " + std::to_string(m_pPlayer1->getWidth()) + "mm", "black");
	m_pInGameSizeP2->LoadTextFile(m_pFont2, "Size: " + std::to_string(m_pPlayer2->getWidth()) + "mm", "black");

	m_pInGameAmmoP1->LoadTextFile(m_pFont2, "Amoeba: " + std::to_string(m_pPlayer1->getCurrAmmo()), "black");
	m_pInGameAmmoP2->LoadTextFile(m_pFont2, "Amoeba: " + std::to_string(m_pPlayer2->getCurrAmmo()), "black");

	//RE-POSITION ... if window size changes
	m_pInGameTitle->setPos((MyWindow::m_Width / 2) - (m_pInGameTitle->getWidth() / 2), 0);
	m_pInGameSizeP1->setPos((MyWindow::m_Width / 2) - (MyWindow::m_Width / 4 + m_pInGameSizeP1->getWidth()), 10);
	m_pInGameSizeP2->setPos((MyWindow::m_Width / 2) + (MyWindow::m_Width / 4), 10);
	m_pInGameAmmoP1->setPos((MyWindow::m_Width / 2) - (MyWindow::m_Width / 4 + m_pInGameAmmoP1->getWidth()), MyWindow::m_Height - 50);
	m_pInGameAmmoP2->setPos((MyWindow::m_Width / 2) + (MyWindow::m_Width / 4), MyWindow::m_Height - 50);


	m_pInGameTitle->Render(); //RENDER THE TITLE TEXT
	m_pInGameSizeP1->Render();
	m_pInGameSizeP2->Render();
	m_pInGameAmmoP1->Render();
	m_pInGameAmmoP2->Render();

	if (*StateMachine::pCPU == CPUState::SHOOTENEMY)
	{
		SpawnProjectile(false, true);
		*StateMachine::pCPU = CPUState::RANDOMDESTINATION;
		m_pPlayer2->decreaseCurrAmmo();
	}


	if (m_pPlayer1->getWidth() <= 0)
	{
		m_pPlayer1->SetScale(0, 0);
		*StateMachine::pGameState = GameState::P2WIN;
	}
	else if (m_pPlayer2->getWidth() <= 0)
	{
		m_pPlayer2->SetScale(0, 0);
		*StateMachine::pGameState = GameState::P1WIN;
	}
}



//LOAD SCREENS**********************************************
void SDLHelper::LoadMainMenu()
{
	
	if (*StateMachine::pMusicState == MusicState::MUSICON && !Mix_PlayingMusic())
	{ 
		//Mix_PlayMusic(m_pSong1, -1);
		Mix_FadeInMusic(m_pSong1, -1, 3000);
	}

	GameEntity::m_P1color = "";
	GameEntity::m_P2color = "";
	m_pbackground = loadTexture("Pics/mainmenu.png");

	if (!m_Buttons.empty())	{m_Buttons.clear();}
	

	//ADD BUTTONS
	Button* playBut  = new Button(MyWindow::m_Width / 2, MyWindow::m_Height / 2, 250, 180, 0, m_pRenderer, "playButt", 20);
	Button* soundBut = new Button(160, MyWindow::m_Height / 2 + 50 ,     250, 100, 0, m_pRenderer, "soundButt", 20);
	Button* musicBut = new Button(160, MyWindow::m_Height / 2 + 130, 250, 100, 0, m_pRenderer, "musicButt", 20);
	Button* ModeBut   = new Button(170, MyWindow::m_Height / 2 - 30, 350, 150, 0, m_pRenderer, "modeButt", 20);


	playBut->m_pButtonTex->LoadFile("Pics/playButt.png");


	if (*StateMachine::pSoundState == SoundState::SOUNDON){ soundBut->m_pButtonTex->LoadFile("Pics/soundOnButt.png"); }
	else
	{
		soundBut->m_pButtonTex->LoadFile("Pics/soundOffButt.png");
	}

	if (*StateMachine::pMusicState == MusicState::MUSICON){ musicBut->m_pButtonTex->LoadFile("Pics/musicOnButt.png"); }
	else
	{
		musicBut->m_pButtonTex->LoadFile("Pics/musicOffButt.png");
	}
	if (*StateMachine::pGameMode == GameMode::PVP){ ModeBut->m_pButtonTex->LoadFile("Pics/PvPButt.png"); }
	else
	{
		ModeBut->m_pButtonTex->LoadFile("Pics/PvCPUButt.png");
	}

	m_Buttons.push_back(ModeBut);
	m_Buttons.push_back(playBut);
	m_Buttons.push_back(soundBut);
	m_Buttons.push_back(musicBut);
	
	*StateMachine::pLoadedState = LoadedState::MAINMENU;
}

void SDLHelper::LoadPaused()
{
	m_pPaused->LoadTextFile(m_pFont2, "PAUSED ", "royalblue");

	Button* quitBut = new Button(MyWindow::getWidth() / 2 - 200, MyWindow::getHeight() / 2 + 150, 300, 150, 0, m_pRenderer, "quitButt", 20);
	Button* retryBut = new Button(MyWindow::getWidth() / 2 + 200, MyWindow::getHeight() / 2 + 150, 300, 150, 0, m_pRenderer, "retryButt", 20);

	if (!m_Buttons.empty()){ m_Buttons.clear(); }


	quitBut->m_pButtonTex->LoadFile("Pics/quitButt.png");
	retryBut->m_pButtonTex->LoadFile("Pics/restartButt.png");

	m_Buttons.push_back(quitBut);
	m_Buttons.push_back(retryBut);


	*StateMachine::pLoadedState = LoadedState::PAUSED;
}

void SDLHelper::LoadP1Win()
{
	Button* winBut = new Button(MyWindow::getWidth() / 2, MyWindow::getHeight() / 2 - 50, 600, 300, 0, m_pRenderer, "title", 20);
	Button* quitBut = new Button(MyWindow::getWidth() / 2 - 200 , MyWindow::getHeight() / 2 + 150, 300, 150, 0, m_pRenderer, "quitButt", 20);
	Button* retryBut = new Button(MyWindow::getWidth() / 2 + 200, MyWindow::getHeight() / 2 + 150, 300, 150, 0, m_pRenderer, "retryButt", 20);

	if (!m_Buttons.empty()){m_Buttons.clear();}

	winBut->m_pButtonTex->LoadFile("Pics/P1Wins.png");
	winBut->m_pButtonTex->UseMouseEffects(false,0);
	quitBut->m_pButtonTex->LoadFile("Pics/quitButt.png");
	retryBut->m_pButtonTex->LoadFile("Pics/restartButt.png");
	m_Buttons.push_back(winBut);
	m_Buttons.push_back(quitBut);
	m_Buttons.push_back(retryBut);
	*StateMachine::pLoadedState = LoadedState::P1WIN;
}

void SDLHelper::LoadP2Win()
{

	Button* winBut = new Button(MyWindow::getWidth() / 2 , MyWindow::getHeight() / 2 - 50, 600, 300, 0, m_pRenderer, "title", 20);
	Button* quitBut = new Button(MyWindow::getWidth() / 2 - 200, MyWindow::getHeight() / 2 + 150, 300, 150, 0, m_pRenderer, "quitButt", 20);
	Button* retryBut = new Button(MyWindow::getWidth() / 2 + 200, MyWindow::getHeight() / 2 + 150, 300, 150, 0, m_pRenderer, "retryButt", 20);
	
	if (!m_Buttons.empty())	{m_Buttons.clear();}

	if (*StateMachine::pGameMode == GameMode::PVCPU)
	{
		winBut->m_pButtonTex->LoadFile("Pics/CPUWins.png");
	}
	else
	{
		winBut->m_pButtonTex->LoadFile("Pics/P2Wins.png");
	}
	
	winBut->m_pButtonTex->UseMouseEffects(false,0);
	quitBut->m_pButtonTex->LoadFile("Pics/quitButt.png");
	retryBut->m_pButtonTex->LoadFile("Pics/restartButt.png");
	m_Buttons.push_back(winBut);
	m_Buttons.push_back(quitBut);
	m_Buttons.push_back(retryBut);
	*StateMachine::pLoadedState = LoadedState::P2WIN;
}

void SDLHelper::LoadCharSelection()
{
	if (!m_Buttons.empty())	{ m_Buttons.clear(); }

	Button* playBut0 = new Button(MyWindow::m_Width / 2, MyWindow::m_Height / 2 - 120, 450, 220, 0, m_pRenderer, "Player1", 20);
	playBut0->m_pButtonTex->LoadFile("Pics/player1.png");
	playBut0->m_pButtonTex->UseMouseEffects(false, 10);



	//ADD BUTTONS
	Button* playBut = new Button(100, MyWindow::m_Height / 2, 128, 128, 0, m_pRenderer, "colourButt", 20);
	playBut->m_pButtonTex->LoadFile("Pics/whiteButt.png");
	playBut->m_pButtonTex->SetColor("white");
	m_Buttons.push_back(playBut);

	Button* playBut2 = new Button(250, MyWindow::m_Height / 2, 128, 128, 0, m_pRenderer, "colourButt", 20);
	playBut2->m_pButtonTex->LoadFile("Pics/yellowButt.png");
	playBut2->m_pButtonTex->SetColor("yellow");
	m_Buttons.push_back(playBut2);

	Button* playBut3 = new Button(400, MyWindow::m_Height / 2, 128, 128, 0, m_pRenderer, "colourButt", 20);
	playBut3->m_pButtonTex->LoadFile("Pics/redButt.png");
	playBut3->m_pButtonTex->SetColor("red");
	m_Buttons.push_back(playBut3);

	Button* playBut4 = new Button(550, MyWindow::m_Height / 2, 128, 128, 0, m_pRenderer, "colourButt", 20);
	playBut4->m_pButtonTex->LoadFile("Pics/rblueButt.png");
	playBut4->m_pButtonTex->SetColor("royalblue");
	m_Buttons.push_back(playBut4);

	Button* playBut5 = new Button(700, MyWindow::m_Height / 2, 128, 128, 0, m_pRenderer, "colourButt", 20);
	playBut5->m_pButtonTex->LoadFile("Pics/purpleButt.png");
	playBut5->m_pButtonTex->SetColor("purple");
	m_Buttons.push_back(playBut5);

	Button* playBut6 = new Button(850, MyWindow::m_Height / 2, 128, 128, 0, m_pRenderer, "colourButt", 20);
	playBut6->m_pButtonTex->LoadFile("Pics/pinkButt.png");
	playBut6->m_pButtonTex->SetColor("pink");
	m_Buttons.push_back(playBut6);

	//NEXT ROW=========================================================================================

	Button* playBut7 = new Button(100, MyWindow::m_Height / 2 + 100, 128, 128, 0, m_pRenderer, "colourButt", 20);
	playBut7->m_pButtonTex->LoadFile("Pics/orangeButt.png");
	playBut7->m_pButtonTex->SetColor("orange");
	m_Buttons.push_back(playBut7);

	Button* playBut8 = new Button(250, MyWindow::m_Height / 2 + 100, 128, 128, 0, m_pRenderer, "colourButt", 20);
	playBut8->m_pButtonTex->LoadFile("Pics/mintButt.png");
	playBut8->m_pButtonTex->SetColor("mintgreen");
	m_Buttons.push_back(playBut8);

	Button* playBut9 = new Button(400, MyWindow::m_Height / 2 + 100, 128, 128, 0, m_pRenderer, "colourButt", 20);
	playBut9->m_pButtonTex->LoadFile("Pics/greenButt.png");
	playBut9->m_pButtonTex->SetColor("green");
	m_Buttons.push_back(playBut9);

	Button* playBut10 = new Button(550, MyWindow::m_Height / 2 + 100, 128, 128, 0, m_pRenderer, "colourButt", 20);
	playBut10->m_pButtonTex->LoadFile("Pics/grayButt.png");
	playBut10->m_pButtonTex->SetColor("darkgray");
	m_Buttons.push_back(playBut10);

	Button* playBut11 = new Button(700, MyWindow::m_Height / 2 + 100, 128, 128, 0, m_pRenderer, "colourButt", 20);
	playBut11->m_pButtonTex->LoadFile("Pics/goldButt.png");
	playBut11->m_pButtonTex->SetColor("gold");
	m_Buttons.push_back(playBut11);

	Button* playBut12 = new Button(850, MyWindow::m_Height / 2 + 100, 128, 128, 0, m_pRenderer, "colourButt", 20);
	playBut12->m_pButtonTex->LoadFile("Pics/fuchsiaButt.png");
	playBut12->m_pButtonTex->SetColor("fuchsia");
	m_Buttons.push_back(playBut12);

	//NEXT ROW=========================================================================================
	Button* playBut13 = new Button(100, MyWindow::m_Height / 2 + 200, 128, 128, 0, m_pRenderer, "colourButt", 20);
	playBut13->m_pButtonTex->LoadFile("Pics/dgreenButt.png");
	playBut13->m_pButtonTex->SetColor("darkgreen");
	m_Buttons.push_back(playBut13);

	Button* playBut14 = new Button(250, MyWindow::m_Height / 2 + 200, 128, 128, 0, m_pRenderer, "colourButt", 20);
	playBut14->m_pButtonTex->LoadFile("Pics/brownButt.png");
	playBut14->m_pButtonTex->SetColor("brown");
	m_Buttons.push_back(playBut14);

	Button* playBut15 = new Button(400, MyWindow::m_Height / 2 + 200, 128, 128, 0, m_pRenderer, "colourButt", 20);
	playBut15->m_pButtonTex->LoadFile("Pics/blueButt.png");
	playBut15->m_pButtonTex->SetColor("blue");
	m_Buttons.push_back(playBut15);

	Button* playBut16 = new Button(550, MyWindow::m_Height / 2 + 200, 128, 128, 0, m_pRenderer, "colourButt", 20);
	playBut16->m_pButtonTex->LoadFile("Pics/blackButt.png");
	playBut16->m_pButtonTex->SetColor("black");
	m_Buttons.push_back(playBut16);

	Button* playBut17 = new Button(700, MyWindow::m_Height / 2 + 200, 128, 128, 0, m_pRenderer, "colourButt", 20);
	playBut17->m_pButtonTex->LoadFile("Pics/babyButt.png");
	playBut17->m_pButtonTex->SetColor("babyblue");
	m_Buttons.push_back(playBut17);


	m_Buttons.push_back(playBut0); //pushed back last for efficiency .. if in loop ..ya

	m_pbackground = loadTexture("Pics/charmenu.png");

	*StateMachine::pLoadedState = LoadedState::CHARACTERSELECT;
}

void SDLHelper::LoadGameOn()
{
	// 	//SET UP CHARACTER       x   y    w    h  maxS Accel					
	// 	m_pTexture = new Texture(30, 30, 128, 128, 350, 20, "Guy", m_pRenderer, true);
	// 	m_pTexture->LoadFile("Pics/walkGuy.png");				   // Load Up The Full Sprite Sheet
	// 	m_pTexture->SetAnimation(0, 0, 4, 128, 128, 200, true);    // Walk Left		[0]
	// 	m_pTexture->SetAnimation(0, 128, 4, 128, 128, 200, true);  // Walk Right	[1]
	// 	m_pTexture->SetAnimation(0, 256, 4, 128, 128, 200, true);  // Walk ForWard	[2]
	// 	m_pTexture->SetAnimation(0, 384, 4, 128, 128, 200, true);  // Walk Backward	[3]
	// 	m_pTexture->SetAnimation(0, 512, 19, 128, 128, 50, false); // EXPLOSION	[4]

	//COLORS .. SO FAR ****************USE LOWERCASE*****************
	//Pink			//White		//Black			//Red		//Green	
	//DarkGreen		//Blue		//RoyalBlue		//BabyBlue	//MintGreen	
	//Orange		//Yellow	//Gold			//Fuchsia	//Purple
	//Brown			//DarkGray	//LightGray
	//**************************************************************

	if (*StateMachine::pMusicState == MusicState::MUSICON)
	{
		Mix_FadeInMusic(m_pSong2, -1, 10);
		Mix_FadeInMusic(m_pSong2, -1, 10);
	}


	//TITLE
	m_pInGameTitle = new GameEntity(0, 0, 300, 100, 100, 5, "Title", m_pRenderer, false);

	m_pInGameSizeP1 = new GameEntity(0, 0, 180, 40, 0, 0, "P1Size", m_pRenderer, false);
	m_pInGameSizeP2 = new GameEntity(0, 0, 180, 40, 0, 0, "P2Size", m_pRenderer, false);
	m_pInGameAmmoP1 = new GameEntity(0, 0, 180, 40, 0, 0, "P1Ammo", m_pRenderer, false);
	m_pInGameAmmoP2 = new GameEntity(0, 0, 180, 40, 0, 0, "P2Ammo", m_pRenderer, false);

	//PLAYER 1
	//SET UP CHARACTER       x   y    w    h   maxS Accel					
	m_pPlayer1 = new GameEntity((SCREEN_WIDTH / 2) - (SCREEN_WIDTH / 4) - 64, (SCREEN_HEIGHT / 2) - (SCREEN_HEIGHT / 4) + 64, 128, 128, 350, 20, "player1", m_pRenderer, false);
	m_pPlayer1->LoadFile("Pics/blobee.png"); // Load Up The Full Sprite Sheet
	m_pPlayer1->TurnOnCollider(false, true); // Turn on colliders
	m_pPlayer1->RotateToDir(true);			  // Turn on Rotation
	m_pPlayer1->UseKeyForces(true);
	m_pPlayer1->SetColorMod(GameEntity::m_P1color); //COLOUR FOR PLAYER AND BACKDROP BLOB

	if (*StateMachine::pGameMode == GameMode::PVCPU)
	{
		m_pPlayer2 = new GameEntity((SCREEN_WIDTH / 2) + (SCREEN_WIDTH / 4) - 64, (SCREEN_HEIGHT / 2) - (SCREEN_HEIGHT / 4) + 64, 128, 128, 350, 20, "CPU", m_pRenderer, false);
		m_pPlayer2->LoadFile("Pics/blobee.png");  // Load Up The Full Sprite Sheet
		m_pPlayer2->TurnOnCollider(false, true);  // Turn on colliders
		m_pPlayer2->RotateToDir(true);			  // Turn on Rotation
		m_pPlayer2->SetColorMod(GameEntity::m_P2color); //COLOUR FOR PLAYER AND BACKDROP BLOB
		m_pPlayer2->TurnOnAi(true);
		m_pPlayer2->UseGoToPoint(true);
		*StateMachine::pCPU = CPUState::RANDOMDESTINATION;
	}
	else
	{
		//PLAYER 2
		//SET UP CHARACTER       x   y    w    h   maxS Accel					
		m_pPlayer2 = new GameEntity((SCREEN_WIDTH / 2) + (SCREEN_WIDTH / 4) - 64, (SCREEN_HEIGHT / 2) - (SCREEN_HEIGHT / 4) + 64, 128, 128, 350, 20, "player2", m_pRenderer, false);
		m_pPlayer2->LoadFile("Pics/blobee.png");  // Load Up The Full Sprite Sheet
		m_pPlayer2->TurnOnCollider(false, true);  // Turn on colliders
		m_pPlayer2->RotateToDir(true);			  // Turn on Rotation
		m_pPlayer2->UseGoToPoint(true);
		m_pPlayer2->SetColorMod(GameEntity::m_P2color); //COLOUR FOR PLAYER AND BACKDROP BLOB
	}


	if (!m_P1PickUps.empty())
	{
		m_P1PickUps.clear();
	}
	if (!m_P2PickUps.empty())
	{
		m_P2PickUps.clear();
	}
	if (!m_RSlime.empty())
	{
		m_RSlime.clear();
	}
	if (!m_LSlime.empty())
	{
		m_LSlime.clear();
	}
	if (!m_P1Projectiles.empty()) { m_P1Projectiles.clear(); }
	if (!m_P2Projectiles.empty()) { m_P2Projectiles.clear(); }


	//TITLE
	m_pInGameTitle->LoadTextFile(m_pFont2, "Fight !", "royalblue");


	//PLAYER BG 
	m_pP1BG = loadTexture("Pics/p1BG.png");
	m_pP2BG = loadTexture("Pics/p2BG.png");

	m_pbackground = loadTexture("Pics/background.png");

	m_pInGameTitle->setPos((MyWindow::m_Width / 2) - (m_pInGameTitle->getWidth() / 2), 0);
	m_pInGameSizeP1->setPos((MyWindow::m_Width / 2) - (MyWindow::m_Width / 4 + m_pInGameSizeP1->getWidth()), 10);
	m_pInGameSizeP2->setPos((MyWindow::m_Width / 2) + (MyWindow::m_Width / 4), 10);
	m_pInGameAmmoP1->setPos((MyWindow::m_Width / 2) - (MyWindow::m_Width / 4 + m_pInGameAmmoP1->getWidth()), MyWindow::m_Height - 50);
	m_pInGameAmmoP2->setPos((MyWindow::m_Width / 2) + (MyWindow::m_Width / 4),  MyWindow::m_Height-50);
	m_RPickUpSpawnTimeElapsed = 0;
	m_LPickUpSpawnTimeElapsed = 0;

	*StateMachine::pLoadedState = LoadedState::GAMEON;
}












void SDLHelper::close()
{
	//Destroy window
	SDL_DestroyRenderer(m_pRenderer);
	m_pMyWindow->Free();

	//free the sound effects
	Mix_FreeChunk(m_pP1Fire);
	Mix_FreeChunk(m_pP1Hit);

	Mix_FreeChunk(m_pP2Fire);
	Mix_FreeChunk(m_pP2Hit);

	m_pP1Fire = NULL;
	m_pP1Hit = NULL;

	m_pP2Fire = NULL;
	m_pP2Hit = NULL;



	//free the music
	Mix_FreeMusic(m_pSong1);
	Mix_FreeMusic(m_pSong2);

	m_pSong1 = NULL;
	m_pSong2 = NULL;

	//Free global font
	TTF_CloseFont(m_pFont1);
	Mix_CloseAudio();

	m_pFont1 = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
	TTF_Quit();
	Mix_Quit();
}