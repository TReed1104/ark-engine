#ifndef ARKENGINE_GAMECONTROLLER_H_
#define ARKENGINE_GAMECONTROLLER_H_

#include "SDL.h"
#include <map>

class GameController
{
public:
	// Input dictionaries
	std::map<std::string, bool> buttonInputs;
	std::map<std::string, float> thumbStickAxis;

	GameController(SDL_GameController* sdlHook);
	~GameController();

	SDL_GameController* GetSDLHook(void);

private:
	SDL_GameController* gameController;

};
#endif
