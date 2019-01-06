#ifndef ARKENGINE_BOUNDINGBOX_H_
#define ARKENGINE_BOUNDINGBOX_H_
#include "glm/glm.hpp"

class Engine;

class BoundingBox
{
public:
	static Engine* Engine_Pointer;

	BoundingBox(float x = 0.0f, float y = 0.0f, float width = 0.0f, float height = 0.0f);
	BoundingBox(glm::vec2 position);
	BoundingBox(glm::vec2 position, glm::vec2 dimensions);
	~BoundingBox(void);

	bool Intersect(BoundingBox otherBB);
	void UpdatePosition(const glm::vec2& newPosition);
	void Resize(const glm::vec2& newDimensions);
	
	glm::vec2 GetPosition(void);
	glm::ivec2 GetGridPosition(void);
	glm::ivec2 GetDimensions(void);
	glm::ivec2 TopLeftPosition(void);
	glm::ivec2 TopRightPosition(void);
	glm::ivec2 BottomLeftPosition(void);
	glm::ivec2 BottomRightPosition(void);
	glm::ivec2 TopLeftGridPosition(void);
	glm::ivec2 TopRightGridPosition(void);
	glm::ivec2 BottomLeftGridPosition(void);
	glm::ivec2 BottomRightGridPosition(void);

private:
	float x;
	float y;
	float width;
	float height;

};

#endif