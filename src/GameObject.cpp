#include "GameObject.h"
#include "Engine.h"

Engine* GameObject::Engine_Pointer;

GameObject::GameObject(const std::string& filePath) {
	// Load the Config File
	if (filePath != "NOT LOADED") {
		configFile = new JsonFile(filePath);
		if (!configFile->IsLoaded()) {
			Engine_Pointer->engineDebugger.WriteLine(">>>> Object failed to load Config File: " + filePath);
			isLoaded = false;
		}
	}
	else {
		configFile = nullptr;
		isLoaded = false;
	}

	name = "Unnamed";
	indexOfShader = Engine_Pointer->GetIndexOfShader("game object - texture array");

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
	currentFallingSpeed = baseFallingSpeed;
	fallTimer = 0.0f;

	boundingBoxOffset = glm::vec2(0.0f);
	boundingBox = BoundingBox(glm::vec2(this->position.x, this->position.y) + boundingBoxOffset);

	model.Translate(drawPosition);
	model.Rotate(rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	model.Scale(scale);

	sourceFrameSize = Engine_Pointer->tileTextureFrameSize;

	isFalling = false;
	isAffectedByGravity = false;
}
GameObject::~GameObject(void) {
	// Delete the config file
	if (configFile != nullptr) {
		delete configFile;
	}
}

// Game Run Time.
void GameObject::Update(const float& deltaTime) {
	// Base update takes place after the derived class update has executed
	PhysicsController(deltaTime);
	AnimationController(deltaTime);
	TransformationController();
}
void GameObject::Draw(void) {
	glEnable(GL_BLEND);
	// Loop through each mesh of the model
	for (int i = 0; i < model.meshes.size(); i++) {

		Engine_Pointer->shaderRegister[indexOfShader]->Activate();
		Model::Mesh &currentMesh = model.meshes[i];	// Ref to the current mesh for easier access.

		// Bind the VAO to be used in this draw.
		glBindVertexArray(currentMesh.vertexArrayObject);

		// Passes the Matrices to the shader
		const GLuint* shader = Engine_Pointer->shaderRegister[indexOfShader]->GetShader();
		glUniformMatrix4fv(glGetUniformLocation(*shader, "u_viewMatrix"), 1, GL_FALSE, glm::value_ptr(Engine_Pointer->mainCamera->viewMatrix));
		glUniformMatrix4fv(glGetUniformLocation(*shader, "u_projectionMatrix"), 1, GL_FALSE, glm::value_ptr(Engine_Pointer->mainCamera->projectionMatrix));
		glUniformMatrix4fv(glGetUniformLocation(*shader, "u_modelMatrix"), 1, GL_FALSE, glm::value_ptr(currentMesh.GetModelMatrix()));
		
		// Universal uniforms all shaders for this engine should support
		glUniform2fv(glGetUniformLocation(*shader, "iResolution"), 1, glm::value_ptr(Engine_Pointer->windowDimensions));
		glUniform1f(glGetUniformLocation(*shader, "iTime"), (float)SDL_GetTicks());	// TODO: Change to not use SDL_Ticks, due to SDL_Ticks being consistent in its values
		glUniform3fv(glGetUniformLocation(*shader, "iCameraPosition"), 1, glm::value_ptr(Engine_Pointer->mainCamera->position));

		bool useTextures = (texture->textureID != -1 && currentMesh.isSetupForTextures);
		glUniform1i(glGetUniformLocation(*shader, "u_hasTexture"), useTextures);
		if (useTextures) {
			// Textures are setup correctly, tell the shader to usse the texture and setup the source frame.
			int textureLayerIndex = (sourceFramePosition.x + (texture->dimensionsInFrames.x * sourceFramePosition.y));
			glUniform1i(glGetUniformLocation(*shader, "u_textureArrayLayer"), textureLayerIndex);

			// Activate the correct texture.
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D_ARRAY, texture->textureID);
			glUniform1i(glGetUniformLocation(*shader, "u_textureSampler"), 0);
		}

		// Lighting
		glUniform3fv(glGetUniformLocation(*shader, "light.position"), 1, glm::value_ptr(glm::vec3(50.0f, 50.0f, 1.0f)));
		glUniform3fv(glGetUniformLocation(*shader, "light.colour"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
		glUniform1f(glGetUniformLocation(*shader, "light.ambientIntensity"), 0.3f);



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

const std::string GameObject::GetName(void) {
	return this->name;
}
const bool GameObject::IsLoaded(void) {
	return isLoaded;
}
void GameObject::Reposition(const glm::vec2& newPosition) {
	position = glm::vec3(newPosition.x, newPosition.y, position.z);
	velocity = glm::vec2(0, 0);																			// Reset Velocity, because the movement has taken place
	gridPosition = Engine_Pointer->ConvertToGridPosition(glm::vec2(position.x, position.y));
	drawPosition = (position + glm::vec3(drawOffset, 0.0f));
	drawPosition = glm::vec3(glm::floor(drawPosition.x), glm::floor(drawPosition.y), drawPosition.z);	// Floor the x and y values of the drawPosition to cap it to pixel grid
	model.Translate(drawPosition);
	boundingBox.UpdatePosition(glm::vec2(position.x, position.y) + boundingBoxOffset);					// Update the bounding box to match the new positions
}
void GameObject::Reposition(const glm::vec3& newPosition) {
	position = newPosition;
	velocity = glm::vec2(0, 0);																			// Reset Velocity, because the movement has taken place
	gridPosition = Engine_Pointer->ConvertToGridPosition(glm::vec2(position.x, position.y));
	drawPosition = (position + glm::vec3(drawOffset, 0.0f));
	drawPosition = glm::vec3(glm::floor(drawPosition.x), glm::floor(drawPosition.y), drawPosition.z);	// Floor the x and y values of the drawPosition to cap it to pixel grid
	model.Translate(drawPosition);
	boundingBox.UpdatePosition(glm::vec2(position.x, position.y) + boundingBoxOffset);					// Update the bounding box to match the new positions
}
const glm::ivec2 GameObject::GetGridPosition(void) {
	return gridPosition;
}
void GameObject::UpdatePosition() {
	position += glm::vec3(velocity, 0.0f);
	velocity = glm::vec2(0, 0);																			// Reset Velocity, because the movement has taken place
	gridPosition = Engine_Pointer->ConvertToGridPosition(glm::vec2(position.x, position.y));
	drawPosition = (position + glm::vec3(drawOffset, 0.0f));
	drawPosition = glm::vec3(glm::floor(drawPosition.x), glm::floor(drawPosition.y), drawPosition.z);	// Floor the x and y values of the drawPosition to cap it to pixel grid
	model.Translate(drawPosition);
	boundingBox.UpdatePosition(glm::vec2(position.x, position.y) + boundingBoxOffset);					// Update the bounding box to match the new positions
}
void GameObject::UpdateRotation() {
	model.Rotate(rotation, glm::vec3(0.0f, 0.0f, 1.0f));
}
void GameObject::UpdateScale() {
	model.Scale(scale);
}
void GameObject::TransformationController(void) {
	UpdatePosition();
	UpdateRotation();
	UpdateScale();
}
void GameObject::PhysicsHandlerFalling(const float& deltaTime) {
	// If the entity is not jumping
	if (isAffectedByGravity) {
		// Declare the variables used for the calculations
		glm::vec2 newVelocity = glm::vec2(0.0f, currentFallingSpeed * deltaTime);
		glm::vec2 newPosition = glm::vec2(position) + newVelocity;
		BoundingBox newBoundingBox = BoundingBox(newPosition + boundingBoxOffset, boundingBox.GetDimensions());
		bool isColliding = false;

		// Calculate the difference in grid cells between the top left of the AABB and the top right, giving us the AABB width in cells
		int deltaGridX = abs(newBoundingBox.BottomLeftGridPosition().x - newBoundingBox.BottomRightGridPosition().x);
		// For every delta, do another check until we run out of checks or find a collision
		for (size_t i = 0; i <= deltaGridX; i++) {
			glm::ivec2 gridPositionToCheck = newBoundingBox.BottomLeftGridPosition() + glm::ivec2(i, 0);				// Position of the overlap
			BoundingBox* overlapToCheck = Engine_Pointer->GetCurrentLevel()->GetTileBoundingBox(gridPositionToCheck);	// The AABB of the grid cell
			if (overlapToCheck != nullptr) {
				isColliding = newBoundingBox.Intersect(*overlapToCheck);												// Do the actual intersection check
				if (isColliding) {
					// If we have found a collision, break out the loop because no more checks are needed
					break;
				}
			}
			else {
				isColliding = true;
				break;
			}
		}

		// If there is no collision, Jump
		if (!isColliding) {
			// Apply physics
			isFalling = true;
			velocity.y = newVelocity.y;

			// Work out the objects falling speed for the next update cycle
			fallTimer += deltaTime;
			if (fallTimer >= Engine_Pointer->physicsInterval) {
				float forceIncrement = (timeForMaxFall / Engine_Pointer->physicsInterval);	// Increment the Falling force is applied at
				currentFallingSpeed += (maxFallingSpeed / forceIncrement);
				fallTimer = 0.0f;
			}
			currentFallingSpeed = glm::clamp(currentFallingSpeed, baseFallingSpeed, maxFallingSpeed);
		}
		else {
			// If there was a collision, reset the fall variables
			isFalling = false;
			currentFallingSpeed = baseFallingSpeed;
			fallTimer = 0.0f;
		}
	}
}
void GameObject::PhysicsController(const float& deltaTime) {
	PhysicsHandlerFalling(deltaTime);
}
void GameObject::AnimationController(const float& deltaTime) {
	if (animations.size() > 0) {
		// Store the old animation index for checking if it changes, this is needed for resetting the old animation.
		int oldAnimationIndex = animationIndex;

		// Call the Derived handler function
		AnimationStateHandler();
		AnimationIndexHandler();

		// If the animation has changed, reset the old animation for its next use.
		if (animationIndex != oldAnimationIndex) {
			animations[oldAnimationIndex].Reset();
		}

		// Run the animation
		sourceFramePosition = animations[animationIndex].Run(deltaTime);
	}

}
