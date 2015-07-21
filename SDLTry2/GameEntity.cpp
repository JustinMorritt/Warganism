#include "GameEntity.h"





GameEntity::GameEntity(int x, int y, int w, int h, int maxSpeed, int accel, std::string name, SDL_Renderer* renderer, bool animation)
{
	m_MouseX			= 0;
	m_MouseY			= 0;
	m_maxSpeed			= maxSpeed;
	m_accel				= accel;
	m_currentFrame		= 0;
	m_currentAnimation	= 0;
	m_timePassed		= 0.0f;
	m_Animate			= false;
	m_Animation			= animation;
	m_pTheRenderer		= renderer;
	m_pPos				= new Vec2;
	m_pVel				= new Vec2;
	m_pcurrSpeed		= new Vec2;
	m_pCircleCollider	= new Circle;
	m_pcurrSpeed->x		= 0;
	m_pcurrSpeed->y		= 0;
	m_pVel->x			= 0;
	m_pVel->y			= 0;
	m_pPos->x			= x;
	m_pPos->y			= y;
	m_pRect.w			= w;
	m_pRect.h			= h;
	m_name				= name;
	m_pRect				= { x, y, w, h };
	m_numFrames			= 1;
	m_frameW			= 1;
	m_frameH			= 1;
	m_delay				= 0;
	m_loop				= false;
	m_doneLoop			= false;
	m_pColour			= { 0x00, 0x00, 0x00, 0x00 };
	m_GoUp				= false;
	m_GoDown			= false;
	m_GoLeft			= false;
	m_GoRight			= false;
	m_SlowUp			= false;
	m_SlowDown			= false;
	m_SlowLeft			= false;
	m_SlowRight			= false;
	m_RotateToDir		= false;
	m_Roation			= 0.0;
	m_RenderFlip		= SDL_FLIP_NONE;
	m_pCenter			= new SDL_Point;
	m_pCenter->x		= m_pPos->x + (m_pRect.w/2);
	m_pCenter->y		= m_pPos->y + (m_pRect.h / 2);

}

GameEntity::~GameEntity()
{
	Free();
}

void GameEntity::LoadFile(std::string path)
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
		newTexture = SDL_CreateTextureFromSurface(m_pTheRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		SDL_FreeSurface(loadedSurface);
	}
	m_pTexture = newTexture;
}

void GameEntity::LoadTextFile(TTF_Font* theFont, std::string theText, std::string color)
{
	Free(); //FREE EXISTING TEXTURE IF ONE 
	SetColor(color);
	SDL_Texture* retText = NULL;
	SDL_Surface* textSurface = TTF_RenderText_Solid(theFont, theText.c_str(), m_pColour);
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		//Create texture from surface pixels
		retText = SDL_CreateTextureFromSurface(m_pTheRenderer, textSurface);
		if (retText == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: \n", SDL_GetError());
		}
	}
	SDL_FreeSurface(textSurface);
	m_pTexture = retText;
}

void GameEntity::Free()
{
	if (m_pTexture != NULL)
	{
		SDL_DestroyTexture(m_pTexture);
		m_pTexture = NULL;
	}
}

void GameEntity::Update(float dt)
{
	//IF WINDOW RESIZED 
	if (MyWindow::resetP1Pos || MyWindow::resetP2Pos){ CenterPlayers(); }


	//ROTATE TO DIRECTION IF APPLICABLE
	if (m_RotateToDir)
	{
		if (m_GoDown || m_GoLeft || m_GoRight || m_GoUp){ CalculateRotation(); } //ONLY IF A KEY BEING PRESSED 
		if (m_UseGoToPoint){ CalculateRotation();}
		
	}
	
	m_dt = dt; //Assigned for use in SpriteAnimation
	m_timePassed += dt;

	//UPDATE SPRITE ANIMATIONS
	if (Animating())
	{
		UpdateAnimation();
	}
	
	if (m_timePassed > 0.016) //60times time a second
	{
		if (m_name == "player1" || m_name == "player2")
		{
			if (m_UseGoToPoint){ GoToPoint(m_MouseX,m_MouseY); }
			if (m_UseKeyForces){ ApplyForces(); }
			m_timePassed = 0;
		}
	}

	//ASSIGN NEW POSITION AFTER FORCES HAVE BEEN APPLIED
	m_pPos->x += m_pVel->x * dt;		//STEP ON X
	UpdateColliders();					//UPDATE COLLIDERS
	//****PLAYER 1****
	if (m_name == "player1")			//CHECK COLLISION ON X
	{
		if ((m_pCenter->x - m_pCircleCollider->r < 0) || (m_pCenter->x + m_pCircleCollider->r > MyWindow::m_Width / 2))
		{
			m_pPos->x -= m_pVel->x * dt;
			UpdateColliders();
		}
	}
	//*****PLAYER 2*****
	else if (m_name == "player2")		//CHECK COLLISION ON X
		{
			if (m_pCenter->x - m_pCircleCollider->r < MyWindow::m_Width / 2 || m_pCenter->x + m_pCircleCollider->r > MyWindow::m_Width)
			{
				m_pPos->x -= m_pVel->x * dt;
				m_pcurrSpeed->x = 0;
				UpdateColliders();
			}
		}

	m_pPos->y += m_pVel->y * dt; //STEP ON THE Y SAME FOR BOTH PLAYERS
	UpdateColliders();
	if ((m_pCenter->y - m_pCircleCollider->r < 0) || (m_pCenter->y + m_pCircleCollider->r > MyWindow::m_Height))
	{
		if (m_name == "player2")
		{
			m_pPos->y -= m_pVel->y * dt;
			m_pcurrSpeed->y = 0;
		}
		else
		{
			m_pPos->y -= m_pVel->y * dt;
		}
		UpdateColliders();
	}
	






}

void GameEntity::Render()
{
	if (m_pTexture == NULL)
	{
		std::cout << "Big Error ...Texture not loaded" << std::endl;
	}
	SDL_Rect full = { m_pRect.x, m_pRect.y, m_pRect.w, m_pRect.h };

	if (m_Animation)
	{
		SDL_RenderCopy(m_pTheRenderer, m_pTexture, &GetCurrentFrame(), &full);
	}
	else
	{
		SDL_RenderCopyEx(m_pTheRenderer, m_pTexture, NULL, &full, m_Roation, NULL, m_RenderFlip);
	}
}

void GameEntity::SetAlpha(Uint8 alpha)
{
	SDL_SetTextureAlphaMod(m_pTexture, alpha);
}

void GameEntity::SetScale(int w, int h)
{
	m_pRect.w = w;
	m_pRect.h = h;
}

void GameEntity::setPos(float x, float y)
{
	m_pPos->x = x;
	m_pPos->y = y;

	m_pRect.x = x;
	m_pRect.y = y;

}

std::string GameEntity::getName()
{
	return m_name;
}

const
int GameEntity::getWidth()
{
	return m_pRect.w;
}

const
SDL_Rect* GameEntity::getRect()
{
	return &m_pRect;
}

const
int GameEntity::getHeight()
{
	return m_pRect.h;
}

const
Vec2* GameEntity::getPos()
{
	return m_pPos;
}

void GameEntity::SetAnimation(int x, int y, int numFrames, int frameW, int frameH, float delay, bool loop)
{
	std::vector<SDL_Rect> frames;
	AnimationConfig animConfig = { numFrames, delay, loop, false};
	for (int i = 0; i < numFrames; ++i)
	{
		SDL_Rect newR = { x + (frameW*i), y, frameW, frameH };
		frames.push_back(newR);
	}
	m_Animations.push_back(make_pair(frames, animConfig)); //Push back the Bunch of frames 
}

void GameEntity::UpdateAnimation()
{
	
	// USE EPSILON to compare with .. for presision errors ... used to compare if Velocity.x  > EPSILON;
	//TODO: Velocity based Animation Type Selecting 
	m_frameTimer = SDL_GetTicks();
	m_Elapsed = (m_frameTimer - m_StartTime); //Milloseconds since Starttime was assigned
	if (m_Elapsed >= m_Animations.at(m_currentAnimation).second.frameRate) //Access the framerate from the current Animation
	{
		m_StartTime = SDL_GetTicks();
		m_currentFrame++;
		if (m_currentFrame == m_Animations.at(m_currentAnimation).second.frames)
		{
			m_currentFrame = 0;
			if (!m_Animations.at(m_currentAnimation).second.loop)
			{
				//m_currentFrame = m_Animations.at(m_currentAnimation).second.frames-2;
				m_Animations.at(m_currentAnimation).second.done = true;
				SetAnimate(false);
			}
		}
	}
}

void GameEntity::ApplyForces()
{
	
	//ACCELERATION
	if (m_GoUp )
	{ 		
		if (m_pcurrSpeed->y != -m_maxSpeed){ m_pcurrSpeed->y -= m_accel;  if (m_pcurrSpeed->y < -m_maxSpeed){ m_pcurrSpeed->y = -m_maxSpeed; } }
	}
	if (m_GoDown)
	{ 
		if (m_pcurrSpeed->y != m_maxSpeed){ m_pcurrSpeed->y += m_accel;  if (m_pcurrSpeed->y > m_maxSpeed){ m_pcurrSpeed->y = m_maxSpeed; } }
	}
	if (m_GoLeft)
	{
		if (m_pcurrSpeed->x != -m_maxSpeed){ m_pcurrSpeed->x -= m_accel; if (m_pcurrSpeed->x < -m_maxSpeed){ m_pcurrSpeed->x = -m_maxSpeed; } }
	}
	if (m_GoRight)
	{ 
		if (m_pcurrSpeed->x != m_maxSpeed){ m_pcurrSpeed->x += m_accel; if (m_pcurrSpeed->x > m_maxSpeed){ m_pcurrSpeed->x = m_maxSpeed; } }
		//std::cout << m_pcurrSpeed->x << " velx: " << m_pVel->x << " " << m_maxSpeed << std::endl;
	}

	//DECELLERATION
	if (m_SlowUp)
	{
		m_pcurrSpeed->y += m_accel; if (m_pcurrSpeed->y > 0){ m_SlowUp = false; m_pcurrSpeed->y = 0;}
	}
	if (m_SlowDown)
	{
		m_pcurrSpeed->y -= m_accel; if (m_pcurrSpeed->y < 0){ m_SlowDown = false; m_pcurrSpeed->y = 0; }
	}
	if (m_SlowLeft)
	{
		m_pcurrSpeed->x += m_accel; if (m_pcurrSpeed->x > 0){ m_SlowLeft = false; m_pcurrSpeed->x = 0; }
	}
	if (m_SlowRight)
	{
		m_pcurrSpeed->x -= m_accel; if (m_pcurrSpeed->x < 0){ m_SlowRight = false; m_pcurrSpeed->x = 0; }
	}

	//ASSIGN NEW VELOCITYS 
	m_pVel->x = (m_pcurrSpeed->x);
	m_pVel->y = (m_pcurrSpeed->y);
}

void GameEntity::SetAnimate(bool on)
{
	m_Animate = on;
}

bool GameEntity::Animating()
{
	return m_Animate;
}

SDL_Point GameEntity::getCenter()
{
	return *m_pCenter;
}

void GameEntity::SetCurrentAnimation(int num)
{
	if (!m_Animations.at(m_currentAnimation).second.loop)
	{
		if (m_Animations.at(m_currentAnimation).second.done)
		{
			m_Animations.at(m_currentAnimation).second.done = false;
			m_currentAnimation = num;
		}
	}
	else
	{
		m_currentAnimation = num;
	}
	
}

int GameEntity::GetCurrentAnimation()
{
	return m_currentAnimation;
}

SDL_Rect GameEntity::GetCurrentFrame()
{
	return m_Animations.at(m_currentAnimation).first.at(m_currentFrame); // Returns the Rect of the Current Frame .. 
}

void GameEntity::SetColor(std::string color)
{
	if (color == "pink"){				m_pColour = {0xFF, 0x99, 0xFF, 0xFF} ;	 }
	else if (color == "white"){			m_pColour = {0xFF, 0xFF, 0xFF, 0xFF} ;	 }
	else if (color == "black"){			m_pColour = {0x00, 0x00, 0x00, 0xFF} ;	 }
	else if (color == "red"){			m_pColour = {0xFF, 0x00, 0x00, 0xFF} ;	 }
	else if (color == "green"){			m_pColour = {0x00, 0xFF, 0x00, 0xFF} ;	 }
	else if (color == "mintgreen"){		m_pColour = {0x00, 0xFF, 0x9A, 0xFF} ;	 }
	else if (color == "darkgreen"){		m_pColour = {0x00, 0x66, 0x00, 0xFF} ;	 }
	else if (color == "blue"){			m_pColour = {0x00, 0x00, 0xFF, 0xFF} ;	 }
	else if (color == "royalblue"){		m_pColour = {0x00, 0x00, 0x66, 0xFF} ;	 }
	else if (color == "babyblue"){		m_pColour = {0x00, 0xFF, 0xFF, 0xFF} ;	 }
	else if (color == "orange"){		m_pColour = {0xFF, 0x77, 0x00, 0xFF} ;	 }
	else if (color == "yellow"){		m_pColour = {0xFF, 0xFF, 0x00, 0xFF} ;	 }
	else if (color == "gold"){			m_pColour = {0xFF, 0xBB, 0x00, 0xFF} ;	 }
	else if (color == "fuchsia"){		m_pColour = {0x66, 0x00, 0x66, 0xFF} ;	 }
	else if (color == "purple"){		m_pColour = {0x66, 0x00, 0x66, 0xFF} ;	 }
	else if (color == "brown"){			m_pColour = {0x66, 0x33, 0x00, 0xFF} ;	 }
	else if (color == "darkgray"){		m_pColour = {0x66, 0x66, 0x66, 0xFF} ;	 }
	else if (color == "lightgray"){		m_pColour = {0x99, 0x99, 0x99, 0xFF} ;	 }
	else if (color == "halfblack"){		m_pColour = {0x00, 0x00, 0x00, 0x99} ;	 }
}

void GameEntity::SetColorMod(std::string color)
{
	//TEXTURE MOD
	SetColor(color);
	SDL_SetTextureColorMod(m_pTexture, m_pColour.r, m_pColour.g, m_pColour.b);
}

SDL_Color GameEntity::getColor()
{
	return m_pColour;
}

void GameEntity::addForce(std::string dir)
{

	if (dir == "up")
	{
		m_GoUp = true;
		m_SlowUp = false;
	}
	else if (dir == "down")
	{
		m_GoDown = true;
		m_SlowDown = false;
	}
	else if (dir == "left")
	{
		m_GoLeft = true;
		m_SlowLeft = false;
	}
	else if (dir == "right")
	{
		m_GoRight = true;
		m_SlowRight = false;
	}
}

void GameEntity::removeForce(std::string dir)
{
	if (dir == "up")
	{
		m_GoUp = false;
		m_SlowUp = true;
	}
	else if (dir == "down")
	{
		m_GoDown = false;
		m_SlowDown = true;
	}
	else if (dir == "left")
	{
		m_GoLeft = false;
		m_SlowLeft = true;
	}
	else if (dir == "right")
	{
		m_GoRight = false;
		m_SlowRight = true;
	}
}

void GameEntity::GoToPoint(int x, int y)
{
	if (x != m_pPos->x || y != m_pPos->y)
	{
		Vec2 DIR;
		DIR.x = x - m_pCenter->x;
		DIR.y = y - m_pCenter->y;
		DIR = VectorMath::Normalize(&DIR);

		 m_pcurrSpeed->y += m_accel*DIR.y; 
		 m_pcurrSpeed->x += m_accel*DIR.x;

		//ASSIGN NEW VELOCITYS 
		m_pVel->x = (m_pcurrSpeed->x);
		m_pVel->y = (m_pcurrSpeed->y);
	}
}

void GameEntity::GoOneDirForever(float x, float y)
{
	m_pVel->x = x * m_maxSpeed;
	m_pVel->y = y * m_maxSpeed;
}

void GameEntity::UseGoToPoint(bool on)
{
	m_UseGoToPoint = on;
}

void GameEntity::UseKeyForces(bool on)
{
	m_UseKeyForces = on;
}

void GameEntity::UseGoOneDir(bool on)
{
	m_UseGoOneDirForever = on;
}

void GameEntity::RotateToDir(bool on)
{
	m_RotateToDir = on;
}

void GameEntity::CalculateRotation()
{
	m_Roation = atan2(-m_pVel->x, m_pVel->y) * 180 / pi;
}

void GameEntity::TurnOnCollider(bool square, bool circle)
{
	m_UseCircleCollider = circle;
	m_UseSquareCollider = square;
}

void GameEntity::UpdateColliders()
{
	//Update Center
	m_pCenter->x = m_pPos->x + (m_pRect.w / 2);
	m_pCenter->y = m_pPos->y + (m_pRect.h / 2);

	//Bounding Rect
	m_pRect.x = m_pPos->x;
	m_pRect.y = m_pPos->y;

	m_pCircleCollider->x = m_pCenter->x;
	m_pCircleCollider->y = m_pCenter->y;
	m_pCircleCollider->r = m_pRect.w/2;
}

void GameEntity::CenterPlayers()
{
	if (getName() == "player1")
	{
		setPos((MyWindow::m_Width / 2 - MyWindow::m_Width / 4) - (m_pRect.w / 2), MyWindow::m_Height / 2 - (m_pRect.h / 2));
		MyWindow::resetP1Pos = false;
	}
	if (getName() == "player2")
	{
		setPos((MyWindow::m_Width / 2 + MyWindow::m_Width / 4) - (m_pRect.w / 2), MyWindow::m_Height / 2 - (m_pRect.h / 2));
		MyWindow::resetP2Pos = false;
	}
}

void GameEntity::SetMousePos(int x, int y)
{
	m_MouseX = x;
	m_MouseY = y;
}
