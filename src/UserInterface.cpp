#include "UserInterface.h"
#include "Engine.h"

Engine* UserInterface::Engine_Pointer;

UserInterface::Button::Button(std::string name) {

}
UserInterface::Button::~Button() {

}

const std::string UserInterface::Button::GetName(void) {
	return name;
}
void UserInterface::Button::Update(const float & deltaTime) {
	// Check the OnClick is set
	if (OnClick != nullptr) {
		OnClick();
	}
	// Check the MouseOver is set
	if (MouseOver != nullptr) {
		MouseOver();
	}
}

UserInterface::UserInterface(const std::string& filePath) {
	isLoaded = Load(filePath);
}
UserInterface::~UserInterface() {
	// Clean up the created buttons
	size_t numberOfButtons = buttonRegister.size();
	for (size_t i = 0; i < numberOfButtons; i++) {
		delete buttonRegister[i];
	}

	// If the texture has been loaded, delete it
	if (texture != nullptr) {
		delete texture;
	}

	// If the model has been generated, delete it
	if (model != nullptr) {
		delete model;
	}

	// Delete the config file
	if (configFile != nullptr) {
		delete configFile;
	}
}

const std::string UserInterface::GetName(void) {
	return this->name;
}
const bool UserInterface::IsLoaded(void) {
	return isLoaded;
}
const bool UserInterface::IsActive(void) {
	return isActive;
}
void UserInterface::SetActiveState(const bool& newState) {
	this->isActive = newState;
}
void UserInterface::Update(const float& deltaTime) {
	size_t numberOfButtons = buttonRegister.size();
	for (size_t i = 0; i < numberOfButtons; i++) {
		buttonRegister[i]->Update(deltaTime);
	}
}
void UserInterface::Draw(void) {

}

Model::Mesh UserInterface::GenerateMeshForTexture(const Texture & texture) {
	// Create the mesh we are going to return to the function call
	Model::Mesh backgroundMesh = Model::Mesh();

	// Specify four corner positions of the backgrounds quad 
	backgroundMesh.vertexPositions.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	backgroundMesh.vertexPositions.push_back(glm::vec3(0.0f + texture.dimensionsInPixels.x, 0.0f, 0.0f));
	backgroundMesh.vertexPositions.push_back(glm::vec3(0.0f + texture.dimensionsInPixels.x, 0.0f + texture.dimensionsInPixels.y, 0.0f));
	backgroundMesh.vertexPositions.push_back(glm::vec3(0.0f, 0.0f + texture.dimensionsInPixels.y, 0.0f));

	// Set the UVs for the background quad
	backgroundMesh.uvs.push_back(glm::vec2(0.0f, 0.0f));
	backgroundMesh.uvs.push_back(glm::vec2(1.0f, 0.0f));
	backgroundMesh.uvs.push_back(glm::vec2(1.0f, 1.0f));
	backgroundMesh.uvs.push_back(glm::vec2(0.0f, 1.0f));
	backgroundMesh.isSetupForTextures = true;	// Confirm the mesh can be textured

	// Set the colour data and surface normal for each vertexPosition (these are all default)
	const size_t numberOfVertices = backgroundMesh.vertexPositions.size();
	for (size_t i = 0; i < numberOfVertices; i++) {
		backgroundMesh.colourData.push_back(glm::vec3(1.0f, 1.0f, 1.0f));		// All white
		backgroundMesh.surfaceNormals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));	// All orientated towards our 2D cameras
	}

	// Set the indices for the mesh (the order to draw between the vertex positions
	backgroundMesh.indices.push_back(0);
	backgroundMesh.indices.push_back(1);
	backgroundMesh.indices.push_back(2);
	backgroundMesh.indices.push_back(0);
	backgroundMesh.indices.push_back(2);
	backgroundMesh.indices.push_back(3);

	// Bind the data to buffers ready to pass to the shaders
	backgroundMesh.BindBuffers();

	// Return our generated mesh
	return backgroundMesh;
}
