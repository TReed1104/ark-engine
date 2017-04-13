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
		movementDirection = Player::Left;
		spriteDirection = Player::Left;
	}
	else if ((leftThumbAxis.x > Engine_Pointer->thumbStickDeadZone) || Engine_Pointer->deviceKeyboard->GetKeyState(Engine_Pointer->keybindMovementRight)) {
		// Right movement
		movementDirection = Player::Right;
		spriteDirection = Player::Right;
	}
	if ((leftThumbAxis.y < -Engine_Pointer->thumbStickDeadZone) || Engine_Pointer->deviceKeyboard->GetKeyState(Engine_Pointer->keybindMovementUp)) {
		// Up movement
		if (movementDirection == Player::Left) {
			movementDirection = Player::UpLeft;
			spriteDirection = Player::UpLeft;
		}
		else if (movementDirection == Player::Right) {
			movementDirection = Player::UpRight;
			spriteDirection = Player::UpRight;
		}
		else {
			movementDirection = Player::Up;
			spriteDirection = Player::Up;
		}
	}
	else if ((leftThumbAxis.y > Engine_Pointer->thumbStickDeadZone) || Engine_Pointer->deviceKeyboard->GetKeyState(Engine_Pointer->keybindMovementDown)) {
		// Down movement
		if (movementDirection == Player::Left) {
			movementDirection = Player::DownLeft;
			spriteDirection = Player::DownLeft;
		}
		else if (movementDirection == Player::Right) {
			movementDirection = Player::DownRight;
			spriteDirection = Player::DownRight;
		}
		else {
			movementDirection = Player::Down;
			spriteDirection = Player::Down;
		}
	}
}
void Player::HandleInput_WithoutController(void) {
	// Reset the movement direction
	movementDirection = Player::NotSet;

	if (Engine_Pointer->deviceKeyboard->GetKeyState(Engine_Pointer->keybindMovementLeft)) {
		// Left movement
		movementDirection = Player::Left;
		spriteDirection = Player::Left;
	}
	else if (Engine_Pointer->deviceKeyboard->GetKeyState(Engine_Pointer->keybindMovementRight)) {
		// Right movement
		movementDirection = Player::Right;
		spriteDirection = Player::Right;
	}
	if (Engine_Pointer->deviceKeyboard->GetKeyState(Engine_Pointer->keybindMovementUp)) {
		// Up movement
		if (movementDirection == Player::Left) {
			movementDirection = Player::UpLeft;
			spriteDirection = Player::UpLeft;
		}
		else if (movementDirection == Player::Right) {
			movementDirection = Player::UpRight;
			spriteDirection = Player::UpRight;
		}
		else {
			movementDirection = Player::Up;
			spriteDirection = Player::Up;
		}
	}
	else if (Engine_Pointer->deviceKeyboard->GetKeyState(Engine_Pointer->keybindMovementDown)) {
		// Down movement
		if (movementDirection == Player::Left) {
			movementDirection = Player::DownLeft;
			spriteDirection = Player::DownLeft;
		}
		else if (movementDirection == Player::Right) {
			movementDirection = Player::DownRight;
			spriteDirection = Player::DownRight;
		}
		else {
			movementDirection = Player::Down;
			spriteDirection = Player::Down;
		}
	}
}
