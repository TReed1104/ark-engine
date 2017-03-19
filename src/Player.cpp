#include "Player.h"
#include "Engine.h"

Player::Player(const Model& model, const Texture& texture, const glm::vec3& position, const glm::vec2& sourceFrameSize) : Entity(model, texture, position, sourceFrameSize) {

}
Player::~Player() {

}

void Player::HandleMovement(void) {
	// Handle the Controller button inputs

	// Main Buttons
	if (Engine_Pointer->playerController->GetButtonState(GameController::A)) {

	}
	else {

	}
	if (Engine_Pointer->playerController->GetButtonState(GameController::B)) {

	}
	else {

	}
	if (Engine_Pointer->playerController->GetButtonState(GameController::X)) {

	}
	else {

	}
	if (Engine_Pointer->playerController->GetButtonState(GameController::Y)) {

	}
	else {

	}

	// D=Pad
	if (Engine_Pointer->playerController->GetButtonState(GameController::DPadLeft)) {

	}
	else {

	}
	if (Engine_Pointer->playerController->GetButtonState(GameController::DPadRight)) {

	}
	else {

	}
	if (Engine_Pointer->playerController->GetButtonState(GameController::DPadUp)) {

	}
	else {

	}
	if (Engine_Pointer->playerController->GetButtonState(GameController::DPadDown)) {

	}
	else {

	}

	// Triggers
	if (Engine_Pointer->playerController->GetTriggerState(GameController::TriggerLeft)) {

	}
	else {

	}
	if (Engine_Pointer->playerController->GetTriggerState(GameController::TriggerRight)) {

	}
	else {

	}

	// Reset the movement direction
	movementDirection = Player::NotSet;

	// Thumbsticks
	glm::vec2 leftThumbAxis = Engine_Pointer->playerController->GetThumbStickState(GameController::ThumbStickLeft);
	glm::vec2 rightThumbAxis = Engine_Pointer->playerController->GetThumbStickState(GameController::ThumbStickRight);
	
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

	switch (movementDirection) {
		case Entity::NotSet:
			velocity = glm::vec2(0, 0);
			break;
		case Entity::Up:
			velocity = glm::vec2(0, -movementSpeed);
			break;
		case Entity::Down:
			velocity = glm::vec2(0, movementSpeed);
			break;
		case Entity::Left:
			velocity = glm::vec2(-movementSpeed, 0);
			break;
		case Entity::Right:
			velocity = glm::vec2(movementSpeed, 0);
			break;
		case Entity::UpLeft:
			velocity = glm::vec2(-movementSpeed, -movementSpeed);
			break;
		case Entity::UpRight:
			velocity = glm::vec2(movementSpeed, -movementSpeed);
			break;
		case Entity::DownLeft:
			velocity = glm::vec2(-movementSpeed, movementSpeed);
			break;
		case Entity::DownRight:
			velocity = glm::vec2(movementSpeed, movementSpeed);
			break;
		default:
			break;
	}

	// Right Thumbstick
	if (rightThumbAxis.x < -Engine_Pointer->thumbStickDeadZone) {
		// Right Thumb Left
		std::cout << "Right Thumb Left" << std::endl;
	}
	else if (rightThumbAxis.x > Engine_Pointer->thumbStickDeadZone) {
		// Right Thumb Right
		std::cout << "Right Thumb Right" << std::endl;
	}
	if (rightThumbAxis.y < -Engine_Pointer->thumbStickDeadZone) {
		// Right Thumb Up
		std::cout << "Right Thumb Up" << std::endl;
	}
	else if (rightThumbAxis.y > Engine_Pointer->thumbStickDeadZone) {
		// Right Thumb Down
		std::cout << "Right Thumb Down" << std::endl;
	}
}
