#include "Animation.h"
#include "Engine.h"

Engine* Animation::Engine_Pointer;
Engine* Animation::Frame::Engine_Pointer;

Animation::Frame::Frame(const glm::vec2& sourceRectanglePosition, const float& frameLength) {
	this->sourceRectanglePosition = sourceRectanglePosition;
	this->frameLength = frameLength;
}
Animation::Frame::~Frame(void) {

}

Animation::Animation(const std::string & name) {
	this->name = name;
	animationTimer = 0;
	animationIndex = 0;
}
Animation::~Animation(void) {

}

void Animation::AddFrame(const glm::vec2 & sourceRectanglePosition, const float & frameLength) {
	frames.push_back(Frame(sourceRectanglePosition, frameLength));
}
glm::vec2 Animation::Run(const float & deltaTime) {
	if (frames.size() > 0) {
		animationTimer += deltaTime;
		if (animationTimer >= frames[animationIndex].frameLength) {
			animationIndex = (animationIndex + 1) % frames.size();
			animationTimer = 0;
		}
		return frames[animationIndex].sourceRectanglePosition;
	}
	else {
		return glm::vec2(0, 0);
	}
}
