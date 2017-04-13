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
	
	glm::vec2 GetPosition(void);
	glm::vec2 GetGridPosition(void);
	glm::vec2 GetDimensions(void);
	glm::vec2 TopLeftPosition(void);
	glm::vec2 TopRightPosition(void);
	glm::vec2 BottomLeftPosition(void);
	glm::vec2 BottomRightPosition(void);
	glm::vec2 TopLeftGridPosition(void);
	glm::vec2 TopRightGridPosition(void);
	glm::vec2 BottomLeftGridPosition(void);
	glm::vec2 BottomRightGridPosition(void);

private:
	float x;
	float y;
	float width;
	float height;

};

#endif