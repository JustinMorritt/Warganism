#pragma once

#include <SDL.h>
#include <sstream>
#include "StateMachine.h"

class SDLHelper;
class GameEntity;

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 600;

class MyWindow
{
public:
	MyWindow();
	void init();
	SDL_Renderer* createRenderer();
	void handleEvent(SDL_Event& e);
	void Free(); //CLEAN UP MEMORY

	//GETTERS
	static int getWidth();
	static int getHeight();
	static int m_Width;
	static int m_Height;
	static bool resetP1Pos;
	static bool resetP2Pos;
	bool hasMouseFocus()	const;
	bool hasKeyboardFocus() const;
	bool isMinimized()		const;
	SDL_Window* m_pWindow;
	
	~MyWindow();
private:
	SDL_Renderer* m_pTheRenderer;


	bool m_MouseFocus;
	bool m_KeyboardFocus;
	bool m_FullScreen;
	bool m_Minimized;
};

