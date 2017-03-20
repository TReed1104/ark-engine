#include "Player.h"
#include "Engine.h"

Player::Player(const Model& model, const Texture& texture, const glm::vec3& position, const glm::vec2& sourceFrameSize) : Entity(model, texture, position, sourceFrameSize) {

}
Player::~Player() {

}

void Player::HandleMovement(void) {
	if (Engine_Pointer->gameController != nullptr) {
		HandleInput_WithController();
	}
	else {
		HandleInput_WithoutController();
	}
}

void Player::HandleInput_WithController(void) {
	// Handle the Controller button inputs

	// Main Buttons
	if (Engine_Pointer->gameController->GetButtonState(GameController::A)) {

	}
	else {

	}
	if (Engine_Pointer->gameController->GetButtonState(GameController::B)) {

	}
	else {

	}
	if (Engine_Pointer->gameController->GetButtonState(GameController::X)) {

	}
	else {

	}
	if (Engine_Pointer->gameController->GetButtonState(GameController::Y)) {

	}
	else {

	}

	// D-Pad
	if (Engine_Pointer->gameController->GetButtonState(GameController::DPadLeft)) {

	}
	else {

	}
	if (Engine_Pointer->gameController->GetButtonState(GameController::DPadRight)) {

	}
	else {

	}
	if (Engine_Pointer->gameController->GetButtonState(GameController::DPadUp)) {

	}
	else {

	}
	if (Engine_Pointer->gameController->GetButtonState(GameController::DPadDown)) {

	}
	else {

	}

	// Triggers
	if (Engine_Pointer->gameController->GetTriggerState(GameController::TriggerLeft)) {

	}
	else {

	}
	if (Engine_Pointer->gameController->GetTriggerState(GameController::TriggerRight)) {

	}
	else {

	}

	// Reset the movement direction
	movementDirection = Player::NotSet;

	// Thumbsticks
	glm::vec2 leftThumbAxis = Engine_Pointer->gameController->GetThumbStickState(GameController::ThumbStickLeft);

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

	// Right Thumbstick
	glm::vec2 rightThumbAxis = Engine_Pointer->gameController->GetThumbStickState(GameController::ThumbStickRight);
	if (rightThumbAxis.x < -Engine_Pointer->thumbStickDeadZone) {
		// Right Thumb Left

	}
	else if (rightThumbAxis.x > Engine_Pointer->thumbStickDeadZone) {
		// Right Thumb Right

	}
	if (rightThumbAxis.y < -Engine_Pointer->thumbStickDeadZone) {
		// Right Thumb Up

	}
	else if (rightThumbAxis.y > Engine_Pointer->thumbStickDeadZone) {
		// Right Thumb Down

	}
}

void Player::HandleInput_WithoutController(void) {
	
}
