#include "GameController.h"

GameController::GameController(SDL_GameController* sdlHook) {
	gameController = sdlHook;
}

GameController::~GameController() {

}

SDL_GameController* GameController::GetSDLHook(void) {
	return gameController;
}
