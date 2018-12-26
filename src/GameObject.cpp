#include "GameObject.h"
#include "Engine.h"

Engine* GameObject::Engine_Pointer;

GameObject::GameObject(const std::string & scriptPath) {
	// Load the script if given
	if (scriptPath != "NO SCRIPT") {
		script = new LuaScript(scriptPath);
		if (script->isScriptLoaded) {
			LoadAnimations();
		}
	}
	else {
		script = nullptr;
	}

	indexOfCurrentShader = Engine_Pointer->GetIndexOfShader("default");

	// Animation Setup
	animationIndex = 0;

	// Texture Setup
	texture = nullptr;
	sourceFramePosition = glm::ivec2(0);

	// Position Setup
	position = glm::vec3(0.0f);
	gridPosition = Engine_Pointer->ConvertToGridPosition(glm::vec2(this->position.x, this->position.y));
	drawOffset = glm::ivec2(0);
	drawPosition = this->position + glm::vec3(this->drawOffset, 0);
	rotation = 0.0f;
	scale = glm::vec3(1.0f);

	// Physics Setup
	velocity = glm::vec2(0.0f);
	baseMovementSpeed = 0.0f;
	currentMovementSpeed = 0.0f;
	baseFallingSpeed = 0.0f;
	currentFallingSpeed = 0.0f;
	baseJumpingSpeed = 0.0f;
	currentJumpingSpeed = 0.0f;

	boundingBoxOffset = glm::vec2(0.0f);
	boundingBox = BoundingBox(glm::vec2(this->position.x, this->position.y) + boundingBoxOffset);

	model.Translate(drawPosition);
	model.Rotate(rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	model.Scale(scale);

	sourceFrameSize = Engine_Pointer->tileTextureFrameSize;
}
GameObject::~GameObject(void) {
	if (script != nullptr) {
		delete script;
	}
}

// Game Run Time.
void GameObject::Update(const float& deltaTime) {

	// Apply transformations
	UpdatePosition();
	UpdateRotation();
	UpdateScale();

	// Animate the object
	HandleAnimations(deltaTime);

	// Reset the velocities.
	velocity = glm::vec2(0, 0);
}
void GameObject::Draw(void) {
	glEnable(GL_BLEND);
	// Loop through each mesh of the model
	for (int i = 0; i < model.meshes.size(); i++) {
		Engine_Pointer->shaderRegister[indexOfCurrentShader]->Activate();
		Model::Mesh &currentMesh = model.meshes[i];	// Ref to the current mesh for easier access.

		// Bind the VAO to be used in this draw.
		glBindVertexArray(currentMesh.vertexArrayObject);

		// Passes the Matrices to the shader
		glUniformMatrix4fv(glGetUniformLocation(Engine_Pointer->shaderRegister[indexOfCurrentShader]->program, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(Engine_Pointer->mainCamera->viewMatrix));
		glUniformMatrix4fv(glGetUniformLocation(Engine_Pointer->shaderRegister[indexOfCurrentShader]->program, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(Engine_Pointer->mainCamera->projectionMatrix));
		glUniformMatrix4fv(glGetUniformLocation(Engine_Pointer->shaderRegister[indexOfCurrentShader]->program, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(currentMesh.GetModelMatrix()));

		bool useTextures = (texture->id != -1 && currentMesh.isSetupForTextures);
		glUniform1i(glGetUniformLocation(Engine_Pointer->shaderRegister[indexOfCurrentShader]->program, "hasTexture"), useTextures);
		if (useTextures) {
			// Textures are setup correctly, tell the shader to usse the texture and setup the source frame.
			int textureLayerIndex = (sourceFramePosition.x + (texture->dimensionsInFrames.x * sourceFramePosition.y));
			glUniform1i(glGetUniformLocation(Engine_Pointer->shaderRegister[indexOfCurrentShader]->program, "textureArrayLayer"), textureLayerIndex);

			// Activate the correct texture.
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D_ARRAY, texture->id);
			glUniform1i(glGetUniformLocation(Engine_Pointer->shaderRegister[indexOfCurrentShader]->program, "textureSampler"), 0);
		}

		// Tell the shader how to draw between each point.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, currentMesh.indicesBufferObject);

		// Executes the draw call. Draws triangles, drawing between the points in the order specified. Using the indicies buffer currently bound.
		glDrawElements(GL_TRIANGLES, (GLsizei)currentMesh.indices.size(), GL_UNSIGNED_INT, (void*)0);

		// If the mesh was setup for texturing, this unbinds the textures used, clearing up ready for next time.
		if (useTextures) {
			glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
		}

		// Unbinds the indices buffer.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// Unbind the VAO, ready for the next object.
		glBindVertexArray(0);
		glUseProgram(0);
	}
	glDisable(GL_BLEND);
}

// Position control functions
void GameObject::Reposition(const glm::vec2& newPosition) {
	position = glm::vec3(newPosition.x, newPosition.y, position.z);
	gridPosition = Engine_Pointer->ConvertToGridPosition(glm::vec2(position.x, position.y));
	drawPosition = (position + glm::vec3(drawOffset, 0.0f));
	boundingBox.UpdatePosition(glm::vec2(position.x, position.y) + boundingBoxOffset);
	model.Translate(drawPosition);
}
void GameObject::UpdatePosition() {
	position += glm::vec3(velocity, 0.0f);
	gridPosition = Engine_Pointer->ConvertToGridPosition(glm::vec2(position.x, position.y));
	drawPosition = (position + glm::vec3(drawOffset, 0.0f));
	boundingBox.UpdatePosition(glm::vec2(position.x, position.y) + boundingBoxOffset);
	model.Translate(drawPosition);
}
void GameObject::UpdateRotation() {
	model.Rotate(rotation, glm::vec3(0.0f, 0.0f, 1.0f));
}
void GameObject::UpdateScale() {
	model.Scale(scale);
}
void GameObject::LoadAnimations() {
	animations.clear();
	bool areAnimationsPresent = script->Get<bool>("entity.has_animations");
	if (areAnimationsPresent) {
		int numberOfAnimations = script->Get<int>("entity.animations.number_of_animations");
		for (int i = 0; i < numberOfAnimations; i++) {
			std::string animationName = script->Get<std::string>("entity.animations.animation_" + std::to_string(i) + ".name");
			int numberOfFrames = script->Get<int>("entity.animations.animation_" + std::to_string(i) + ".number_of_frames");
			Animation newAnimation = Animation(animationName);
			for (int j = 0; j < numberOfFrames; j++) {
				int frameX = script->Get<int>("entity.animations.animation_" + std::to_string(i) + ".frame_" + std::to_string(j) + ".x");
				int frameY = script->Get<int>("entity.animations.animation_" + std::to_string(i) + ".frame_" + std::to_string(j) + ".y");
				float frameLength = script->Get<float>("entity.animations.animation_" + std::to_string(i) + ".frame_" + std::to_string(j) + ".length");
				newAnimation.AddFrame(glm::ivec2(frameX, frameY), frameLength);
			}
			animations.push_back(newAnimation);
		}
	}
}
void GameObject::HandleAnimations(const float& deltaTime) {
	if (animations.size() > 0) {
		// Store the old animation index for checking if it changes, this is needed for resetting the old animation.
		int oldAnimationIndex = animationIndex;

		// Call the Derived handler function
		UpdateAnimationState();
		UpdateAnimationIndex();

		// If the animation has changed, reset the old animation for its next use.
		if (animationIndex != oldAnimationIndex) {
			animations[oldAnimationIndex].Reset();
		}

		// Run the animation
		sourceFramePosition = animations[animationIndex].Run(deltaTime);
	}

}
void GameObject::UpdateAnimationState(void) {
}
void GameObject::UpdateAnimationIndex(void) {
}
