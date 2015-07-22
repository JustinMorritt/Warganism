#pragma once
#include "SDL.h"
#include <vector>
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "MathUtils.h"
#include <iostream>
#include <utility>
#include <cmath>
#include "MyWindow.h"

struct AnimationConfig
{
	int frames, frameRate;
	bool loop;
	bool done;
};

//TEXTURE WRAPPER CLASS
class GameEntity
{
public:
	GameEntity(int x, int y, int w, int h, int maxSpeed, int accel, std::string name, SDL_Renderer* renderer, bool animation);
	~GameEntity();
	void LoadFile(std::string path); //USED FOR LOADING IMAGES
	void LoadTextFile(TTF_Font* theFont, std::string theText, std::string color);
	void Free();
	void Update(float dt);
	void Render();
	void SetAlpha(Uint8 alpha); //0xFF
	void SetScale(int w, int h);

	void setPos(float x, float y);

	std::string getName(); const
	SDL_Rect* getRect(); const
	int getWidth(); const
	int getHeight(); const
	
	Vec2* getPos(); 
	void SetAnimation(int x, int y, int numFrames, int frameW, int frameH, float delay, bool loop);
	void UpdateAnimation();
	void ApplyForces();
	void SetAnimate(bool on);
	bool Animating();
	SDL_Point getCenter();
	void SetCurrentAnimation(int num);
	int GetCurrentAnimation();
	SDL_Rect GetCurrentFrame();
	Circle GetCircleCollider();

	void SetColor(std::string color); //USED FOR FONTS
	void SetColorMod(std::string color);
	SDL_Color getColor();
	void addForce(std::string dir);
	void removeForce(std::string dir);
	void GoToPoint(int x, int y);
	void GoOneDirForever(float x, float y);
	void UseGoToPoint(bool on);
	void UseKeyForces(bool on);
	void UseGoOneDir(bool on);
	void RotateToDir(bool on);
	void CalculateRotation();
	void CheckPlayerBounds();
	void CheckProjectileBounds();
	void TurnOnCollider(bool square, bool circle);
	void UpdateColliders();
	void CenterPlayers();
	void SetMousePos(int x, int y);
	bool IsProjectileDone();
	
	
	std::string m_name;
	double m_Roation;
	static std::string m_P1color;
	static std::string m_P2color;
private:
	SDL_Renderer* m_pTheRenderer;
	Vec2* m_pPos;
	Vec2* m_pVel;
	Vec2* m_pcurrSpeed;
	Circle* m_pCircleCollider;
	SDL_Point* m_pCenter;
	float m_timePassed;
	float m_maxSpeed;
	float m_accel;
	SDL_Texture* m_pTexture;
	SDL_Rect m_pRect;
	
	SDL_RendererFlip m_RenderFlip;
	int m_MouseX;
	int m_MouseY;

	//FONT RENDERING 
	TTF_Font *m_pFont = NULL;
	SDL_Color m_pColour;

	//ANIMATION STUFF
	std::vector<std::pair<std::vector<SDL_Rect>, AnimationConfig>> m_Animations; //Vector of Pairs : Pair (ClipRects, AnimationStats)
	int m_currentAnimation;
	int m_currentFrame;
	int m_numFrames, m_frameW, m_frameH;
	float m_dt;
	float m_delay;
	Uint32 m_frameTimer;
	Uint32 m_Elapsed;
	Uint32 m_StartTime;
	bool m_Animation;
	bool m_Animate;
	bool m_loop;
	bool m_doneLoop;
	bool m_GoUp;
	bool m_GoDown;
	bool m_GoLeft;
	bool m_GoRight;
	bool m_SlowUp;
	bool m_SlowDown;
	bool m_SlowLeft;
	bool m_SlowRight;
	bool m_RotateToDir;
	bool m_UseCircleCollider;
	bool m_UseSquareCollider;
	bool m_UseGoToPoint;
	bool m_UseKeyForces;
	bool m_UseGoOneDirForever;
	bool m_ProjectileDone;
	
};

