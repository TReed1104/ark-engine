#include "SoundEffect.h"
#include "Engine.h"

Engine* SoundEffect::Engine_Pointer;

SoundEffect::SoundEffect(const std::string& soundName, const std::string& filePath) {
	name = soundName;
	isLoaded = LoadSource() && LoadBuffer(filePath);

	// If the source and buffer are loaded, bind the buffer to the source
	if (isLoaded) {
		alSourcei(alSource, AL_BUFFER, alBuffer);
		isLoaded = !CheckOpenALErrors();
	}
	else {
		Engine_Pointer->engineDebugger.WriteLine(">>>> ERROR!!!! - Failed to load Audio File: " + filePath);
		return;
	}
}
SoundEffect::~SoundEffect() {
	this->Stop();
	alDeleteSources(1, &alSource);
	alDeleteBuffers(1, &alBuffer);
}

const std::string SoundEffect::GetName(void) {
	return name;
}
const bool SoundEffect::IsLoaded(void) {
	return isLoaded;
}
void SoundEffect::Play(void) {
	ALint sourceState;
	alGetSourcei(alSource, AL_SOURCE_STATE, &sourceState);
	
	// If the source is not playing, play it
	if (sourceState != AL_PLAYING) {
		alSourcePlay(alSource);
	}
}
void SoundEffect::Pause(void) {
	ALint sourceState;
	alGetSourcei(alSource, AL_SOURCE_STATE, &sourceState);

	// if the source is playing, pause it
	if (sourceState == AL_PLAYING) {
		alSourcePause(alSource);
	}
}
void SoundEffect::Stop(void) {
	ALint sourceState;
	alGetSourcei(alSource, AL_SOURCE_STATE, &sourceState);

	// if the source is not stopped, stop it
	if (sourceState != AL_STOPPED) {
		alSourceStop(alSource);
	}
}
bool SoundEffect::CheckOpenALErrors(void) {
	ALenum errorCode = alGetError();
	switch (errorCode) {
	case AL_NO_ERROR:
		return false;
	case AL_INVALID_NAME:
		Engine_Pointer->engineDebugger.WriteLine(">>>> ERROR!!!! - OpenAL: AL_INVALID_NAME");
		return true;
	case AL_INVALID_ENUM:
		Engine_Pointer->engineDebugger.WriteLine(">>>> ERROR!!!! - OpenAL: AL_INVALID_ENUM");
		return true;
	case AL_INVALID_VALUE:
		Engine_Pointer->engineDebugger.WriteLine(">>>> ERROR!!!! - OpenAL: AL_INVALID_VALUE");
		return true;
	case AL_INVALID_OPERATION:
		Engine_Pointer->engineDebugger.WriteLine(">>>> ERROR!!!! - OpenAL: AL_INVALID_OPERATION");
		return true;
	case AL_OUT_OF_MEMORY:
		Engine_Pointer->engineDebugger.WriteLine(">>>> ERROR!!!! - OpenAL: AL_OUT_OF_MEMORY");
		return true;
	default:
		return false;
	}
}
bool SoundEffect::LoadSource(void) {
	alGenSources((ALuint)1, &alSource);
	alSourcef(alSource, AL_PITCH, 1);
	alSourcef(alSource, AL_GAIN, 1);
	alSource3f(alSource, AL_POSITION, 0, 0, 0);
	alSource3f(alSource, AL_VELOCITY, 0, 0, 0);
	alSourcei(alSource, AL_LOOPING, AL_FALSE);
	return !CheckOpenALErrors();
}
bool SoundEffect::LoadSource(const float& pitch, const float& gain, const glm::vec3& position, const glm::vec3& velocity, const bool& isLooping) {
	alGenSources((ALuint)1, &alSource);
	alSourcef(alSource, AL_PITCH, pitch);
	alSourcef(alSource, AL_GAIN, gain);
	alSource3f(alSource, AL_POSITION, position.x, position.y, position.z);
	alSource3f(alSource, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
	alSourcei(alSource, AL_LOOPING, isLooping);
	return !CheckOpenALErrors();
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

	// Convert our format from SDL_WavSpec to ALenum wav format
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

	// Bind the buffer
	alGenBuffers((ALuint)1, &alBuffer);
	alBufferData(alBuffer, wavFormat, wavData, wavLength, wavSpec.freq);
	SDL_FreeWAV(wavData);	// Clean up the SDL buffer now we are done
	return !CheckOpenALErrors();
}
