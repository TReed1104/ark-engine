#ifndef ARKENGINE_ANIMATION_H_
#define ARKENGINE_ANIMATION_H_

#include <string>
#include <vector>
#include <glm/glm.hpp>

class Engine;

// Animation class
class Animation
{
public:
	static Engine* Engine_Pointer;

	// Animation Frame class
	class Frame
	{
	public:
		static Engine* Engine_Pointer;

		glm::vec2 sourceRectanglePosition;
		float frameLength;

		Frame(const glm::vec2& sourceRectanglePosition, const float& frameLength);
		~Frame();

	private:

	};

	std::string name;
	std::vector<Frame> frames;

	Animation(const std::string& name);
	~Animation();

	void AddFrame(const glm::vec2& sourceRectanglePosition, const float& frameLength);

private:

};

#endif
