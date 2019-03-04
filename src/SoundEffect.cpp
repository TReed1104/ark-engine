#include "SoundEffect.h"
#include "Engine.h"

Engine* SoundEffect::Engine_Pointer;

SoundEffect::SoundEffect(const std::string& soundName, const std::string& filePath) {
	name = soundName;
	isLoaded = LoadBuffer(filePath);
}
SoundEffect::~SoundEffect() {
	alDeleteBuffers(1, &alBuffer);
}

const std::string SoundEffect::GetName(void) {
	return name;
}

const bool SoundEffect::IsLoaded(void) {
	return isLoaded;
}

bool SoundEffect::LoadBuffer(const std::string& filePath) {
	Uint32 wavLength;			// length of our sample
	Uint8* wavData;				// buffer containing our audio file
	SDL_AudioSpec wavSpec;		// the specs of our piece of music

	if (SDL_LoadWAV(filePath.c_str(), &wavSpec, &wavData, &wavLength) == NULL) {
		Engine_Pointer->engineDebugger.WriteLine(">>>> ERROR!!!! - SDL_LoadWav Error: " + (std::string)SDL_GetError());
		Engine_Pointer->engineDebugger.WriteLine(">>>> ERROR!!!! - Failed to load: " + name);
		return false;
	}

	ALenum wavFormat;
	switch (wavSpec.format) {
	case AUDIO_U8:
		// Fall through
	case AUDIO_S8:
		wavFormat = wavSpec.channels == 2 ? AL_FORMAT_STEREO8 : AL_FORMAT_MONO8;
		break;
	case AUDIO_U16:
		// Fall through
	case AUDIO_S16:
		wavFormat = wavSpec.channels == 2 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
		break;
	default:
		// ERROR
		Engine_Pointer->engineDebugger.WriteLine(">>>> ERROR!!!! - .Wav Format Error: " + (std::string)SDL_GetError());
		Engine_Pointer->engineDebugger.WriteLine(">>>> ERROR!!!! - Failed to load: " + name);
		SDL_FreeWAV(wavData);
		return false;
	}

	alBufferData(alBuffer, wavFormat, wavData, wavLength, wavSpec.freq);
	SDL_FreeWAV(wavData);	// Clean up the SDL buffer now we are done

	return true;
}
