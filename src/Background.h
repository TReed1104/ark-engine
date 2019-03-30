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

private:
	std::string name;
	bool isLoaded = false;

	Texture* texture = nullptr;
	Model* model = nullptr;

	bool Load(const std::string& texturePath);
	Model::Mesh GenerateMeshForTexture(const Texture& textureToUse);
};

#endif