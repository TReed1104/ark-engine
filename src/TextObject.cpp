#include "TextObject.h"
#include "Engine.h"

Engine* TextObject::Engine_Pointer;

TextObject::TextObject(const std::string& name, const std::string& text, Font* font, const glm::vec3& position, const glm::vec3& colour, const bool& isEnabled, const bool& useCamera) {
	// Informational
	this->name = name;
	this->baseText = text;
	this->hasDataBindings = false;
	this->textToRender = text;
	this->font = font;

	// Transforms
	this->position = position;
	this->rotation = 0.0f;
	this->scale = glm::vec3(1.0f);

	// Rendering
	this->colour = colour;
	this->indexOfShader = Engine_Pointer->GetIndexOfShader("text object - default");
	this->model = new Model(text, false);
	this->model->OverrideLoadState(true);
	this->useCamera = useCamera;
	this->isEnabled = isEnabled;

	LoadText();
}
TextObject::~TextObject() {
	// Delete all the bindings
	for (DataBinding* binding : dataBindingRegister) {
		delete binding;
		binding = nullptr;
	}

	// Delete the model
	if (model != nullptr) {
		delete model;
		model = nullptr;
	}
}

void TextObject::Update(const float& deltaTime) {
	if (!isEnabled) {
		return;
	}

	// Execute our data bindings
	ExecuteDataBindings();

	// Apply transformations
	UpdatePosition();
	UpdateRotation();
	UpdateScale();
}
void TextObject::Draw(void) {
	if (!isEnabled) {
		return;
	}

	// Setup the MVP matrix for the Text.
	glm::vec2 viewPort = (Engine_Pointer->windowGridSize * Engine_Pointer->tileSize);
	glm::mat4* projectionMatrix = &(glm::ortho(0.0f, viewPort.x, viewPort.y, 0.0f, 0.0f, 2.0f));
	glm::mat4* viewMatrix = &(glm::lookAt(glm::vec3(position.x, position.y, 1.0f), glm::vec3(position.x, position.y, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	if (useCamera) {
		viewMatrix = &Engine_Pointer->mainCamera->viewMatrix;
		projectionMatrix = &Engine_Pointer->mainCamera->projectionMatrix;
	}

	glEnable(GL_BLEND);
	const size_t numberOfMeshes = model->meshes.size();
	for (size_t i = 0; i < numberOfMeshes; i++) {
		Engine_Pointer->shaderRegister[indexOfShader]->Activate();
		Model::Mesh* currentMesh = &model->meshes[i];
		glBindVertexArray(currentMesh->vertexArrayObject);
		const GLuint* shaderProgramID = Engine_Pointer->shaderRegister[indexOfShader]->GetShaderID();

		// Transformations
		glUniformMatrix4fv(glGetUniformLocation(*shaderProgramID, "u_viewMatrix"), 1, GL_FALSE, glm::value_ptr(*viewMatrix));
		glUniformMatrix4fv(glGetUniformLocation(*shaderProgramID, "u_projectionMatrix"), 1, GL_FALSE, glm::value_ptr(*projectionMatrix));
		glUniformMatrix4fv(glGetUniformLocation(*shaderProgramID, "u_modelMatrix"), 1, GL_FALSE, glm::value_ptr(currentMesh->GetModelMatrix()));

		// Universal uniforms all shaders for this engine should support
		glUniform2fv(glGetUniformLocation(*shaderProgramID, "iResolution"), 1, glm::value_ptr(Engine_Pointer->windowDimensions));
		glUniform1f(glGetUniformLocation(*shaderProgramID, "iTime"), (float)SDL_GetTicks());	// TODO: Change to not use SDL_Ticks, due to SDL_Ticks being consistent in its values
		glUniform3fv(glGetUniformLocation(*shaderProgramID, "iCameraPosition"), 1, glm::value_ptr(Engine_Pointer->mainCamera->position));

		// Texturing
		bool useTextures = (glyphs[i].texture.textureID != -1 && currentMesh->isSetupForTextures);
		glUniform1i(glGetUniformLocation(*shaderProgramID, "u_hasTexture"), useTextures);
		if (useTextures) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, glyphs[i].texture.textureID);
			glUniform1i(glGetUniformLocation(*shaderProgramID, "u_textureSampler"), 0);
			glUniform3fv(glGetUniformLocation(*shaderProgramID, "u_textColour"), 1, glm::value_ptr(colour));
		}

		// Draw calls
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, currentMesh->indicesBufferObject);
		glDrawElements(GL_TRIANGLES, (GLsizei)currentMesh->indices.size(), GL_UNSIGNED_INT, (void*)0);
		if (useTextures) {
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glUseProgram(0);
	}
	glDisable(GL_BLEND);
}
void TextObject::UpdateText(const std::string& newText) {
	this->baseText = newText;
	this->textToRender = newText;
	this->LoadText();
}
void TextObject::UpdateFont(Font* font) {
	this->font = font;
	LoadText();
}
const glm::vec3 TextObject::GetPosition(void) {
	return this->position;
}
void TextObject::Reposition(const glm::vec3& newPosition) {
	position = newPosition;
	model->Translate(position);

	// TODO: Model/Mesh classes need to be rewritten to make it so full reloading isn't required
	LoadText();
}
const std::string TextObject::GetName(void) {
	return this->name;
}
const bool TextObject::IsEnabled(void) {
	return this->isEnabled;
}
void TextObject::SetEnabledState(const bool& enableState) {
	this->isEnabled = enableState;
}
void TextObject::LoadText() {
	glyphs.clear();
	model->meshes.clear();
	const size_t lengthOfText = textToRender.size();
	for (size_t i = 0; i < lengthOfText; i++) {
		Glyph& currentGlyph = font->GetGlyph(textToRender[i]);
		glyphs.push_back(currentGlyph);
		model->meshes.push_back(currentGlyph.mesh);
	}

	// Setup the Model for handling its new Meshes.
	model->SetMeshParents();
	model->Translate(position);
	model->Rotate(rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	model->Scale(scale);

	// Configure the positioning of each glyph in a piece of text
	glm::vec3 cursorPosition = position;	// The cursor is the horizontal position along a line of text where a character is to be rendered
	for (size_t i = 0; i < lengthOfText; i++) {
		// Create a reference directly to the glyph and model mesh to configure
		Glyph& currentGlyph = glyphs[i];
		Model::Mesh& currentMesh = model->meshes[i];

		/* Create the draw position for the glyph, this uses the cursor position and the glyph metrics for its bearings,
		the bearing is the values deciding how the glyph itself is positioned in comparison to the line the text is rendered along.
		E.g. how the letter 'g' hangs half below the line of text, or how an apostrophe is at the top of the line. */
		glm::vec3 drawPosition = cursorPosition;
		drawPosition.x = cursorPosition.x + currentGlyph.horizontalBearing.x;	// shift right
		drawPosition.y = cursorPosition.y - currentGlyph.horizontalBearing.y;	// shift down, oddly this has to be inverted

		//TODO: Implement the ability to the text to go vertically, this uses currentGlyph.verticalBearing.

		// Move the mesh to the position the cursor dictates
		currentMesh.SetTranslation(false, drawPosition);

		// Move the cursor to the correct position ready for the next character
		cursorPosition.x += currentGlyph.advance.x;
	}
}
void TextObject::UpdatePosition() {
	model->Translate(position);
}
void TextObject::UpdateRotation() {
	model->Rotate(rotation, glm::vec3(0.0f, 0.0f, 1.0f));
}
void TextObject::UpdateScale() {
	model->Scale(scale);
}

// Data binding
const bool TextObject::HasDataBindings(void) {
	return this->hasDataBindings;
}
void TextObject::RegisterDataBinding(DataBinding* newDataBinding) {
	// Register the data binding with the object
	dataBindingRegister.push_back(newDataBinding);

	// Mark the object as having data bindings
	if (hasDataBindings == false) {
		hasDataBindings = true;
	}
}
void TextObject::RemoveDataBinding(const std::string& bindingID) {
	// Find the binding
	int indexOfBinding = -1;

	const size_t sizeOfBindingRegister = dataBindingRegister.size();
	for (size_t i = 0; i < sizeOfBindingRegister; i++) {
		if (dataBindingRegister[i]->GetName() == bindingID) {
			indexOfBinding = (int)i;
		}
	}

	// Binding wasn't found, exit the function
	if (indexOfBinding == -1) {
		return;
	}

	// Remove the binding
	delete dataBindingRegister[indexOfBinding];
	dataBindingRegister.erase(dataBindingRegister.begin() + indexOfBinding);

	// If the binding register is now empty, set the hasBindings flag to false
	const size_t newBindingRegisterSize = dataBindingRegister.size();
	if (newBindingRegisterSize == 0) {
		hasDataBindings = false;
	}
}
void TextObject::ExecuteDataBindings(void) {
	// Check we have any data bindings
	if (!hasDataBindings) {
		return;
	}

	// Store our last text
	std::string previousText = textToRender;

	// We split the base text by each space character, this gives us the 'words' of the text
	std::vector<std::string> splitTextString = StringUtilities::Split(baseText, ' ');

	// Go through each binding in the register
	for (DataBinding* bindToExecute : dataBindingRegister) {

		// Our exported data on the object to bind
		std::map<std::string, std::string> objectsExportedData;

		// We've found our binding, lets find the object we wanna use
		switch (bindToExecute->GetTargetType()) {
			case DataBinding::ENGINE:
			{
				objectsExportedData = Engine_Pointer->ExportDataForBinding();
				break;
			}
			case DataBinding::CAMERA:
			{
				Engine_Pointer->engineDebugger.WriteLine(">> NOT IMPLEMENTED -> DataBinding object type - Camera");
				break;
			}
			case DataBinding::LEVEL:
			{
				// Get the level the binding focuses on
				Level* level = LevelManager::GetInstance()->GetLevel(bindToExecute->GetTargetName());

				// Check the level was found
				if (level == nullptr) {
					break;
				}

				// Check the token is a valid binding
				objectsExportedData = level->ExportDataForBinding();
				break;
			}
			case DataBinding::TILE:
			{
				Engine_Pointer->engineDebugger.WriteLine(">> NOT IMPLEMENTED -> DataBinding object type - Tile");
				break;
			}
			case DataBinding::ITEM:
			{
				Engine_Pointer->engineDebugger.WriteLine(">> NOT IMPLEMENTED -> DataBinding object type - Item");
				break;
			}
			case DataBinding::ENTITY:
			{
				Entity* entity = nullptr;
				if (bindToExecute->GetTargetName() == "player") {
					entity = Engine_Pointer->player;

					// Check we found the player
					if (entity == nullptr) {
						break;
					}

					objectsExportedData = entity->ExportDataForBinding();
				}
				else {
					int indexOfEntity = Engine_Pointer->GetIndexOfEntity(bindToExecute->GetTargetName());

					// Make sure we found an entity
					if (indexOfEntity != -1) {
						break;
					}
					entity = Engine_Pointer->entityRegister[indexOfEntity];
					objectsExportedData = entity->ExportDataForBinding();
				}
				break;
			}
			default:
				break;
		}


		// For each binding, go through each word of the base text looking for it
		textToRender = "";
		for (std::string& potentialBinding : splitTextString) {
			// Check if the current word starts with a %
			if (potentialBinding[0] != '%') {
				// No symbol found, this word isn't a data binding

				// check if its the last word in the string
				if (splitTextString.back() == potentialBinding) {
					// Last word doesn't need a space
					textToRender += potentialBinding;
				}
				else {
					textToRender += potentialBinding + " ";
				}

				// continue to the next potentially binding
				continue;
			}

			// Check if the potential binding is the current binding we are executing
			if (potentialBinding != bindToExecute->GetBindingToken()) {
				continue;
			}

			// Make sure the binding is actually a valid option in the objects exported variables
			if (objectsExportedData.find(bindToExecute->GetBindingToken()) == objectsExportedData.end()) {
				Engine_Pointer->engineDebugger.WriteLine(">> Invalid Data Binding");
				break;
			}

			// Bind the data
			potentialBinding = objectsExportedData[bindToExecute->GetVariableName()];

			// check if its the last word in the string
			if (splitTextString.back() == potentialBinding) {
				// Last word doesn't need a space
				textToRender += potentialBinding;
			}
			else {
				textToRender += potentialBinding + " ";
			}
		}
	}

	// If the text has changed, update the models
	if (textToRender != previousText) {
		LoadText();	// Reload the text glyphs using the new text
	}
}
