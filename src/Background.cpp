#include "Background.h"
#include "Engine.h"

Engine* Background::Engine_Pointer;

Background::Background(const std::string& name, const std::string& texturePath, const std::string& shaderName) {
	this->name = name;
	this->indexOfShader = Engine_Pointer->GetIndexOfShader(shaderName);
	isLoaded = Load(texturePath);
	position = glm::vec3(0.0f, 0.0f, -0.02f);	// Position the background behind the level's tiles, which are at -0.01f
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
	BehaviourHandler(deltaTime);	// Where the magic for the background effects happens
	model->Translate(position);
}
void Background::Draw(void) {
	glEnable(GL_BLEND);
	const size_t numberOfMeshes = model->meshes.size();
	for (size_t i = 0; i < numberOfMeshes; i++) {
		Engine_Pointer->shaderRegister[indexOfShader]->Activate();
		Model::Mesh &currentMesh = model->meshes[i];
		glBindVertexArray(currentMesh.vertexArrayObject);
		const GLuint* shaderProgramID = Engine_Pointer->shaderRegister[indexOfShader]->GetShaderID();
		glUniformMatrix4fv(glGetUniformLocation(*shaderProgramID, "u_viewMatrix"), 1, GL_FALSE, glm::value_ptr(Engine_Pointer->mainCamera->viewMatrix));
		glUniformMatrix4fv(glGetUniformLocation(*shaderProgramID, "u_projectionMatrix"), 1, GL_FALSE, glm::value_ptr(Engine_Pointer->mainCamera->projectionMatrix));
		glUniformMatrix4fv(glGetUniformLocation(*shaderProgramID, "u_modelMatrix"), 1, GL_FALSE, glm::value_ptr(currentMesh.GetModelMatrix()));

		// Universal uniforms all shaders for this engine should support
		glUniform2fv(glGetUniformLocation(*shaderProgramID, "iResolution"), 1, glm::value_ptr(Engine_Pointer->windowDimensions));
		glUniform1f(glGetUniformLocation(*shaderProgramID, "iTime"), (float)SDL_GetTicks());	// TODO: Change to not use SDL_Ticks, due to SDL_Ticks being consistent in its values
		glUniform3fv(glGetUniformLocation(*shaderProgramID, "iCameraPosition"), 1, glm::value_ptr(Engine_Pointer->mainCamera->position));

		bool useTextures = (texture->textureID != -1 && currentMesh.isSetupForTextures);
		glUniform1i(glGetUniformLocation(*shaderProgramID, "u_hasTexture"), useTextures);
		if (useTextures) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture->textureID);
			glUniform1i(glGetUniformLocation(*shaderProgramID, "u_textureSampler"), 0);
		}

		// Lighting
		LightSource* levelLight = LevelManager::GetInstance()->GetCurrentLevel()->lightSourcesRegister[0];
		glUniform3fv(glGetUniformLocation(*shaderProgramID, "light.position"), 1, glm::value_ptr(levelLight->GetPosition()));
		glUniform3fv(glGetUniformLocation(*shaderProgramID, "light.colour"), 1, glm::value_ptr(levelLight->GetColour()));
		glUniform1f(glGetUniformLocation(*shaderProgramID, "light.radius"), levelLight->GetRadius());

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, currentMesh.indicesBufferObject);
		glDrawElements(GL_TRIANGLES, (GLsizei)currentMesh.indices.size(), GL_UNSIGNED_INT, (void*)0);
		if (useTextures) {
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glUseProgram(0);
	}
	glDisable(GL_BLEND);
}
const std::string Background::GetName() {
	return name;
}
const bool Background::IsLoaded(void) {
	return isLoaded;
}
const glm::vec2 Background::GetPosition(void) {
	return glm::vec2(position);
}
const void Background::SetPosition(const glm::vec2& newPosition) {
	this->position = glm::vec3(newPosition.x, newPosition.y, position.z);
	model->Translate(position);
}
bool Background::Load(const std::string& texturePath) {
	texture = new Texture(name, texturePath, true, false);

	// Texture was successfully loaded
	if (texture->IsLoaded()) {

		// Create the mesh using the dimensions of the texture
		model = new Model(name, false);
		model->meshes.clear();	// Clear the mesh list just for safety
		model->meshes.push_back(GenerateMeshForTexture(*(this->texture)));
		model->SetMeshParents();
		model->Translate(position);
		model->OverrideLoadState(true);

		return true;
	}
	else {
		// Texture failed to load, Background::Load() failed
		Engine_Pointer->engineDebugger.WriteLine("ERROR!!!! - Background " + name + " failed to load");
		return false;
	}
}
Model::Mesh Background::GenerateMeshForTexture(const Texture& textureToUse) {
	// Create the mesh we are going to return to the function call
	Model::Mesh backgroundMesh = Model::Mesh();

	// Specify four corner positions of the backgrounds quad 
	backgroundMesh.vertexPositions.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	backgroundMesh.vertexPositions.push_back(glm::vec3(0.0f + textureToUse.dimensionsInPixels.x, 0.0f, 0.0f));
	backgroundMesh.vertexPositions.push_back(glm::vec3(0.0f + textureToUse.dimensionsInPixels.x, 0.0f + textureToUse.dimensionsInPixels.y, 0.0f));
	backgroundMesh.vertexPositions.push_back(glm::vec3(0.0f, 0.0f + textureToUse.dimensionsInPixels.y, 0.0f));

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
void Background::BehaviourHandler(const float& deltaTime) {
	// TODO: Implement background magic
}
