#ifndef ARKENGINE_GAMECONTROLLER_H_
#define ARKENGINE_GAMECONTROLLER_H_

#include <map>
#include "SDL.h"
#include "glm/glm.hpp"

class Engine;

class GameController
{
public:
	static Engine* Engine_Pointer;

	enum Buttons { A, B, X, Y, Start, Back, ShoulderLeft, ShoulderRight, DPadUp, DPadDown, DPadLeft, DPadRight, };
	enum ThumbSticks { ThumbStickLeft, ThumbStickRight, };
	enum Triggers { TriggerLeft, TriggerRight, };

	GameController(SDL_GameController* sdlHook);
	~GameController();

	SDL_GameController* GetSDLHook(void);
	bool GetButtonState(Buttons button);
	glm::vec2 GetThumbStickState(ThumbSticks thumbStick);
	bool GetTriggerState(Triggers trigger);
	void UpdateButtonStates(void);
	void UpdateThumbSticks(void);

private:
	// SDL Hook
	SDL_GameController* gameController;

	// Input dictionaries
	std::map<std::string, bool> butttonStates;
	std::map<std::string, int> thumbStickStates;
	std::map<std::string, bool> triggerStates;
};
#endif
