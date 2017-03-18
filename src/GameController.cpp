#include "GameController.h"

GameController::GameController(SDL_GameController* sdlHook) {
	gameController = sdlHook;

	// Setup the button dictionary
	buttonInputs["Button A"] = false;
	buttonInputs["Button B"] = false;
	buttonInputs["Button X"] = false;
	buttonInputs["Button Y"] = false;
	buttonInputs["Button Start"] = false;
	buttonInputs["Button Back"] = false;
	buttonInputs["Button Left Trigger"] = false;
	buttonInputs["Button Left Bumper"] = false;
	buttonInputs["Button Right Trigger"] = false;
	buttonInputs["Button Right Bumper"] = false;
	buttonInputs["Button DPad Up"] = false;
	buttonInputs["Button DPad Down"] = false;
	buttonInputs["Button DPad Left"] = false;
	buttonInputs["Button DPad Right"] = false;

	thumbStickAxis["Left Thumb X"] = 0.0f;
	thumbStickAxis["Left Thumb Y"] = 0.0f;
	thumbStickAxis["Right Thumb X"] = 0.0f;
	thumbStickAxis["Right Thumb Y"] = 0.0f;
}

GameController::~GameController() {

}

SDL_GameController* GameController::GetSDLHook(void) {
	return gameController;
}
