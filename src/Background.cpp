#include "Background.h"
#include "Engine.h"

Engine* Background::Engine_Pointer;

Background::Background(const std::string& name, const std::string& texturePath) {
	this->name = name;
	isLoaded = Load(texturePath);
}
Background::~Background() {
	// If the texture has been created on the heap, delete it
	if (texture != nullptr) {
		delete texture;
	}
	if (model != nullptr) {
		delete model;
	}
}

void Background::Update(const float& deltaTime) {

}
void Background::Draw(void) {

}
const std::string Background::GetName() {
	return name;
}
const bool Background::IsLoaded(void) {
	return isLoaded;
}
bool Background::Load(const std::string& texturePath) {
	texture = new Texture(name, texturePath, true, false);
	
	// Texture was successfully loaded
	if (texture->IsLoaded()) {
		
		// Create the mesh using the dimensions of the texture
		model = new Model(name, false);
		Model::Mesh backgroundMesh = Model::Mesh();

		// Specify four corner positions of the backgrounds quad 
		backgroundMesh.vertexPositions.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
		backgroundMesh.vertexPositions.push_back(glm::vec3(0.0f + texture->dimensionsInPixels.x, 0.0f, 0.0f));
		backgroundMesh.vertexPositions.push_back(glm::vec3(0.0f + texture->dimensionsInPixels.x, 0.0f + texture->dimensionsInPixels.y, 0.0f));
		backgroundMesh.vertexPositions.push_back(glm::vec3(0.0f, 0.0f + texture->dimensionsInPixels.y, 0.0f));



		//OverideLoadState(true);

		return true;
	}
	else {
		// Texture failed to load, Background::Load() failed
		Engine_Pointer->engineDebugger.WriteLine("ERROR!!!! - Background " + name + " failed to load");
		return false;
	}
}
