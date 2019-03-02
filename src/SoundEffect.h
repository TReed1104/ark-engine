#ifndef ARKENGINE_SOUNDEFFECT_H_
#define ARKENGINE_SOUNDEFFECT_H_

#include <string>

// OpenAL
#include <AL/al.h>
#include <AL/alc.h>

class Engine;

class SoundEffect {
public:
	static Engine* Engine_Pointer;


	SoundEffect(const std::string& soundName = "NOT SET", const std::string& filePath = "NOT SET");
	~SoundEffect();
	

private:
	std::string name;
	bool isLoaded = false;

	bool LoadBuffer(void);

};

#endif