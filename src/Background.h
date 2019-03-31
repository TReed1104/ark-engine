#ifndef ARKENGINE_BACKGROUND_H_
#define ARKENGINE_BACKGROUND_H_

#include <string>
#include "Texture.h"
#include "Model.h"

class Engine;

class Background {
public:
	static Engine* Engine_Pointer;

	Background(const std::string& name, const std::string& texturePath);
	~Background();

	void Update(const float& deltaTime);
	void Draw(void);
	const std::string GetName(void);
	const bool IsLoaded(void);
	const glm::vec3 GetPosition(void);
	const void SetPosition(const glm::vec3& newPosition);

private:
	std::string name;
	bool isLoaded = false;

	Texture* texture = nullptr;
	Model* model = nullptr;
	glm::vec3 position;

	bool Load(const std::string& texturePath);
	Model::Mesh GenerateMeshForTexture(const Texture& textureToUse);
};

#endif