#ifndef ARKENGINE_PLAYER_H_
#define ARKENGINE_PLAYER_H_
#include "Entity.h"

class Player : public Entity
{
public:
	Player(const Engine& engine, const Model& model, const Texture& texture, const glm::vec3& position = glm::vec3(0.0f), const glm::vec2& sourceFrameSize = glm::vec2(16, 16));
	~Player();
	
	void Update(float deltaTime);

private:

};

#endif