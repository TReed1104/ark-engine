#ifndef ARKENGINE_GAMECONTROLLER_H_
#define ARKENGINE_GAMECONTROLLER_H_

#include "SDL.h"

class GameController
{
public:


	GameController(SDL_GameController* sdlHook);
	~GameController();

	SDL_GameController* GetSDLHook(void);

private:
	SDL_GameController* gameController;

};
#endif
