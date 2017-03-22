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
	~BoundingBox();

	bool Intersect(BoundingBox otherBB);
	void UpdatePosition(glm::vec2 newPosition);
	
	glm::vec2 GetPosition();
	glm::vec2 GetGridPosition();
	glm::vec2 GetDimensions();
	glm::vec2 TopLeftPosition();
	glm::vec2 TopRightPosition();
	glm::vec2 BottomLeftPosition();
	glm::vec2 BottomRightPosition();
	glm::vec2 TopLeftGridPosition();
	glm::vec2 TopRightGridPosition();
	glm::vec2 BottomLeftGridPosition();
	glm::vec2 BottomRightGridPosition();

private:
	float x;
	float y;
	float width;
	float height;

};

#endif