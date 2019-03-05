#ifndef ARKENGINE_SOUNDEFFECT_H_
#define ARKENGINE_SOUNDEFFECT_H_

#include <string>
#include <SDL.h>
#include <glm/glm.hpp>
#include <AL/al.h>
#include <AL/alc.h>

class Engine;

class SoundEffect {
public:
	static Engine* Engine_Pointer;

	SoundEffect(const std::string& soundName = "NOT SET", const std::string& filePath = "NOT SET");
	~SoundEffect();
	
	const std::string GetName(void);
	const bool IsLoaded(void);

private:
	std::string name;
	bool isLoaded = false;
	ALuint alBuffer;
	ALuint alSource;

	bool CheckOpenALErrors(const ALenum& errorCode);
	bool LoadSource(void);
	bool LoadSource(const float& pitch, const float& gain, const glm::ivec3& position, const glm::ivec3& velocity, const bool& isLooping);
	bool LoadBuffer(const std::string& filePath);

};

#endif