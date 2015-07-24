#include "StateMachine.h"

//NICE NEW GLOBAL STATE MACHINE BY YOURS TRUELY
GameState*		StateMachine::pGameState	= new GameState;
LoadedState*	StateMachine::pLoadedState	= new LoadedState;
MusicState*		StateMachine::pMusicState	= new MusicState;
SoundState*		StateMachine::pSoundState	= new SoundState;
GameMode*		StateMachine::pGameMode		= new GameMode;
CPUState*		StateMachine::pCPU			= new CPUState;