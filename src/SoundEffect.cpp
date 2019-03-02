#include "SoundEffect.h"
#include "Engine.h"

Engine* SoundEffect::Engine_Pointer;

SoundEffect::SoundEffect(const std::string & soundName, const std::string & filePath) {
}
SoundEffect::~SoundEffect() {
}

bool SoundEffect::LoadBuffer(void) {
	return false;
}
