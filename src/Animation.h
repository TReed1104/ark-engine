#ifndef ARKENGINE_ANIMATION_H_
#define ARKENGINE_ANIMATION_H_

#include <string>
#include <vector>
#include <glm/glm.hpp>

class Engine;

class Animation {
public:
	static Engine* Engine_Pointer;

	// Animation Frame class
	class Frame {
	public:
		static Engine* Engine_Pointer;

		glm::ivec2 sourceRectanglePosition;
		float frameLength;

		Frame(const glm::ivec2& sourceRectanglePosition, const float& frameLength);
		~Frame();

	private:

	};

	Animation(const std::string& name = "");
	~Animation(void);

	const std::string GetName(void);
	void AddFrame(const glm::ivec2& sourceRectanglePosition, const float& frameLength);
	glm::ivec2 Run(const float& deltaTime);
	void Reset(void);

private:
	std::string name;
	std::vector<Frame> frames;
	int animationIndex;
	float animationTimer;

};

#endif
