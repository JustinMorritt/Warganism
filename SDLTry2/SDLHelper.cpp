#include "SDLHelper.h"
//TODO re colour all the colour buttons to be the little dude



SDLHelper::SDLHelper() : m_dt(0.0f), m_done(false)
{
	//SET INITIAL GAMESTATE
	m_pGameState = new GameState;
	*m_pGameState = GameState::MAINMENU;
	if (*m_pGameState == GameState::MAINMENU){ std::cout << "MAINMENU ON" << std::endl; }
	if (*m_pGameState == GameState::GAMEON){ std::cout << "GAMESCREEN ON" << std::endl; }

	//SET LOADED STATE
	m_pLoadedState = new LoadedState;
	*m_pLoadedState = LoadedState::NONE; //ININTIALLY NONE

	//SET MUSIC AND SOUND STATE
	m_pMusicState = new MusicState;
	*m_pMusicState = MusicState::MUSICON;
	m_pSoundState = new SoundState;
	*m_pSoundState = SoundState::SOUNDON;

	//SET GAME MODE
	m_pGameMode = new GameMode;
	*m_pGameMode = GameMode::PVP;

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

	//CREATE THE WINDOW APPLICATION
	//m_pWindow = SDL_CreateWindow("SDL HELPER BY JUSTIN !", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_RENDERER_PRESENTVSYNC | SDL_WINDOW_RESIZABLE);

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

	*m_pMusicState = MusicState::MUSICON;

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

	//std::cout << "GameState = " + std::to_string((int)*m_pGameState) << std::endl;
	//std::cout << "GameState = " + std::to_string((int)*m_pLoadedState) << std::endl;
	HandleEvents();

	//Clear screen
	SDL_RenderClear(m_pRenderer);


	if (*m_pGameState == GameState::MAINMENU){ ShowMainMenu(); }
	if (*m_pGameState == GameState::CHARACTERSELECT){ ShowCharSelection(); }
	if (*m_pGameState == GameState::GAMEON){ ShowGameOn(); }
	if (*m_pGameState == GameState::PAUSED){ ShowPaused(); }
	if (*m_pGameState == GameState::P1WIN){ ShowP1Win(); }
	if (*m_pGameState == GameState::P2WIN){ ShowP2Win(); }

	//Update screen
	SDL_RenderPresent(m_pRenderer);
}








//GAME FUNCTIONS**************************************************

void SDLHelper::SpawnProjectile(bool p1, bool p2)
{
	if (p1)
	{

	
		if (*m_pSoundState == SoundState::SOUNDON){ Mix_PlayChannel(-1, m_pP1Fire, 0); }
		
		
		Projectile* proj = new Projectile(m_pPlayer1->getCenter().x, m_pPlayer1->getCenter().y, m_pPlayer1->getWidth() / 2, m_pPlayer1->getHeight() / 2, m_pPlayer1->m_Roation, GameEntity::m_P1color, m_pRenderer, "P1projectile");
		m_P1Projectiles.push_back(proj);
	}
	else if (p2)
	{
	
		if (*m_pSoundState == SoundState::SOUNDON){ Mix_PlayChannel(-1, m_pP2Fire, 0); }
		
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
			if (*m_pGameState != GameState::PAUSED && *m_pGameState != GameState::P1WIN && *m_pGameState != GameState::P2WIN)
			{
				m_P1Projectiles[i]->m_pProjTex->Update(m_dt);
			}
			m_P1Projectiles[i]->m_pProjTex->Render();

			if (Collision::CircleVsCircle(m_P1Projectiles[i]->m_pProjTex->GetCircleCollider(), m_pPlayer2->GetCircleCollider()) && m_P1Projectiles[i]->m_pProjTex->UsingCircleCollider())
			{
				//std::cout << "Erased a P1 Projectile -- COLLISION" << std::endl;
				if (*m_pSoundState == SoundState::SOUNDON){ Mix_PlayChannel(-1, m_pP2Hit, 0); }

				
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
			if (*m_pGameState != GameState::PAUSED && *m_pGameState != GameState::P1WIN && *m_pGameState != GameState::P2WIN)
			{
				m_P2Projectiles[i]->m_pProjTex->Update(m_dt);
			}
			m_P2Projectiles[i]->m_pProjTex->Render();
			if (Collision::CircleVsCircle(m_P2Projectiles[i]->m_pProjTex->GetCircleCollider(), m_pPlayer1->GetCircleCollider()) && m_P2Projectiles[i]->m_pProjTex->UsingCircleCollider())
			{
				//std::cout << "Erased a P2 Projectile --COLLISION" << std::endl;
				if (*m_pSoundState == SoundState::SOUNDON){ Mix_PlayChannel(-1, m_pP1Hit, 0); }

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
			if (*m_pGameState == GameState::GAMEON && *m_pLoadedState == LoadedState::GAMEON || *m_pGameState == GameState::PAUSED && *m_pLoadedState == LoadedState::PAUSED)
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
			if (*m_pGameState == GameState::GAMEON && *m_pLoadedState == LoadedState::GAMEON)
			{
				if (m_pPlayer1->getCurrAmmo() > 0 && *m_pGameState != GameState::PAUSED)
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
			if (*m_pGameState != GameState::PAUSED)
			{
				*m_pGameState = GameState::PAUSED;
			}
			else
			{
				*m_pGameState = GameState::GAMEON;
				*m_pLoadedState = LoadedState::GAMEON;
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

	if (*m_pGameState == GameState::GAMEON && *m_pLoadedState == LoadedState::GAMEON)
	{
		m_pPlayer2->SetMousePos(m_MouseX, m_MouseY);
	}

	//std::cout << "Mouse X: " << x << " Mouse Y: " << y << std::endl;	//CHECK BOUNDS OF WHAT MOUSE IS IN ...

	switch (e.type)
	{
	case SDL_MOUSEMOTION:

		break;

	case SDL_MOUSEBUTTONDOWN:
		if (*m_pGameState == GameState::GAMEON && *m_pLoadedState == LoadedState::GAMEON)
		{
			if (m_pPlayer2->getCurrAmmo() > 0 && *m_pGameState != GameState::PAUSED)
			{
				SpawnProjectile(false, true);
				m_pPlayer2->decreaseCurrAmmo();
			}
		}

	   if (*m_pGameState == GameState::MAINMENU && *m_pLoadedState == LoadedState::MAINMENU || 
		   *m_pGameState == GameState::P2WIN && *m_pLoadedState == LoadedState::P2WIN || 
		   *m_pGameState == GameState::P1WIN && *m_pLoadedState == LoadedState::P1WIN ||
		   *m_pGameState == GameState::CHARACTERSELECT && *m_pLoadedState == LoadedState::CHARACTERSELECT ||
		   *m_pGameState == GameState::PAUSED && *m_pLoadedState == LoadedState::PAUSED)
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

		if (*m_pGameState == GameState::MAINMENU && *m_pLoadedState == LoadedState::MAINMENU || 
			*m_pGameState == GameState::P2WIN && *m_pLoadedState == LoadedState::P2WIN || 
			*m_pGameState == GameState::P1WIN && *m_pLoadedState == LoadedState::P1WIN ||
			*m_pGameState == GameState::CHARACTERSELECT && *m_pLoadedState == LoadedState::CHARACTERSELECT || 
			*m_pGameState == GameState::PAUSED && *m_pLoadedState == LoadedState::PAUSED)
		{
			if (m_Buttons.size() > 0)
			{
				for (int i = 0; i < m_Buttons.size(); i++)
				{
					m_Buttons[i]->m_pButtonTex->m_ClickDown = false;
					if (m_Buttons[i]->m_pButtonTex->m_Clicked && m_Buttons[i]->m_pButtonTex->m_name == "playButt")
					{
						*m_pGameState = GameState::CHARACTERSELECT;
					}
					else if (m_Buttons[i]->m_pButtonTex->m_Clicked && m_Buttons[i]->m_pButtonTex->m_name == "retryButt")
					{
						*m_pGameState = GameState::GAMEON;
						*m_pLoadedState = LoadedState::NONE;
					}
					else if (m_Buttons[i]->m_pButtonTex->m_Clicked && m_Buttons[i]->m_pButtonTex->m_name == "quitButt")
					{
						*m_pGameState = GameState::MAINMENU;
						
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
						if (*m_pSoundState == SoundState::SOUNDON)
						{
							*m_pSoundState = SoundState::SOUNDOFF;
							*m_pLoadedState = LoadedState::NONE;
							
						}
						else
						{
							*m_pSoundState = SoundState::SOUNDON;
							*m_pLoadedState = LoadedState::NONE;
							
						}
					}
					else if (m_Buttons[i]->m_pButtonTex->m_Clicked && m_Buttons[i]->m_pButtonTex->m_name == "musicButt")
					{
						if (*m_pMusicState == MusicState::MUSICON)
						{
							*m_pMusicState = MusicState::MUSICOFF;
							*m_pLoadedState = LoadedState::NONE;
							Mix_PauseMusic();
						}
						else
						{
							*m_pMusicState = MusicState::MUSICON;
							*m_pLoadedState = LoadedState::NONE;
							Mix_ResumeMusic();
						}
					}
					else if (m_Buttons[i]->m_pButtonTex->m_Clicked && m_Buttons[i]->m_pButtonTex->m_name == "modeButt")
					{
						if (*m_pGameMode == GameMode::PVP)
						{
							*m_pGameMode = GameMode::PVCPU;
							*m_pLoadedState = LoadedState::NONE;
							Mix_PauseMusic();
						}
						else
						{
							*m_pGameMode = GameMode::PVP;
							*m_pLoadedState = LoadedState::NONE;
							Mix_ResumeMusic();
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
	if (*m_pLoadedState != LoadedState::MAINMENU){ LoadMainMenu(); }

	

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
	if (*m_pLoadedState != LoadedState::PAUSED){ LoadPaused(); }

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
	if (*m_pLoadedState != LoadedState::P1WIN){ LoadP1Win(); }
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
	if (*m_pLoadedState != LoadedState::P2WIN){ LoadP2Win(); }
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
	if (*m_pLoadedState != LoadedState::CHARACTERSELECT){ LoadCharSelection(); }

	SDL_Rect rec = { 0, 0, MyWindow::m_Width, MyWindow::m_Height };
	SDL_RenderCopy(m_pRenderer, m_pbackground, NULL, &rec);

	if (m_Buttons.size() > 0)
	{
		for (int i = 0; i < m_Buttons.size(); i++)
		{
			if (m_Buttons[i]->m_pButtonTex->m_name == "Player1" && GameEntity::m_P1color != "")
			{
				m_Buttons[i]->m_pButtonTex->SetName("Player2");
				m_Buttons[i]->m_pButtonTex->LoadFile("Pics/player2.png");
				
			}
			if (m_Buttons[i]->m_pButtonTex->m_name == "Player2" && GameEntity::m_P2color != "")
			{
				*m_pGameState = GameState::GAMEON;
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
	if (*m_pLoadedState != LoadedState::GAMEON && *m_pLoadedState != LoadedState::PAUSED && *m_pLoadedState != LoadedState::P1WIN && *m_pLoadedState != LoadedState::P2WIN){ LoadGameOn(); }
	if (*m_pGameState != GameState::PAUSED && *m_pGameState != GameState::P1WIN && *m_pGameState != GameState::P2WIN)
	{
		m_RPickUpSpawnTimeElapsed += m_dt;
		m_LPickUpSpawnTimeElapsed += m_dt;




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

		m_pPlayer2->Update(m_dt);
		m_pPlayer1->Update(m_dt);
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


	if (m_pPlayer1->getWidth() <= 0)
	{
		m_pPlayer1->SetScale(0, 0);
		*m_pGameState = GameState::P2WIN;
	}
	else if (m_pPlayer2->getWidth() <= 0)
	{
		m_pPlayer2->SetScale(0, 0);
		*m_pGameState = GameState::P1WIN;
	}
}



//LOAD SCREENS**********************************************
void SDLHelper::LoadMainMenu()
{
	
	if (*m_pMusicState == MusicState::MUSICON)
	{ 
		Mix_FadeInMusic(m_pSong1, -1, 10); 
		Mix_FadeInMusic(m_pSong1, -1, 10);
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


	if (*m_pSoundState == SoundState::SOUNDON){ soundBut->m_pButtonTex->LoadFile("Pics/soundOnButt.png"); }
	else
	{
		soundBut->m_pButtonTex->LoadFile("Pics/soundOffButt.png");
	}

	if (*m_pMusicState == MusicState::MUSICON){ musicBut->m_pButtonTex->LoadFile("Pics/musicOnButt.png"); }
	else
	{
		musicBut->m_pButtonTex->LoadFile("Pics/musicOffButt.png");
	}
	if (*m_pGameMode == GameMode::PVP){ ModeBut->m_pButtonTex->LoadFile("Pics/PvPButt.png"); }
	else
	{
		ModeBut->m_pButtonTex->LoadFile("Pics/PvCPUButt.png");
	}

	m_Buttons.push_back(ModeBut);
	m_Buttons.push_back(playBut);
	m_Buttons.push_back(soundBut);
	m_Buttons.push_back(musicBut);
	
	*m_pLoadedState = LoadedState::MAINMENU;
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


	*m_pLoadedState = LoadedState::PAUSED;
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
	*m_pLoadedState = LoadedState::P1WIN;
}

void SDLHelper::LoadP2Win()
{

	Button* winBut = new Button(MyWindow::getWidth() / 2 , MyWindow::getHeight() / 2 - 50, 600, 300, 0, m_pRenderer, "title", 20);
	Button* quitBut = new Button(MyWindow::getWidth() / 2 - 200, MyWindow::getHeight() / 2 + 150, 300, 150, 0, m_pRenderer, "quitButt", 20);
	Button* retryBut = new Button(MyWindow::getWidth() / 2 + 200, MyWindow::getHeight() / 2 + 150, 300, 150, 0, m_pRenderer, "retryButt", 20);
	
	if (!m_Buttons.empty())	{m_Buttons.clear();}

	
	winBut->m_pButtonTex->LoadFile("Pics/P2Wins.png");
	winBut->m_pButtonTex->UseMouseEffects(false,0);
	quitBut->m_pButtonTex->LoadFile("Pics/quitButt.png");
	retryBut->m_pButtonTex->LoadFile("Pics/restartButt.png");
	m_Buttons.push_back(winBut);
	m_Buttons.push_back(quitBut);
	m_Buttons.push_back(retryBut);
	*m_pLoadedState = LoadedState::P2WIN;
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

	*m_pLoadedState = LoadedState::CHARACTERSELECT;
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

	if (*m_pMusicState == MusicState::MUSICON)
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

	//COLORS .. SO FAR ****************USE LOWERCASE*****************
	//Pink			//White		//Black			//Red		//Green	
	//DarkGreen		//Blue		//RoyalBlue		//BabyBlue	//MintGreen	
	//Orange		//Yellow	//Gold			//Fuchsia	//Purple
	//Brown			//DarkGray	//LightGray
	//**************************************************************

	//PLAYER 1
	//GameEntity::m_P1color = "green";
	//SET UP CHARACTER       x   y    w    h   maxS Accel					
	m_pPlayer1 = new GameEntity((SCREEN_WIDTH / 2) - (SCREEN_WIDTH / 4) - 64, (SCREEN_HEIGHT / 2) - (SCREEN_HEIGHT / 4) + 64, 128, 128, 350, 20, "player1", m_pRenderer, false);
	m_pPlayer1->LoadFile("Pics/blobee.png"); // Load Up The Full Sprite Sheet
	m_pPlayer1->TurnOnCollider(false, true); // Turn on colliders
	m_pPlayer1->RotateToDir(true);			  // Turn on Rotation
	m_pPlayer1->UseKeyForces(true);
	m_pPlayer1->SetColorMod(GameEntity::m_P1color); //COLOUR FOR PLAYER AND BACKDROP BLOB


	//PLAYER 2
	//GameEntity::m_P2color = "purple";
	//SET UP CHARACTER       x   y    w    h   maxS Accel					
	m_pPlayer2 = new GameEntity((SCREEN_WIDTH / 2) + (SCREEN_WIDTH / 4) - 64, (SCREEN_HEIGHT / 2) - (SCREEN_HEIGHT / 4) + 64, 128, 128, 350, 20, "player2", m_pRenderer, false);
	m_pPlayer2->LoadFile("Pics/blobee.png");  // Load Up The Full Sprite Sheet
	m_pPlayer2->TurnOnCollider(false, true);  // Turn on colliders
	m_pPlayer2->RotateToDir(true);			  // Turn on Rotation
	m_pPlayer2->UseGoToPoint(true);
	m_pPlayer2->SetColorMod(GameEntity::m_P2color); //COLOUR FOR PLAYER AND BACKDROP BLOB

	if (!m_P1PickUps.empty())
	{
		m_P1PickUps.clear();
	}
	if (!m_P2PickUps.empty())
	{
		m_P2PickUps.clear();
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

	*m_pLoadedState = LoadedState::GAMEON;
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