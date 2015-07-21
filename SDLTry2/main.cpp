#include "SDLHelper.h"
#include <iostream>


//TODO : Create Player Class.. 

void Stealth(bool on)
{
	HWND Stealth;
	AllocConsole();
	Stealth = FindWindowA("ConsoleWindowClass", NULL);
	if (on)ShowWindow(Stealth, 0);
	if (!on)ShowWindow(Stealth, 1);
}





int main(int, char**)
{
	Stealth(false);

	SDLHelper* mySDL = new SDLHelper();


	LARGE_INTEGER currTime, prevTime, frequency;
	float deltaTime = 0.0f;
	float timePassed = 0.0f;
	int secsPassed = 0;
	int tickCounter = 0;

	QueryPerformanceFrequency(&frequency); //Most accurate .. CPU
	QueryPerformanceCounter(&prevTime);

	while (!mySDL->Done())
	{
		if (!mySDL->m_pMyWindow->isMinimized())
		{
			//UPDATING THE SDL HELPER
			QueryPerformanceCounter(&currTime);
			deltaTime = (float)(currTime.QuadPart - prevTime.QuadPart )/ (float)frequency.QuadPart;
			prevTime = currTime;
			mySDL->SetDT(deltaTime);
			mySDL->Update();
			tickCounter++;
			timePassed += deltaTime;
			if (timePassed > 1)
			{
				timePassed = 0;
				secsPassed++;
				
				//std::cout << "SECS PASSED: " << secsPassed << " CPU TICKS: " << tickCounter << std::endl; //FPS
				tickCounter = 0;
			}
		}
	}

	mySDL->close();

	//system("PAUSE");
	return 0;
}