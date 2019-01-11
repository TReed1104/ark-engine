#ifndef ARKENGINE_TILE_H_
#define ARKENGINE_TILE_H_
#include "GameObject.h"

class Tile : public GameObject
{
public:
	enum Type { Walkable, Solid, };
	Type type;
	bool isSlope;
	glm::vec2 slopeOffset;	// x = left, y = right
	float slopeAngle;

	Tile(const Texture& texture, const int& tileType, const glm::ivec2& sourceFramePosition = glm::ivec2(0), const glm::vec3& position = glm::vec3(0.0f), const BoundingBox& boundingBox = BoundingBox(glm::ivec2(0)), const glm::ivec2& boundingBoxOffset = glm::ivec2(0), const bool& isSlope = false, const glm::ivec2& slopeOffset = glm::vec2(0));
	~Tile(void);

	void Update(const float& deltaTime);

private:
	float CalculateSlope(const glm::ivec2& slopeOffset);
};
#endif