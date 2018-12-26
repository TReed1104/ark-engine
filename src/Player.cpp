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
	// Reset the movement direction
	movementDirection = Directions::MovementNotSet;
	if (Engine_Pointer->deviceKeyboard->GetKeyState(Engine_Pointer->keybindMovementJump)) {
		// Jumping
		movementDirection = Directions::MovementJumping;

		// See if the Player is pressing left or right as they jump
		if (Engine_Pointer->deviceKeyboard->GetKeyState(Engine_Pointer->keybindMovementLeft)) {
			// Left movement
			spriteDirection = Directions::MovementLeft;
		}
		else if (Engine_Pointer->deviceKeyboard->GetKeyState(Engine_Pointer->keybindMovementRight)) {
			// Right movement
			spriteDirection = Directions::MovementRight;
		}
	}
	else if (Engine_Pointer->deviceKeyboard->GetKeyState(Engine_Pointer->keybindMovementLeft)) {
		// Left movement
		movementDirection = Directions::MovementLeft;
		spriteDirection = Directions::MovementLeft;
	}
	else if (Engine_Pointer->deviceKeyboard->GetKeyState(Engine_Pointer->keybindMovementRight)) {
		// Right movement
		movementDirection = Directions::MovementRight;
		spriteDirection = Directions::MovementRight;
	}
}
void Player::HandleInputGameController(void) {
	// TODO: Implement Controller support when Keyboard input is finished
}
