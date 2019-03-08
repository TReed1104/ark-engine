#include "Player.h"
#include "Engine.h"

Player::Player(const std::string& filePath) : Entity(filePath) {

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

	// Move Left/Right
	if (Engine_Pointer->deviceKeyboard->GetKeyState(Engine_Pointer->keybindingHandler.GetKeybind("Move Right"))) {
		movementDirection = Directions::Right;
		spriteDirection = Directions::Right;
	}
	else if (Engine_Pointer->deviceKeyboard->GetKeyState(Engine_Pointer->keybindingHandler.GetKeybind("Move Left"))) {
		movementDirection = Directions::Left;
		spriteDirection = Directions::Left;
	}

	// Jumping
	if (Engine_Pointer->deviceKeyboard->GetKeyState(Engine_Pointer->keybindingHandler.GetKeybind("Jump"))) {
		if (!isJumping && !isFalling && !isCrawling) {
			isJumping = true;
			currentJumpingSpeed = baseJumpingSpeed;
			movementDirection = Directions::Up;

			// TEST CALL FOR PLAYING THE JUMP SOUND EFFECT
			std::string jumpSoundEffectId = "player jump";
			int indexOfJumpSoundEffect = Engine_Pointer->GetIndexOfSoundEffect(jumpSoundEffectId);
			if (indexOfJumpSoundEffect != -1) {
				Engine_Pointer->soundEffectRegister[indexOfJumpSoundEffect]->Play();
			}
			else {
				Engine_Pointer->engineDebugger.WriteLine(">> ERROR!!!! - Could not find sound effect named: " + jumpSoundEffectId);
			}
			// END OF TEST CALL
		}
	}

	// Crawling
	if (Engine_Pointer->deviceKeyboard->GetKeyState(Engine_Pointer->keybindingHandler.GetKeybind("Crawl"))) {
		if (canCrawl) {
			if (isJumping) {
				isJumping = false;
				currentJumpingSpeed = 0.0f;
				movementDirection = Directions::Down;
			}
			else if (!isJumping && !isFalling) {
				isTryingToCrawl = true;
				isCrawling = true;
			}
		}
	}
	else {
		isTryingToCrawl = false;
	}

	// Sprinting
	if (Engine_Pointer->deviceKeyboard->GetKeyState(Engine_Pointer->keybindingHandler.GetKeybind("Sprint"))) {
		if (!isJumping && !isFalling) {
			currentMovementSpeed = maxMovementSpeed;
		}
	}
	else {
		currentMovementSpeed = baseMovementSpeed;
	}
}
void Player::HandleInputGameController(void) {
	// TODO: Implement Controller support when Keyboard input is finished
}
