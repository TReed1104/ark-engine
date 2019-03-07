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

	SoundEffect(const std::string& soundName = "NOT SET", const std::string& filePath = "NOT SET", const bool& isLooped = false, const glm::vec3& position = glm::vec3(0.0f), const glm::vec3& velocity = glm::vec3(0.0f), const float& pitch = 1.0f, const float& gain = 1.0f);
	~SoundEffect();
	
	// Gets
	const std::string GetName(void);
	const bool IsLoaded(void);
	const bool IsLooped(void);
	const float GetPitch(void);
	const float GetGain(void);
	const glm::vec3 GetPosition(void);
	const glm::vec3 GetVelocity(void);
	
	// Sets
	void SetPitch(const float& newPitch);

	// Playback functions
	void Play(void);
	void Pause(void);
	void Stop(void);

private:
	std::string name;
	bool isLoaded = false;
	bool isLooped = false;
	float pitch = 1.0f;
	float gain = 1.0f;
	glm::vec3 position = glm::vec3(0);
	glm::vec3 velocity = glm::vec3(0);

	ALuint alBuffer;
	ALuint alSource;

	bool CheckOpenALErrors(void);
	bool LoadSource(void);
	bool LoadBuffer(const std::string& filePath);

};

#endif