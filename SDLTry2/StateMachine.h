#pragma once

enum class CPUState
{
	GETPICKUP,
	AIMENEMY,
	SHOOTENEMY,
	MOVERANDOM,
};
/*
AI PSEUDOCODE


*/

enum class GameState
{
	MAINMENU,
	PAUSED,
	P1WIN,
	P2WIN,
	CHARACTERSELECT,
	GAMEON
};

enum class LoadedState
{
	MAINMENU,
	PAUSED,
	P1WIN,
	P2WIN,
	CHARACTERSELECT,
	GAMEON,
	NONE
};

enum class SoundState
{
	SOUNDON,
	SOUNDOFF
};

enum class MusicState
{
	MUSICON,
	MUSICOFF
};

enum class GameMode
{
	PVP,
	PVCPU
};

struct StateMachine
{
	static GameState*	pGameState;
	static LoadedState* pLoadedState;
	static SoundState*	pSoundState;
	static MusicState*	pMusicState;
	static GameMode*	pGameMode;
	static CPUState*	pCPU;
};

