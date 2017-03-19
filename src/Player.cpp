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

	// Thumbsticks
	glm::vec2 leftThumbAxis = s_EnginePointer->playerController->GetThumbStickState(GameController::ThumbStickLeft);
	glm::vec2 rightThumbAxis = s_EnginePointer->playerController->GetThumbStickState(GameController::ThumbStickRight);
	if (leftThumbAxis.x < -s_EnginePointer->thumbStickDeadZone) {
		// Left Thumb Left
		std::cout << "Left Thumb Left" << std::endl;
	}
	else if (leftThumbAxis.x > s_EnginePointer->thumbStickDeadZone) {
		// Left Thumb Right
		std::cout << "Left Thumb Right" << std::endl;
	}
	if (leftThumbAxis.y < -s_EnginePointer->thumbStickDeadZone) {
		// Left Thumb Up
		std::cout << "Left Thumb Up" << std::endl;
	}
	else if (leftThumbAxis.y > s_EnginePointer->thumbStickDeadZone) {
		// Left Thumb Down
		std::cout << "Left Thumb Down" << std::endl;
	}
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
