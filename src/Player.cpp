#include "Player.h"
#include "Engine.h"

Player::Player(const Model& model, const Texture& texture, const glm::vec3& position, const glm::vec2& sourceFrameSize) : Entity(model, texture, position, sourceFrameSize) {

}
Player::~Player() {

}

void Player::HandleMovement(void) {
	// Handle the Controller button inputs

	// Main Buttons
	if (s_EnginePointer->playerController->GetButtonState(GameController::A)) {

	}
	else {

	}
	if (s_EnginePointer->playerController->GetButtonState(GameController::B)) {

	}
	else {

	}
	if (s_EnginePointer->playerController->GetButtonState(GameController::X)) {

	}
	else {

	}
	if (s_EnginePointer->playerController->GetButtonState(GameController::Y)) {

	}
	else {

	}

	// D=Pad
	if (s_EnginePointer->playerController->GetButtonState(GameController::DPadLeft)) {

	}
	else {

	}
	if (s_EnginePointer->playerController->GetButtonState(GameController::DPadRight)) {

	}
	else {

	}
	if (s_EnginePointer->playerController->GetButtonState(GameController::DPadUp)) {

	}
	else {

	}
	if (s_EnginePointer->playerController->GetButtonState(GameController::DPadDown)) {

	}
	else {

	}

	// Triggers
	if (s_EnginePointer->playerController->GetTriggerState(GameController::TriggerLeft)) {

	}
	else {

	}
	if (s_EnginePointer->playerController->GetTriggerState(GameController::TriggerRight)) {

	}
	else {

	}

	// Store the old direction and check the new direction
	MovementDirection oldDirection = movementDirection;
	movementDirection = Player::NotSet;

	// Thumbsticks
	glm::vec2 leftThumbAxis = s_EnginePointer->playerController->GetThumbStickState(GameController::ThumbStickLeft);
	glm::vec2 rightThumbAxis = s_EnginePointer->playerController->GetThumbStickState(GameController::ThumbStickRight);
	
	// Left Thumbstick
	if (leftThumbAxis.x < -s_EnginePointer->thumbStickDeadZone) {
		// Left Thumb Left
		movementDirection = Player::Left;
	}
	else if (leftThumbAxis.x > s_EnginePointer->thumbStickDeadZone) {
		// Left Thumb Right
		movementDirection = Player::Right;
	}
	if (leftThumbAxis.y < -s_EnginePointer->thumbStickDeadZone) {
		// Left Thumb Up
		if (movementDirection == Player::Left) {
			movementDirection = Player::UpLeft;
		}
		else if (movementDirection == Player::Right) {
			movementDirection = Player::UpRight;
		}
		else {
			movementDirection = Player::Up;
		}
	}
	else if (leftThumbAxis.y > s_EnginePointer->thumbStickDeadZone) {
		// Left Thumb Down
		if (movementDirection == Player::Left) {
			movementDirection = Player::DownLeft;
		}
		else if (movementDirection == Player::Right) {
			movementDirection = Player::DownRight;
		}
		else {
			movementDirection = Player::Down;
		}
	}

	// If the direction was not changed, use the oldDirection
	if (movementDirection == Player::NotSet) {
		movementDirection = oldDirection;
	}

	std::string directionToPrint = "";
	switch (movementDirection) {
		case Entity::NotSet:
			directionToPrint = "Not Set";
			break;
		case Entity::Up:
			directionToPrint = "Up";
			break;
		case Entity::Down:
			directionToPrint = "Down";
			break;
		case Entity::Left:
			directionToPrint = "Left";
			break;
		case Entity::Right:
			directionToPrint = "Right";
			break;
		case Entity::UpLeft:
			directionToPrint = "Up Left";
			break;
		case Entity::UpRight:
			directionToPrint = "Up Right";
			break;
		case Entity::DownLeft:
			directionToPrint = "Down Left";
			break;
		case Entity::DownRight:
			directionToPrint = "Down Right";
			break;
		default:
			break;
	}
	std::cout << directionToPrint << std::endl;

	// Right Thumbstick
	if (rightThumbAxis.x < -s_EnginePointer->thumbStickDeadZone) {
		// Right Thumb Left
		std::cout << "Right Thumb Left" << std::endl;
	}
	else if (rightThumbAxis.x > s_EnginePointer->thumbStickDeadZone) {
		// Right Thumb Right
		std::cout << "Right Thumb Right" << std::endl;
	}
	if (rightThumbAxis.y < -s_EnginePointer->thumbStickDeadZone) {
		// Right Thumb Up
		std::cout << "Right Thumb Up" << std::endl;
	}
	else if (rightThumbAxis.y > s_EnginePointer->thumbStickDeadZone) {
		// Right Thumb Down
		std::cout << "Right Thumb Down" << std::endl;
	}
}
