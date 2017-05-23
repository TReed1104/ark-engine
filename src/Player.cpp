#include "Player.h"
#include "Engine.h"

Player::Player(const std::string & scriptPath) : Entity(scriptPath) {
}
Player::~Player(void) {

}

void Player::HandleMovement(void) {
	if (Engine_Pointer->deviceGameControllerRegister.size() != 0) {
		HandleInput_WithController();
	}
	else {
		HandleInput_WithoutController();
	}
}
void Player::HandleInput_WithController(void) {
	// Reset the movement direction
	movementDirection = Player::NotSet;

	// Get the Thumbstick
	glm::vec2 leftThumbAxis = Engine_Pointer->deviceGameControllerRegister[Engine_Pointer->indexOfPlayerController]->GetThumbStickState(GameController::ThumbStickLeft);

	if ((leftThumbAxis.x < -Engine_Pointer->thumbStickDeadZone) || Engine_Pointer->deviceKeyboard->GetKeyState(Engine_Pointer->keybindMovementLeft)) {
		// Left movement
		movementDirection = Directions::Left;
		spriteDirection = Directions::Left;
	}
	else if ((leftThumbAxis.x > Engine_Pointer->thumbStickDeadZone) || Engine_Pointer->deviceKeyboard->GetKeyState(Engine_Pointer->keybindMovementRight)) {
		// Right movement
		movementDirection = Directions::Right;
		spriteDirection = Directions::Right;
	}
	if ((leftThumbAxis.y < -Engine_Pointer->thumbStickDeadZone) || Engine_Pointer->deviceKeyboard->GetKeyState(Engine_Pointer->keybindMovementUp)) {
		// Up movement
		if (movementDirection == Directions::Left) {
			movementDirection = Directions::UpLeft;
			spriteDirection = Directions::Left;
		}
		else if (movementDirection == Directions::Right) {
			movementDirection = Directions::UpRight;
			spriteDirection = Directions::Right;
		}
		else {
			movementDirection = Directions::Up;
			spriteDirection = Directions::Up;
		}
	}
	else if ((leftThumbAxis.y > Engine_Pointer->thumbStickDeadZone) || Engine_Pointer->deviceKeyboard->GetKeyState(Engine_Pointer->keybindMovementDown)) {
		// Down movement
		if (movementDirection == Directions::Left) {
			movementDirection = Directions::DownLeft;
			spriteDirection = Directions::Left;
		}
		else if (movementDirection == Directions::Right) {
			movementDirection = Directions::DownRight;
			spriteDirection = Directions::Right;
		}
		else {
			movementDirection = Directions::Down;
			spriteDirection = Directions::Down;
		}
	}
}
void Player::HandleInput_WithoutController(void) {
	// Reset the movement direction
	movementDirection = Directions::NotSet;

	if (Engine_Pointer->deviceKeyboard->GetKeyState(Engine_Pointer->keybindMovementLeft)) {
		// Left movement
		movementDirection = Directions::Left;
		spriteDirection = Directions::Left;
	}
	else if (Engine_Pointer->deviceKeyboard->GetKeyState(Engine_Pointer->keybindMovementRight)) {
		// Right movement
		movementDirection = Directions::Right;
		spriteDirection = Directions::Right;
	}
	if (Engine_Pointer->deviceKeyboard->GetKeyState(Engine_Pointer->keybindMovementUp)) {
		// Up movement
		if (movementDirection == Directions::Left) {
			movementDirection = Directions::UpLeft;
			spriteDirection = Directions::Left;
		}
		else if (movementDirection == Directions::Right) {
			movementDirection = Directions::UpRight;
			spriteDirection = Directions::Right;
		}
		else {
			movementDirection = Directions::Up;
			spriteDirection = Directions::Up;
		}
	}
	else if (Engine_Pointer->deviceKeyboard->GetKeyState(Engine_Pointer->keybindMovementDown)) {
		// Down movement
		if (movementDirection == Directions::Left) {
			movementDirection = Directions::DownLeft;
			spriteDirection = Directions::Left;
		}
		else if (movementDirection == Directions::Right) {
			movementDirection = Directions::DownRight;
			spriteDirection = Directions::Right;
		}
		else {
			movementDirection = Directions::Down;
			spriteDirection = Directions::Down;
		}
	}
}
