#include "GameObject.h"
#include "Engine.h"

Engine* GameObject::Engine_Pointer;

GameObject::GameObject(const std::string & scriptPath) {
	indexOfCurrentShader = Engine_Pointer->indexOfDefaultShader;
	// Load the script if given
	if (scriptPath != "NO SCRIPT") {
		this->script = new LuaScript(scriptPath);
		if (script->isScriptLoaded) {
			LoadAnimations();
		}
	}
	else {
		this->script = nullptr;
	}

	// Animation Setup
	animationState = AnimationState::IdleDown;
	animationIndex = 0;

	// Texture Setup
	this->texture = nullptr;
	this->sourceFramePosition = glm::ivec2(0.0f);

	// Position Setup
	this->position = glm::vec3(0.0f);
	this->gridPosition = Engine_Pointer->ConvertToGridPosition(glm::vec2(this->position.x, this->position.y));
	this->drawOffset = glm::ivec2(0.0f);
	this->drawPosition = this->position + glm::vec3(this->drawOffset, 0);
	this->rotation = 0.0f;
	this->scale = glm::vec3(1.0f);

	// Physics Setup
	this->velocity = glm::vec2(0.0f);
	this->velocitySnap = glm::vec2(0.0f);
	this->movementSpeed = 0.0f;
	this->boundingBoxOffset = glm::vec2(0.0f);
	this->boundingBox = BoundingBox(glm::vec2(this->position.x, this->position.y) + boundingBoxOffset);

	model.Translate(drawPosition);
	model.Rotate(rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	model.Scale(scale);

	doRotation = false;
	doScalar = false;

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
	UpdatePosition((velocity != glm::vec2(0.0f, 0.0f) || velocitySnap != glm::vec2(0.0f, 0.0f)));
	UpdateRotation();
	UpdateScale();

	// Animate the object
	UpdateAnimations(deltaTime);

	// Reset the velocities.
	velocity = glm::vec2(0, 0);
	velocitySnap = glm::vec2(0.0f, 0.0f);
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
		glUniformMatrix4fv(glGetUniformLocation(Engine_Pointer->shaderRegister[indexOfCurrentShader]->program, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(Engine_Pointer->camera->viewMatrix));
		glUniformMatrix4fv(glGetUniformLocation(Engine_Pointer->shaderRegister[indexOfCurrentShader]->program, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(Engine_Pointer->camera->projectionMatrix));
		glUniformMatrix4fv(glGetUniformLocation(Engine_Pointer->shaderRegister[indexOfCurrentShader]->program, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(currentMesh.GetModelMatrix()));

		bool useTextures = (texture->id != -1 && currentMesh.isSetupForTextures);
		if (useTextures) {
			// Textures are setup correctly, tell the shader to usse the texture and setup the source frame.
			glUniform1i(glGetUniformLocation(Engine_Pointer->shaderRegister[indexOfCurrentShader]->program, "hasTexture"), useTextures);
			glUniform1i(glGetUniformLocation(Engine_Pointer->shaderRegister[indexOfCurrentShader]->program, "textureArrayLayer"), sourceFramePosition.x + (texture->dimensionsInFrames.x * sourceFramePosition.y));

			// Activate the correct texture.
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D_ARRAY, texture->id);
			glUniform1i(glGetUniformLocation(Engine_Pointer->shaderRegister[0]->program, "textureSampler"), 0);
		}
		else {
			// Textures are not setup, use the colour buffer.
			glUniform1i(glGetUniformLocation(Engine_Pointer->shaderRegister[0]->program, "hasTexture"), false);
		}

		// Tell the shader how to draw between each point.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, currentMesh.indicesBufferObject);

		// Executes the draw call. Draws triangles, drawing between the points in the order specified. Using the indicies buffer currently bound.
		glDrawElements(GL_TRIANGLES, currentMesh.indices.size(), GL_UNSIGNED_INT, (void*)0);

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
void GameObject::UpdatePosition(bool doTransform) {
	if (doTransform) {
		position += glm::vec3(velocity, 0.0f);
		position += glm::vec3(velocitySnap, 0.0f);
		gridPosition = Engine_Pointer->ConvertToGridPosition(glm::vec2(position.x, position.y));
		drawPosition = (position + glm::vec3(drawOffset, 0.0f));
		boundingBox.UpdatePosition(glm::vec2(position.x, position.y) + boundingBoxOffset);
		model.Translate(drawPosition);
	}
}
void GameObject::UpdateRotation() {
	if (doRotation) {
		model.Rotate(rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	}
}
void GameObject::UpdateScale() {
	if (doScalar) {
		model.Scale(scale);
	}
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
void GameObject::UpdateAnimations(const float& deltaTime) {
	if (animations.size() > 0) {
		// Store the old animation index for checking if it changes, this is needed for resetting the old animation.
		int oldAnimationIndex = animationIndex;

		// Call the Derived handler function
		AnimationHandler();

		// Using the current animaiton state, work out which animation in the list to use.
		switch (animationState) {
		case AnimationState::IdleDown:
			animationIndex = 0;
			break;
		case AnimationState::IdleUp:
			animationIndex = 1;
			break;
		case AnimationState::IdleLeft:
			animationIndex = 2;
			break;
		case AnimationState::IdleRight:
			animationIndex = 3;
			break;
		case AnimationState::MoveDown:
			animationIndex = 4;
			break;
		case AnimationState::MoveUp:
			animationIndex = 5;
			break;
		case AnimationState::MoveLeft:
			animationIndex = 6;
			break;
		case AnimationState::MoveRight:
			animationIndex = 7;
			break;
		case AnimationState::AttackDown:
			animationIndex = 8;
			break;
		case AnimationState::AttackUp:
			animationIndex = 9;
			break;
		case AnimationState::AttackLeft:
			animationIndex = 10;
			break;
		case AnimationState::AttackRight:
			animationIndex = 11;
			break;
		default:
			break;
		}

		// If the animation has changed, reset the old animation for its next use.
		if (animationIndex != oldAnimationIndex) {
			animations[oldAnimationIndex].Reset();
		}

		// Run the animation
		sourceFramePosition = animations[animationIndex].Run(deltaTime);
		//std::cout << "Source Frame Position - X:" << sourceFramePosition.x << ", Y: " << sourceFramePosition.y << std::endl;
	}

}
void GameObject::AnimationHandler(void) {
}
