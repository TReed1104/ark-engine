#include "Animation.h"

Engine* Animation::Engine_Pointer;
Engine* Animation::Frame::Engine_Pointer;

Animation::Frame::Frame(const glm::vec2& sourceRectanglePosition, const float& frameLength) {
	this->sourceRectanglePosition = sourceRectanglePosition;
	this->frameLength = frameLength;
}
Animation::Frame::~Frame() {

}

Animation::Animation(const std::string & name) {
	this->name = name;
}
Animation::~Animation() {

}

void Animation::AddFrame(const glm::vec2 & sourceRectanglePosition, const float & frameLength) {
	frames.push_back(Frame(sourceRectanglePosition, frameLength));
}