#include "Player.h"
#include "Engine.h"

Player::Player(const std::string & scriptPath) : Entity(scriptPath) {

}
Player::~Player(void) {

}

void Player::EntityController() {
	// If decide how the player is getting its inputs
	if (Engine_Pointer->deviceGameControllerRegister.size() != 0) {
		HandleInputGameController();
	}
	else {
		HandleInputKeyboard();
	}
}
void Player::HandleInputKeyboard(void) {
	movementDirection = Directions::NotSet;

	if (Engine_Pointer->deviceKeyboard->GetKeyState(Engine_Pointer->keybindMovementRight)) {
		movementDirection = Directions::Right;
		spriteDirection = Directions::Right;
	}
	else if (Engine_Pointer->deviceKeyboard->GetKeyState(Engine_Pointer->keybindMovementLeft)) {
		movementDirection = Directions::Left;
		spriteDirection = Directions::Left;
	}

	if (Engine_Pointer->deviceKeyboard->GetKeyState(Engine_Pointer->keybindMovementJump)) {
		if (!isJumping && !isFalling) {
			isJumping = true;
			currentJumpingSpeed = baseJumpingSpeed;
		}
		
	}

	// Crouching
	if (Engine_Pointer->deviceKeyboard->GetKeyState(Keyboard::LeftCtrl)) {
		isCrouching = true;
	}
	else {
		isCrouching = false;
	}

	// Sprinting
	if (Engine_Pointer->deviceKeyboard->GetKeyState(Keyboard::o)) {
		currentMovementSpeed = baseMovementSpeed * 2;
	}
	else {
		currentMovementSpeed = baseMovementSpeed;
	}
}
void Player::HandleInputGameController(void) {
	// TODO: Implement Controller support when Keyboard input is finished
}
