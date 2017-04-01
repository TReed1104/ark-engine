#ifndef ARKENGINE_TILE_H_
#define ARKENGINE_TILE_H_
#include "GameObject.h"

class Tile : public GameObject
{
public:
	enum Type { Walkable, Solid, };
	Type type;

	Tile(const Texture& texture, const int& tileType, const glm::vec2& sourceFramePosition = glm::vec2(0.0f), const glm::vec3& position = glm::vec3(0.0f));
	~Tile();

	void Update(const float& deltaTime);

private:

};
#endif