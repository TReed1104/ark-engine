#include "Player.h"
#include "Engine.h"

Player::Player(const Model& model, const Texture& texture, const glm::vec3& position, const glm::vec2& sourceFrameSize) : Entity(model, texture, position, sourceFrameSize) {

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

	// Thumbsticks
	glm::vec2 leftThumbAxis = Engine_Pointer->deviceGameController->GetThumbStickState(GameController::ThumbStickLeft);

	// Left Thumbstick
	if (leftThumbAxis.x < -Engine_Pointer->thumbStickDeadZone) {
		// Left Thumb Left
		movementDirection = Player::Left;
		spriteDirection = Player::Left;
	}
	else if (leftThumbAxis.x > Engine_Pointer->thumbStickDeadZone) {
		// Left Thumb Right
		movementDirection = Player::Right;
		spriteDirection = Player::Right;
	}
	if (leftThumbAxis.y < -Engine_Pointer->thumbStickDeadZone) {
		// Left Thumb Up
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
	else if (leftThumbAxis.y > Engine_Pointer->thumbStickDeadZone) {
		// Left Thumb Down
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
	
}
