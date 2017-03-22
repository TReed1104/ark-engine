#ifndef ARKENGINE_BOUNDINGBOX_H_
#define ARKENGINE_BOUNDINGBOX_H_
#include "glm/glm.hpp"

class Engine;

class BoundingBox
{
public:
	static Engine* Engine_Pointer;

	float x;
	float y;
	float width;
	float height;

	BoundingBox(float x = 0.0f, float y = 0.0f, float width = 0.0f, float height = 0.0f);
	BoundingBox(glm::vec2 position);
	BoundingBox(glm::vec2 position, glm::vec2 dimensions);
	~BoundingBox();

	bool Intersect(BoundingBox otherBB);
	void UpdatePosition(glm::vec2 newPosition);

private:

};

#endif