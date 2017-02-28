#ifndef ARKENGINE_PLAYER_H_
#define ARKENGINE_PLAYER_H_
#include "Entity.h"

class Player : public Entity
{
public:
	Player(const Engine& engine, const Model& model, const glm::vec3& position = glm::vec3(0.0f), const char* texturePath = "", const glm::vec2& sourceFrameSize = glm::vec2(TILE_SIZE_X, TILE_SIZE_Y));
	~Player();
	
	void Update(float deltaTime);

private:

};

#endif