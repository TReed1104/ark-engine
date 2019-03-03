#include "SoundEffect.h"
#include "Engine.h"

Engine* SoundEffect::Engine_Pointer;

SoundEffect::SoundEffect(const std::string& soundName, const std::string& filePath) {
	name = soundName;
	isLoaded = LoadBuffer(filePath);
}
SoundEffect::~SoundEffect() {
	SDL_FreeWAV(wav_buffer);
}

const std::string SoundEffect::GetName(void) {
	return name;
}

const bool SoundEffect::IsLoaded(void) {
	return isLoaded;
}

bool SoundEffect::LoadBuffer(const std::string& filePath) {
	if (SDL_LoadWAV(filePath.c_str(), &wav_spec, &wav_buffer, &wav_length) == NULL) {
		Engine_Pointer->engineDebugger.WriteLine(">>>> ERROR!!!! - SDL_LoadWav Error: " + (std::string)SDL_GetError());
		Engine_Pointer->engineDebugger.WriteLine(">>>> ERROR!!!! - Failed to load: " + name);
		return false;
	}

	return false;
}
