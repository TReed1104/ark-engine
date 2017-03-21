#include "Player.h"
#include "Engine.h"

Player::Player(const Model& model, const Texture& texture, const glm::vec3& position, const glm::vec2& sourceFrameSize) : Entity(model, texture, position, sourceFrameSize) {
	keybindMovementUp = Keyboard::w;
	keybindMovementDown = Keyboard::s;
	keybindMovementLeft = Keyboard::a;
	keybindMovementRight = Keyboard::d;
}
Player::~Player() {

}

void Player::HandleMovement(void) {
	if (Engine_Pointer->deviceGameController != nullptr) {
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
	glm::vec2 leftThumbAxis = Engine_Pointer->deviceGameController->GetThumbStickState(GameController::ThumbStickLeft);

	if ((leftThumbAxis.x < -Engine_Pointer->thumbStickDeadZone) || Engine_Pointer->deviceKeyboard->GetKeyState(keybindMovementLeft)) {
		// Left movement
		movementDirection = Player::Left;
		spriteDirection = Player::Left;
	}
	else if ((leftThumbAxis.x > Engine_Pointer->thumbStickDeadZone) || Engine_Pointer->deviceKeyboard->GetKeyState(keybindMovementRight)) {
		// Right movement
		movementDirection = Player::Right;
		spriteDirection = Player::Right;
	}
	if ((leftThumbAxis.y < -Engine_Pointer->thumbStickDeadZone) || Engine_Pointer->deviceKeyboard->GetKeyState(keybindMovementUp)) {
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
	else if ((leftThumbAxis.y > Engine_Pointer->thumbStickDeadZone) || Engine_Pointer->deviceKeyboard->GetKeyState(keybindMovementDown)) {
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

	if (Engine_Pointer->deviceKeyboard->GetKeyState(keybindMovementLeft)) {
		// Left movement
		movementDirection = Player::Left;
		spriteDirection = Player::Left;
	}
	else if (Engine_Pointer->deviceKeyboard->GetKeyState(keybindMovementRight)) {
		// Right movement
		movementDirection = Player::Right;
		spriteDirection = Player::Right;
	}
	if (Engine_Pointer->deviceKeyboard->GetKeyState(keybindMovementUp)) {
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
	else if (Engine_Pointer->deviceKeyboard->GetKeyState(keybindMovementDown)) {
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
