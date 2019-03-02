#include "SoundEffect.h"
#include "Engine.h"

Engine* SoundEffect::Engine_Pointer;

SoundEffect::SoundEffect(const std::string & soundName, const std::string & filePath) {
}
SoundEffect::~SoundEffect() {
}

const std::string SoundEffect::GetName(void) {
	return std::string();
}

const bool SoundEffect::IsLoaded(void) {
	return false;
}

bool SoundEffect::LoadBuffer(void) {
	return false;
}
