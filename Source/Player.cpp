#include "Player.h"

Player::Player(const Engine& engine, const Model& model, const glm::vec3& position, const char* texturePath, const glm::vec2& sourceFrameSize) : GameObject(engine, model, position, texturePath, sourceFrameSize) {

}

Player::~Player() {

}

void Player::Update(float deltaTime) {
	
	
	// Calls the base class update.
	GameObject::Update(deltaTime);
}
