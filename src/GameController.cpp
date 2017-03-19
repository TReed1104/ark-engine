#include "GameController.h"
#include "Engine.h"

Engine* GameController::s_EnginePointer;

GameController::GameController(SDL_GameController* sdlHook) {
	// Set the SDL Hook, this is what SDL actually puts the data into
	gameController = sdlHook;

	// Button State dictionary
	butttonStates["A"] = false;
	butttonStates["B"] = false;
	butttonStates["X"] = false;
	butttonStates["Y"] = false;
	butttonStates["Start"] = false;
	butttonStates["Back"] = false;
	butttonStates["Left Shoulder"] = false;
	butttonStates["Right Shoulder"] = false;
	butttonStates["DPad Up"] = false;
	butttonStates["DPad Down"] = false;
	butttonStates["DPad Left"] = false;
	butttonStates["DPad Right"] = false;

	// Thumb stick state dictionary
	thumbStickStates["Left Thumb X"] = 0.0f;
	thumbStickStates["Left Thumb Y"] = 0.0f;
	thumbStickStates["Right Thumb X"] = 0.0f;
	thumbStickStates["Right Thumb Y"] = 0.0f;

	// Trigger state dictionary
	triggerStates["Left"] = false;
	triggerStates["Right"] = false;
}
GameController::~GameController() {

}

SDL_GameController* GameController::GetSDLHook(void) {
	return gameController;
}
bool GameController::GetButtonState(Buttons controllerButton) {
	switch (controllerButton) {
		case Buttons::A:
			return butttonStates["A"];
		case Buttons::B:
			return butttonStates["B"];
		case Buttons::X:
			return butttonStates["X"];
		case Buttons::Y:
			return butttonStates["Y"];
		case Buttons::Start:
			return butttonStates["Start"];
		case Buttons::Back:
			return butttonStates["Back"];
		case Buttons::ShoulderLeft:
			return butttonStates["Left Shoulder"];
		case Buttons::ShoulderRight:
			return butttonStates["Right Shoulder"];
		case Buttons::DPadUp:
			return butttonStates["DPad Up"];
		case Buttons::DPadDown:
			return butttonStates["DPad Down"];
		case Buttons::DPadLeft:
			return butttonStates["DPad Left"];
		case Buttons::DPadRight:
			return butttonStates["DPad Right"];
		default:
			return false;
	}
	return false;
}
glm::vec2 GameController::GetThumbStickState(ThumbSticks thumbStick) {
	if (thumbStick == ThumbSticks::ThumbStickLeft) {
		return glm::vec2(thumbStickStates["Left Thumb X"], thumbStickStates["Left Thumb Y"]);
	}
	else if (thumbStick == ThumbSticks::ThumbStickRight) {
		return glm::vec2(thumbStickStates["Right Thumb X"], thumbStickStates["Right Thumb Y"]);
	}
}
bool GameController::GetTriggerState(Triggers trigger) {
	if (trigger == Triggers::TriggerLeft) {
		return triggerStates["Left"];
	}
	else if (trigger == Triggers::TriggerRight) {
		return triggerStates["Right"];
	}
	else {
		return false;
	}
}
void GameController::UpdateButtonStates(void) {
	// Standard Buttons
	int a = SDL_GameControllerGetButton(gameController, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_A);
	int b = SDL_GameControllerGetButton(gameController, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_B);
	int x = SDL_GameControllerGetButton(gameController, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_X);
	int y = SDL_GameControllerGetButton(gameController, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_Y);
	int start = SDL_GameControllerGetButton(gameController, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_START);
	int back = SDL_GameControllerGetButton(gameController, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_BACK);
	// DPad Buttons
	int dPadUp = SDL_GameControllerGetButton(gameController, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_BACK);
	int dPadDown = SDL_GameControllerGetButton(gameController, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_BACK);
	int dPadLeft = SDL_GameControllerGetButton(gameController, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_BACK);
	int dPadRight = SDL_GameControllerGetButton(gameController, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_BACK);
	// Shoulder Buttons
	int leftBumper = SDL_GameControllerGetButton(gameController, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
	int rightBumper = SDL_GameControllerGetButton(gameController, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);

	// Main Button inputs
	if (a == s_EnginePointer->pressedStateFlag) {
		butttonStates["A"] = true;
	}
	else {
		butttonStates["A"] = false;
	}
	if (b == s_EnginePointer->pressedStateFlag) {
		butttonStates["B"] = true;
	}
	else {
		butttonStates["B"] = false;
	}
	if (x == s_EnginePointer->pressedStateFlag) {
		butttonStates["X"] = true;
	}
	else {
		butttonStates["X"] = false;
	}
	if (y == s_EnginePointer->pressedStateFlag) {
		butttonStates["Y"] = true;
	}
	else {
		butttonStates["Y"] = false;
	}
	// Center Buttons
	if (start == s_EnginePointer->pressedStateFlag) {
		butttonStates["Start"] = true;
	}
	else {
		butttonStates["Start"] = false;
	}
	if (back == s_EnginePointer->pressedStateFlag) {
		butttonStates["Back"] = true;
	}
	else {
		butttonStates["Back"] = false;
	}
	// DPad Buttons
	if (dPadUp == s_EnginePointer->pressedStateFlag) {
		butttonStates["DPad Up"] = true;
	}
	else {
		butttonStates["DPad Up"] = false;
	}
	if (dPadDown == s_EnginePointer->pressedStateFlag) {
		butttonStates["DPad Down"] = true;
	}
	else {
		butttonStates["DPad Down"] = false;
	}
	if (dPadLeft == s_EnginePointer->pressedStateFlag) {
		butttonStates["DPad Left"] = true;
	}
	else {
		butttonStates["DPad Left"] = false;
	}
	if (dPadRight == s_EnginePointer->pressedStateFlag) {
		butttonStates["DPad Right"] = true;
	}
	else {
		butttonStates["DPad Right"] = false;
	}
}
void GameController::UpdateThumbSticks(void) {

	// Left Thumb Stick
	int leftThumbX = SDL_GameControllerGetAxis(gameController, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTX);
	int leftThumbY = SDL_GameControllerGetAxis(gameController, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTY);
	// Right Thumb Stick
	int rightThumbX = SDL_GameControllerGetAxis(gameController, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_RIGHTX);
	int rightThumbY = SDL_GameControllerGetAxis(gameController, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_RIGHTY);
	// Left Thumb Stick
	int leftTrigger = SDL_GameControllerGetAxis(gameController, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_TRIGGERLEFT);
	int rightTrigger = SDL_GameControllerGetAxis(gameController, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_TRIGGERRIGHT);

	// Check the Left Thumb X
	if (leftThumbX < -s_EnginePointer->thumbStickDeadZone) {
		thumbStickStates["Left Thumb X"] = leftThumbX;
	}
	else if (leftThumbX > s_EnginePointer->thumbStickDeadZone) {
		thumbStickStates["Left Thumb X"] = leftThumbX;
	}
	else {
		thumbStickStates["Left Thumb X"] = 0.0f;
	}

	// Check the Left Thumb Y
	if (leftThumbY < -s_EnginePointer->thumbStickDeadZone) {
		thumbStickStates["Left Thumb Y"] = leftThumbY;
	}
	else if (leftThumbY > s_EnginePointer->thumbStickDeadZone) {
		thumbStickStates["Left Thumb Y"] = leftThumbY;
	}
	else {
		thumbStickStates["Left Thumb Y"] = 0.0f;
	}

	// Check the Right Thumb X
	if (rightThumbX < -s_EnginePointer->thumbStickDeadZone) {
		thumbStickStates["Right Thumb X"] = rightThumbX;
	} 
	else if(rightThumbX > s_EnginePointer->thumbStickDeadZone) {
		thumbStickStates["Right Thumb X"] = rightThumbX;
	}
	else {
		thumbStickStates["Right Thumb X"] = 0.0f;
	}

	// Check the Right Thumb Y
	if (rightThumbY < -s_EnginePointer->thumbStickDeadZone) {
		thumbStickStates["Right Thumb Y"] = rightThumbY;
	}
	else if (rightThumbY > s_EnginePointer->thumbStickDeadZone) {
		thumbStickStates["Right Thumb Y"] = rightThumbY;
	}
	else {
		thumbStickStates["Right Thumb Y"] = 0.0f;
	}

	// Check the Triggers
	if (leftTrigger > s_EnginePointer->triggerDeadZone) {
		triggerStates["Left"] = true;
	}
	else {
		triggerStates["Left"] = false;
	}
	if (rightTrigger > s_EnginePointer->triggerDeadZone) {
		triggerStates["Right"] = true;
	}
	else {
		triggerStates["Right"] = false;
	}
}