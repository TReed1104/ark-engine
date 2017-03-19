#include "GameController.h"

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
		case Buttons::LeftShoulder:
			return butttonStates["Left Shoulder"];
		case Buttons::RightShoulder:
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
	const int PRESSED_STATE = 1;

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
	if (a == PRESSED_STATE) {
		butttonStates["A"] = true;
	}
	else {
		butttonStates["A"] = false;
	}
	if (b == PRESSED_STATE) {
		butttonStates["B"] = true;
	}
	else {
		butttonStates["B"] = false;
	}
	if (x == PRESSED_STATE) {
		butttonStates["X"] = true;
	}
	else {
		butttonStates["X"] = false;
	}
	if (y == PRESSED_STATE) {
		butttonStates["Y"] = true;
	}
	else {
		butttonStates["Y"] = false;
	}
	// Center Buttons
	if (start == PRESSED_STATE) {
		butttonStates["Start"] = true;
	}
	else {
		butttonStates["Start"] = false;
	}
	if (back == PRESSED_STATE) {
		butttonStates["Back"] = true;
	}
	else {
		butttonStates["Back"] = false;
	}
	// DPad Buttons
	if (dPadUp == PRESSED_STATE) {
		butttonStates["DPad Up"] = true;
	}
	else {
		butttonStates["DPad Up"] = false;
	}
	if (dPadDown == PRESSED_STATE) {
		butttonStates["DPad Down"] = true;
	}
	else {
		butttonStates["DPad Down"] = false;
	}
	if (dPadLeft == PRESSED_STATE) {
		butttonStates["DPad Left"] = true;
	}
	else {
		butttonStates["DPad Left"] = false;
	}
	if (dPadRight == PRESSED_STATE) {
		butttonStates["DPad Right"] = true;
	}
	else {
		butttonStates["DPad Right"] = false;
	}
}
void GameController::UpdateThumbSticks(void) {
	// Set the dead zone values
	const int THUMB_STICK_DEAD_ZONE = 12000;
	const int TRIGGER_DEAD_ZONE = 12000;

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
	if (leftThumbX < -THUMB_STICK_DEAD_ZONE) {
		thumbStickStates["Left Thumb X"] = leftThumbX;
	}
	else if (leftThumbX > THUMB_STICK_DEAD_ZONE) {
		thumbStickStates["Left Thumb X"] = leftThumbX;
	}
	else {
		thumbStickStates["Left Thumb X"] = 0.0f;
	}

	// Check the Left Thumb Y
	if (leftThumbY < -THUMB_STICK_DEAD_ZONE) {
		thumbStickStates["Left Thumb Y"] = leftThumbY;
	}
	else if (leftThumbY > THUMB_STICK_DEAD_ZONE) {
		thumbStickStates["Left Thumb Y"] = leftThumbY;
	}
	else {
		thumbStickStates["Left Thumb Y"] = 0.0f;
	}

	// Check the Right Thumb X
	if (rightThumbX < -THUMB_STICK_DEAD_ZONE) {
		thumbStickStates["Right Thumb X"] = rightThumbX;
	} 
	else if(rightThumbX > THUMB_STICK_DEAD_ZONE) {
		thumbStickStates["Right Thumb X"] = rightThumbX;
	}
	else {
		thumbStickStates["Right Thumb X"] = 0.0f;
	}

	// Check the Right Thumb Y
	if (rightThumbY < -THUMB_STICK_DEAD_ZONE) {
		thumbStickStates["Right Thumb Y"] = rightThumbX;
	}
	else if (rightThumbY > THUMB_STICK_DEAD_ZONE) {
		thumbStickStates["Right Thumb Y"] = rightThumbX;
	}
	else {
		thumbStickStates["Right Thumb Y"] = 0.0f;
	}

	// Check the Triggers
	if (leftTrigger < TRIGGER_DEAD_ZONE) {
		triggerStates["Left"] = true;
	}
	else {
		triggerStates["Left"] = false;
	}
	if (rightTrigger < TRIGGER_DEAD_ZONE) {
		triggerStates["Right"] = true;
	}
	else {
		triggerStates["Right"] = false;
	}
}