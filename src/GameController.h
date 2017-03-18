#ifndef ARKENGINE_ENGINE_H_
#define ARKENGINE_ENGINE_H_

#include "SDL.h"

class GameController
{
public:


	GameController();
	~GameController();

	SDL_GameController* GetSDLHook(void);

private:
	SDL_GameController* gameController;

};
#endif
