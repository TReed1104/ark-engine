#include "SoundEffect.h"
#include "Engine.h"

Engine* SoundEffect::Engine_Pointer;

SoundEffect::SoundEffect(const std::string& soundName, const std::string& filePath) {
	name = soundName;
	isLoaded = LoadBuffer(filePath);
}
SoundEffect::~SoundEffect() {
}

const std::string SoundEffect::GetName(void) {
	return name;
}

const bool SoundEffect::IsLoaded(void) {
	return isLoaded;
}

bool SoundEffect::LoadBuffer(const std::string& filePath) {
	return false;
}
