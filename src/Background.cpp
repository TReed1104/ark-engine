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

		// Set the UVs for the background quad
		backgroundMesh.uvs.push_back(glm::vec2(0.0f, 0.0f));
		backgroundMesh.uvs.push_back(glm::vec2(1.0f, 0.0f));
		backgroundMesh.uvs.push_back(glm::vec2(1.0f, 1.0f));
		backgroundMesh.uvs.push_back(glm::vec2(0.0f, 1.0f));
		backgroundMesh.isSetupForTextures = true;

		// Set the colour data and surface normal for each vertexPosition (these are all default)
		const size_t numberOfVertices = backgroundMesh.vertexPositions.size();
		for (size_t i = 0; i < numberOfVertices; i++) {
			backgroundMesh.colourData.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
			backgroundMesh.surfaceNormals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
		}

		// Set the indices for the mesh (the order to draw between the vertex positions
		backgroundMesh.indices.push_back(0);
		backgroundMesh.indices.push_back(1);
		backgroundMesh.indices.push_back(2);
		backgroundMesh.indices.push_back(0);
		backgroundMesh.indices.push_back(2);
		backgroundMesh.indices.push_back(3);


		//OverideLoadState(true);

		return true;
	}
	else {
		// Texture failed to load, Background::Load() failed
		Engine_Pointer->engineDebugger.WriteLine("ERROR!!!! - Background " + name + " failed to load");
		return false;
	}
}
