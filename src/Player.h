#ifndef ARKENGINE_PLAYER_H_
#define ARKENGINE_PLAYER_H_
#include "Entity.h"
#include "Keyboard.h"

class Engine;

class Player : public Entity
{
public:
	

	Player(const Model& model, const Texture& texture, const glm::vec3& position = glm::vec3(0.0f), const glm::vec2& sourceFrameSize = glm::vec2(16, 16));
	~Player();
	
	void HandleMovement(void);
	void HandleInput_WithController(void);
	void HandleInput_WithoutController(void);

private:

};

#endif