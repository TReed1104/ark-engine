#include "Player.h"

Player::Player(const Model& model, const Texture& texture, const glm::vec3& position, const glm::vec2& sourceFrameSize) : Entity(model, texture, position, sourceFrameSize) {

}
Player::~Player() {

}

void Player::Update(float deltaTime) {
	
	
	// Calls the base class update.
	Entity::Update(deltaTime);
}
