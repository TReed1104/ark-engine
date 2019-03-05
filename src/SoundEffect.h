#ifndef ARKENGINE_SOUNDEFFECT_H_
#define ARKENGINE_SOUNDEFFECT_H_

#include <string>

// OpenAL
#include <AL/al.h>
#include <AL/alc.h>
#include <SDL.h>

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

	bool LoadBuffer(const std::string& filePath);

};

#endif