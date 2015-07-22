#include "SDLHelper.h"




SDLHelper::SDLHelper() : m_dt(0.0f), m_done(false)
{
	//SET INITIAL GAMESTATE
	m_pGameState = new GameState;
	*m_pGameState = GameState::GAMEON;
	if (*m_pGameState == GameState::MAINMENU){ std::cout << "MAINMENU ON" << std::endl; }
	if (*m_pGameState == GameState::GAMEON){ std::cout << "GAMESCREEN ON" << std::endl; }


	//SDL INIT
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("SDL_Init Error:", SDL_GetError());
	}
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
	if (m_pFont1 == NULL){std::cout << "ERROR FONT NOT LOADED ... NULL" << std::endl;}

	m_pFont2 = TTF_OpenFont("Fonts/BLACKR.ttf", 300);
	if (m_pFont2 == NULL){ std::cout << "ERROR FONT NOT LOADED ... NULL" << std::endl; }

	m_pFont3 = TTF_OpenFont("Fonts/ANUDRG__.ttf", 300);
	if (m_pFont3 == NULL){ std::cout << "ERROR FONT NOT LOADED ... NULL" << std::endl; }



	SDL_Color  color = {0x5D,0x00,0x00,0xFF};
	m_pfontTest = loadText("Justin's Test Font WOOO!", color, 1);


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

	//PLAYER 1
	GameEntity::m_P1color = "green";
	//SET UP CHARACTER       x   y    w    h   maxS Accel					
	m_pPlayer1 = new GameEntity((SCREEN_WIDTH / 2) - (SCREEN_WIDTH / 4)-64 , (SCREEN_HEIGHT / 2) - (SCREEN_HEIGHT / 4)+64, 128, 128, 350, 20, "player1", m_pRenderer, false);
	m_pPlayer1->LoadFile("Pics/blobee.png"); // Load Up The Full Sprite Sheet
	m_pPlayer1->TurnOnCollider(false, true); // Turn on colliders
	m_pPlayer1->RotateToDir(true);			  // Turn on Rotation
	m_pPlayer1->UseKeyForces(true);
	m_pPlayer1->SetColorMod(GameEntity::m_P1color); //COLOUR FOR PLAYER AND BACKDROP BLOB
	


	//PLAYER 2
	GameEntity::m_P2color = "purple";
	//SET UP CHARACTER       x   y    w    h   maxS Accel					
	m_pPlayer2 = new GameEntity((SCREEN_WIDTH / 2) + (SCREEN_WIDTH / 4)-64, (SCREEN_HEIGHT / 2) - (SCREEN_HEIGHT / 4)+64, 128, 128, 350, 20, "player2", m_pRenderer, false);
	m_pPlayer2->LoadFile("Pics/blobee.png");  // Load Up The Full Sprite Sheet
	m_pPlayer2->TurnOnCollider(false, true);  // Turn on colliders
	m_pPlayer2->RotateToDir(true);			  // Turn on Rotation
	m_pPlayer2->UseGoToPoint(true);
	m_pPlayer2->SetColorMod(GameEntity::m_P2color); //COLOUR FOR PLAYER AND BACKDROP BLOB
	



	//TITLE
	m_pTextTest = new GameEntity(0, 0, 300, 80,100,5, "Title", m_pRenderer, false);
	m_pTextTest->LoadTextFile(m_pFont2, "Warganism", "royalblue");


	//PLAYER BG 
	m_pP1BG = loadTexture("Pics/p1BG.png");
	m_pP2BG = loadTexture("Pics/p2BG.png");


	m_pbackground = loadTexture("Pics/background.png");
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
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
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

	if (*m_pGameState == GameState::MAINMENU){			ShowMainMenu(); }
	if (*m_pGameState == GameState::CHARACTERSELECT){	ShowCharSelection(); }
	if (*m_pGameState == GameState::GAMEON){			ShowGameOn(); }
	if (*m_pGameState == GameState::PAUSED){			ShowPaused(); }
	if (*m_pGameState == GameState::P1WIN){				ShowP1Win(); }
	if (*m_pGameState == GameState::P2WIN){				ShowP2Win(); }

	//Update screen
	SDL_RenderPresent(m_pRenderer);
}








//GAME FUNCTIONS**************************************************

void SDLHelper::SpawnProjectile(bool p1, bool p2)
{
	if (p1)
	{
		Projectile* proj = new Projectile(m_pPlayer1->getCenter().x, m_pPlayer1->getCenter().y, m_pPlayer1->getWidth() / 2, m_pPlayer1->getHeight() / 2, m_pPlayer1->m_Roation, GameEntity::m_P1color, m_pRenderer, "P1projectile");
		m_P1Projectiles.push_back(proj);
	}
	else if (p2)
	{
		//std::cout << "Made a Projectile" << std::endl;
		Projectile* proj = new Projectile(m_pPlayer2->getCenter().x, m_pPlayer2->getCenter().y, m_pPlayer2->getWidth() / 2, m_pPlayer2->getHeight() / 2, m_pPlayer2->m_Roation, GameEntity::m_P2color, m_pRenderer, "P2projectile");
		m_P2Projectiles.push_back(proj);
	}
	
}

void SDLHelper::SpawnRPickUp()
{
	
	int randomX = m_RG(MyWindow::getWidth() - MyWindow::getWidth()/2) + MyWindow::getWidth()/2;
	int randomY = m_RG(MyWindow::getHeight()) + 1;

	if (randomX > MyWindow::getWidth() / 2)
	{
		PickUp* pickUp = new PickUp(randomX, randomY, 50, 50, m_pRenderer, "PickUp", GameEntity::m_P1color);
		m_P2PickUps.push_back(pickUp);
	}

	 
}

void SDLHelper::SpawnLPickUp()
{

	int randomX = m_RG(MyWindow::getWidth() / 2) + 1;
	int randomY = m_RG(MyWindow::getHeight()) + 1;

	if (randomX < MyWindow::getWidth() / 2)
	{
		PickUp* pickUp = new PickUp(randomX, randomY, 50, 50, m_pRenderer, "PickUp", GameEntity::m_P2color);
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
			m_P1Projectiles[i]->m_pProjTex->Update(m_dt);
			m_P1Projectiles[i]->m_pProjTex->Render();

			if (Collision::CircleVsCircle(m_P1Projectiles[i]->m_pProjTex->GetCircleCollider(), m_pPlayer2->GetCircleCollider()))
			{
				//std::cout << "Erased a P1 Projectile -- COLLISION" << std::endl;
				m_pPlayer2->GetSmaller();
				m_P1Projectiles.erase(m_P1Projectiles.begin() + i);
				break;
			}

			if (m_P1Projectiles[i]->m_pProjTex->IsProjectileDone())
			{
				//std::cout << "Erased a P1 Projectile -- OFF SCREEN" << std::endl;
				m_P1Projectiles.erase(m_P1Projectiles.begin()+i);
			}
		}
	}

	if (m_P2Projectiles.size() > 0)
	{
		for (int i = 0; i < m_P2Projectiles.size(); i++)
		{
			m_P2Projectiles[i]->m_pProjTex->Update(m_dt);
			m_P2Projectiles[i]->m_pProjTex->Render();
			if (Collision::CircleVsCircle(m_P2Projectiles[i]->m_pProjTex->GetCircleCollider(), m_pPlayer1->GetCircleCollider()))
			{
				//std::cout << "Erased a P2 Projectile --COLLISION" << std::endl;
				m_pPlayer1->GetSmaller();
				m_P2Projectiles.erase(m_P2Projectiles.begin() + i);
				break;
			}
			if (m_P2Projectiles[i]->m_pProjTex->IsProjectileDone())
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
			KeyBoardHandler(e);
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
			SpawnProjectile(true, false);
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
	int x, y;
	SDL_GetMouseState(&x, &y);
	m_pPlayer2->SetMousePos(x, y);
	
	//std::cout << "Mouse X: " << x << " Mouse Y: " << y << std::endl;
	//CHECK BOUNDS OF WHAT MOUSE IS IN ...
	//m_pTexture->setPos((float)x, (float)y);
	
	switch (e.type)
	{
	case SDL_MOUSEMOTION:
		
		break;

	case SDL_MOUSEBUTTONDOWN:
		SpawnProjectile(false, true);
		//std::cout << "Mouse CLICK DOWN "<< std::endl;
		break;

	case SDL_MOUSEBUTTONUP:
		//std::cout << "Mouse CLICK UP " << std::endl;
		break;
	}

}






//RENDERING FUNCTIONS**************************************************
void SDLHelper::SetRenderColor(std::string color)
{
	if (color == "pink"){				SDL_SetRenderDrawColor(m_pRenderer, 0xFF, 0x99, 0xFF, 0xFF); }
	else if (color == "white"){			SDL_SetRenderDrawColor(m_pRenderer, 0xFF, 0xFF, 0xFF, 0xFF); }
	else if (color == "black"){			SDL_SetRenderDrawColor(m_pRenderer, 0x00, 0x00, 0x00, 0xFF); }
	else if (color == "red"){			SDL_SetRenderDrawColor(m_pRenderer, 0xFF, 0x00, 0x00, 0xFF); }
	else if (color == "green"){			SDL_SetRenderDrawColor(m_pRenderer, 0x00, 0xFF, 0x00, 0xFF); }
	else if (color == "mintgreen"){		SDL_SetRenderDrawColor(m_pRenderer, 0x00, 0xFF, 0x9A, 0xFF); }
	else if (color == "darkgreen"){		SDL_SetRenderDrawColor(m_pRenderer, 0x00, 0x66, 0x00, 0xFF); }
	else if (color == "blue"){			SDL_SetRenderDrawColor(m_pRenderer, 0x00, 0x00, 0xFF, 0xFF); }
	else if (color == "royalblue"){		SDL_SetRenderDrawColor(m_pRenderer, 0x00, 0x00, 0x66, 0xFF); }
	else if (color == "babyblue"){		SDL_SetRenderDrawColor(m_pRenderer, 0x00, 0xFF, 0xFF, 0xFF); }
	else if (color == "orange"){		SDL_SetRenderDrawColor(m_pRenderer, 0xFF, 0x77, 0x00, 0xFF); }
	else if (color == "yellow"){		SDL_SetRenderDrawColor(m_pRenderer, 0xFF, 0xFF, 0x00, 0xFF); }
	else if (color == "gold"){			SDL_SetRenderDrawColor(m_pRenderer, 0xFF, 0xBB, 0x00, 0xFF); }
	else if (color == "fuchsia"){		SDL_SetRenderDrawColor(m_pRenderer, 0x66, 0x00, 0x66, 0xFF); }
	else if (color == "purple"){		SDL_SetRenderDrawColor(m_pRenderer, 0x66, 0x00, 0x66, 0xFF); }
	else if (color == "brown"){			SDL_SetRenderDrawColor(m_pRenderer, 0x66, 0x33, 0x00, 0xFF); }
	else if (color == "darkgray"){		SDL_SetRenderDrawColor(m_pRenderer, 0x66, 0x66, 0x66, 0xFF); }
	else if (color == "lightgray"){		SDL_SetRenderDrawColor(m_pRenderer, 0x99, 0x99, 0x99, 0xFF); }
	else if (color == "halfblack"){		SDL_SetRenderDrawColor(m_pRenderer, 0x00, 0x00, 0x00, 0x99); }

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
	SDL_Rect rect = {x,y,w,h};
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
	SDL_RenderDrawLine(m_pRenderer,X1, Y1, X2, Y2);
}

void SDLHelper::DrawPoint(int x, int y, std::string color)
{
	SetRenderColor(color);
	SDL_RenderDrawPoint(m_pRenderer, x, y);
}








//SCREENS**************************************************

void SDLHelper::ShowMainMenu()
{

}

void SDLHelper::ShowPaused()
{

}


void SDLHelper::ShowP1Win()
{

}


void SDLHelper::ShowP2Win()
{

}


void SDLHelper::ShowCharSelection()
{

}


void SDLHelper::ShowGameOn()
{
	m_RPickUpSpawnTimeElapsed += m_dt;
	m_LPickUpSpawnTimeElapsed += m_dt;

	//BACKGROUND DRAWING
	SetRenderColor("white");
	SDL_Rect rec = { 0, 0, MyWindow::m_Width, MyWindow::m_Height };
	SDL_SetTextureColorMod(m_pP1BG, m_pPlayer1->getColor().r, m_pPlayer1->getColor().g, m_pPlayer1->getColor().b);
	SDL_SetTextureColorMod(m_pP2BG, m_pPlayer2->getColor().r, m_pPlayer2->getColor().g, m_pPlayer2->getColor().b);
	SDL_RenderCopy(m_pRenderer, m_pP1BG, NULL, &rec);
	SDL_RenderCopy(m_pRenderer, m_pP2BG, NULL, &rec);
	SDL_RenderCopy(m_pRenderer, m_pbackground, NULL, &rec);


	// 	SDL_Rect rec2 = { 0, 0, 1000, 100 };
	// 	SDL_RenderCopy(m_pRenderer, m_pfontTest, NULL, &rec2); //ALWAYS USE  DSTRect on Render.. Otherwise wont render ... wierd


	if (m_RPickUpSpawnTimeElapsed > m_RPickUpSpawnTime)
	{
		SpawnRPickUp();
		m_RPickUpSpawnTimeElapsed = 0;
		m_RPickUpSpawnTime = m_RG(15 - 7) + 7;
	}
	if (m_LPickUpSpawnTimeElapsed > m_LPickUpSpawnTime)
	{
		SpawnLPickUp();
		m_LPickUpSpawnTimeElapsed = 0;
		m_LPickUpSpawnTime = m_RG(15 - 7) + 7;
	}

	UpdateProjectiles();
	UpdatePickUp();
	//PLAYER 1
	m_pPlayer1->Update(m_dt);
	//m_pTexture2->SetScale(200, 200);
	m_pPlayer1->Render();

	//PLAYER 2
	//m_pTexture->SetScale(-100, -100);
	m_pPlayer2->Update(m_dt);
	m_pPlayer2->Render();


	m_pTextTest->setPos((MyWindow::m_Width / 2) - (m_pTextTest->getWidth() / 2), 0);
	m_pTextTest->Render(); //RENDER THE TEST TEXT


}






void SDLHelper::close()
{
	//Destroy window
	SDL_DestroyRenderer(m_pRenderer);
	m_pMyWindow->Free();

	//Free global font
	TTF_CloseFont(m_pFont1);
	m_pFont1 = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
	TTF_Quit();
}