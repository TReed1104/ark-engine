#include "GameController.h"

GameController::GameController() {

}

GameController::~GameController() {

}

SDL_GameController * GameController::GetSDLHook(void) {
	return gameController;
}
