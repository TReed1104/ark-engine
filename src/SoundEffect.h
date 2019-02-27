#ifndef ARKENGINE_SOUNDEFFECT_H_
#define ARKENGINE_SOUNDEFFECT_H_

// OpenAL
#include <AL/al.h>
#include <AL/alc.h>

class Engine;

class SoundEffect {
public:
	static Engine* Engine_Pointer;

	ALsizei size;
	ALsizei frequency;
	ALvoid* data;
	ALboolean loop;

	SoundEffect();
	~SoundEffect();

private:

};

#endif