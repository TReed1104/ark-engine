#include "UserInterface.h"
#include "Engine.h"

Engine* UserInterface::Engine_Pointer;

UserInterface::Button::Button(std::string name) {

}
UserInterface::Button::~Button() {
	if (texture != nullptr) {
		delete texture;
	}
	if (model != nullptr) {
		delete model;
	}
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
void UserInterface::Button::Draw(void) {
}

UserInterface::UserInterface(const std::string& filePath) {
	isLoaded = Load(filePath);
}
UserInterface::~UserInterface() {
	// Clean up the created buttons
	const size_t numberOfButtons = buttonRegister.size();
	for (size_t i = 0; i < numberOfButtons; i++) {
		delete buttonRegister[i];
	}

	// Clean up the text objects
	const size_t numberOfTextObjects = textRegister.size();
	for (size_t i = 0; i < numberOfTextObjects; i++) {
		delete textRegister[i];
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
const glm::vec2 UserInterface::GetPosition(void) {
	return glm::vec2(position.x, position.y);
}
void UserInterface::SetPosition(const glm::vec2& newPosition) {
	// Leave the Z component the same, as it's our render order
	this->position = glm::vec3(newPosition.x, newPosition.y, position.z);
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
bool UserInterface::Load(const std::string& configFilePath) {
	if (configFilePath != "NOT LOADED") {
		// Reloading check
		if (configFile != nullptr) {
			delete configFile;
			configFile = nullptr;
		}

		configFile = new JsonFile(configFilePath);
		if (configFile->IsLoaded()) {
			name = configFile->Get<std::string>("interface.id");
			indexOfShader = Engine_Pointer->GetIndexOfShader(configFile->Get<std::string>("interface.shader"));
			dimensions = glm::ivec2(configFile->Get<int>("interface.dimensions.width"), configFile->Get<int>("interface.dimensions.height"));
			position = glm::vec3(configFile->Get<int>("interface.position.x"), configFile->Get<int>("interface.position.y"), 0.01f);
			if (indexOfShader == -1) {
				return false;
			}

			// Reloading check
			if (texture != nullptr) {
				delete texture;
				texture = nullptr;
			}

			texture = new Texture(name, "content/textures/" + configFile->Get<std::string>("interface.texture"), true, false);
			// Check we actually loaded the texture correctly
			if (texture->IsLoaded()) {

				// Reloading check
				if (model != nullptr) {
					delete model;
					model = nullptr;
				}

				// Generate the UIs model
				model = new Model(name, false);
				model->meshes.push_back(Model::GenerateMesh(dimensions));
				model->SetMeshParents();
				model->Translate(position);
				model->SetMeshTranslation(0, position);
				model->OverrideLoadState(true);

				// Load the interface elements from the config
				const size_t numberOfInterfaceElements = configFile->SizeOfObjectArray("interface.inteface elements");
				for (size_t i = 0; i < numberOfInterfaceElements; i++) {
					// Create the element
				}

				// Load the Text objects
				const size_t numberOfTextObjects = configFile->SizeOfObjectArray("interface.text objects");
				for (size_t i = 0; i < numberOfTextObjects; i++) {
					// Create the Text object
					std::string textName = configFile->Get<std::string>("interface.text objects." + std::to_string(i) + ".text.id");
					const int indexOfFont = Engine_Pointer->GetIndexOfFont(configFile->Get<std::string>("interface.text objects." + std::to_string(i) + ".text.font"));
					if (indexOfFont == -1) {
						return false;
					}
					std::string textString = configFile->Get<std::string>("interface.text objects." + std::to_string(i) + ".text.text");

					//TODO: Data binding

					glm::vec3 textPosition = glm::vec3(configFile->Get<int>("interface.text objects." + std::to_string(i) + ".text.position.x"), configFile->Get<int>("interface.text objects." + std::to_string(i) + ".text.position.y"), 0.015f);
					glm::vec3 relativePosition = position + textPosition;	// Move the text object relative to the UI background
					glm::vec3 textColour = glm::vec3(configFile->Get<float>("interface.text objects." + std::to_string(i) + ".text.colour.red") / 255.0f, configFile->Get<float>("interface.text objects." + std::to_string(i) + ".text.colour.green") / 255.0f, configFile->Get<float>("interface.text objects." + std::to_string(i) + ".text.colour.blue") / 255.0f);

					textRegister.push_back(new TextObject(textName, textString, Engine_Pointer->fontRegister[indexOfFont], relativePosition, textColour, true, false));
				}


				return true;
			}
			else {
				return false;
			}
		}
		else {
			Engine_Pointer->engineDebugger.WriteLine(">>>> UserInteface failed to load Config File: " + configFilePath);
			return false;
		}
	}
	else {
		configFile = nullptr;
		return false;
	}
}

void UserInterface::Update(const float& deltaTime) {
	// Update the Buttons
	const size_t numberOfButtons = buttonRegister.size();
	for (size_t i = 0; i < numberOfButtons; i++) {
		buttonRegister[i]->Update(deltaTime);
	}

	// Update the Text objects
	const size_t numberOfTextObjects = textRegister.size();
	for (size_t i = 0; i < numberOfTextObjects; i++) {
		textRegister[i]->Update(deltaTime);
	}

	// Update the position of the model before rendering
	model->Translate(position);
}
void UserInterface::Draw(void) {
	// UIs are not Camera dependant, so we'll need to generate our own View & Projection (just like the text objects)
	glm::vec2 viewPort = (Engine_Pointer->windowGridSize * Engine_Pointer->tileSize);
	glm::mat4* projectionMatrix = &(glm::ortho(0.0f, viewPort.x, viewPort.y, 0.0f, 0.0f, 2.0f));
	glm::mat4* viewMatrix = &(glm::lookAt(glm::vec3(position.x, position.y, 1.0f), glm::vec3(position.x, position.y, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

	glEnable(GL_BLEND);
	const size_t numberOfMeshes = model->meshes.size();
	for (size_t i = 0; i < numberOfMeshes; i++) {
		// Shader activation
		Engine_Pointer->shaderRegister[indexOfShader]->Activate();
		const GLuint* shaderProgramID = Engine_Pointer->shaderRegister[indexOfShader]->GetShaderID();
		// Get our model for the render
		Model::Mesh* currentMesh = &model->meshes[i];
		glBindVertexArray(currentMesh->vertexArrayObject);
		// Pass the Model, View and projection to the shader
		glUniformMatrix4fv(glGetUniformLocation(*shaderProgramID, "u_viewMatrix"), 1, GL_FALSE, glm::value_ptr(*viewMatrix));
		glUniformMatrix4fv(glGetUniformLocation(*shaderProgramID, "u_projectionMatrix"), 1, GL_FALSE, glm::value_ptr(*projectionMatrix));
		glUniformMatrix4fv(glGetUniformLocation(*shaderProgramID, "u_modelMatrix"), 1, GL_FALSE, glm::value_ptr(currentMesh->GetModelMatrix()));
		// Pass the Universal uniforms
		glUniform2fv(glGetUniformLocation(*shaderProgramID, "iResolution"), 1, glm::value_ptr(Engine_Pointer->windowDimensions));
		glUniform1f(glGetUniformLocation(*shaderProgramID, "iTime"), (float)SDL_GetTicks());
		glUniform3fv(glGetUniformLocation(*shaderProgramID, "iCameraPosition"), 1, glm::value_ptr(Engine_Pointer->mainCamera->position));
		// Texturing
		bool useTextures = (texture->textureID != -1 && currentMesh->isSetupForTextures);
		glUniform1i(glGetUniformLocation(*shaderProgramID, "u_hasTexture"), useTextures);
		if (useTextures) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture->textureID);
			glUniform1i(glGetUniformLocation(*shaderProgramID, "u_textureSampler"), 0);
		}
		// Draw the mesh
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, currentMesh->indicesBufferObject);
		glDrawElements(GL_TRIANGLES, (GLsizei)currentMesh->indices.size(), GL_UNSIGNED_INT, (void*)0);
		if (useTextures) {
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		// Clean up
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glUseProgram(0);
	}
	glDisable(GL_BLEND);

	// Render Buttons

	// Render Text objects
	const size_t numberOfTextObjects = textRegister.size();
	for (size_t i = 0; i < numberOfTextObjects; i++) {
		textRegister[i]->Draw();
	}
}
