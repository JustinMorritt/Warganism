#include "MyWindow.h"

int MyWindow::m_Width = SCREEN_WIDTH;
int MyWindow::m_Height = SCREEN_HEIGHT;
bool MyWindow::resetP1Pos = false;
bool MyWindow::resetP2Pos = false;

MyWindow::MyWindow()
{
	m_pWindow			= NULL;
	m_pTheRenderer		= NULL;
	m_MouseFocus		= false;
	m_KeyboardFocus		= false;
	m_FullScreen		= false;
	m_Minimized			= false;
}


void MyWindow::init()
{
	m_pWindow = SDL_CreateWindow("Warganism", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_RENDERER_PRESENTVSYNC | SDL_WINDOW_RESIZABLE);
}

SDL_Renderer* MyWindow::createRenderer()
{
	m_pTheRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	return m_pTheRenderer;
}

void MyWindow::handleEvent(SDL_Event& e)
{
	if (e.type == SDL_WINDOWEVENT)
	{
		bool upDateCaption = false;

		switch (e.window.event)
		{
			//UPDATES SIZES AND RE-PRESENTS
			case SDL_WINDOWEVENT_SIZE_CHANGED:
			m_Width = int(e.window.data1);  // NEW WIDTH
			m_Height = int(e.window.data2); //NEW HEIGHT
			resetP1Pos = true;
			resetP2Pos = true;
			if (*StateMachine::pGameState == GameState::MAINMENU)
			{
				*StateMachine::pLoadedState = LoadedState::NONE;
			}
			SDL_RenderPresent(m_pTheRenderer);
			break;

			//RE-RENDER ON EXPOSURE
			case SDL_WINDOWEVENT_EXPOSED:
			SDL_RenderPresent(m_pTheRenderer);
			break;

			case SDL_WINDOWEVENT_ENTER:
				m_MouseFocus = true;
				upDateCaption = true;
				break;

			case SDL_WINDOWEVENT_LEAVE:
				m_MouseFocus = false;
				upDateCaption = true;
				break;

			case SDL_WINDOWEVENT_FOCUS_GAINED:
				m_KeyboardFocus = true;
				upDateCaption = true;
				break;

			case SDL_WINDOWEVENT_FOCUS_LOST:
				m_KeyboardFocus = false;
				upDateCaption = true;
				break;

			case SDL_WINDOWEVENT_MINIMIZED:
				m_Minimized = true;
				break;

			case SDL_WINDOWEVENT_MAXIMIZED:
				//if (*StateMachine::pGameState == GameState::GAMEON){ *StateMachine::pGameState = GameState::GAMEON; }
				m_Minimized = false;
				break;

			case SDL_WINDOWEVENT_RESTORED:
				m_Minimized = false;
				break;

		}

// 		if (upDateCaption)
// 		{
// 			std::stringstream caption; 
// 			caption << "Mouse Focus: " << ((m_MouseFocus) ? "On" : "Off") << " KeyBoardFocus: " << ((m_KeyboardFocus) ? "On" : "Off") << " W : " << m_Width << "H : " << m_Height;
// 			SDL_SetWindowTitle(m_pWindow, caption.str().c_str());
// 		}

	}
}

int MyWindow::getWidth()
{
	return m_Width;
}



int MyWindow::getHeight() 
{
	return m_Height;
}

bool MyWindow::hasMouseFocus() const
{
	return m_MouseFocus;
}

bool MyWindow::hasKeyboardFocus() const
{
	return m_KeyboardFocus;
}

bool MyWindow::isMinimized() const
{
	return m_Minimized;
}

MyWindow::~MyWindow()
{
}

void MyWindow::Free()
{
	if (m_pWindow != NULL)
	{
		SDL_DestroyWindow(m_pWindow);
	}

	m_MouseFocus = false;
	m_KeyboardFocus = false;
	m_Width = 0;
	m_Height = 0;
}
