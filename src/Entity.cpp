#include "Entity.h"
#include "Engine.h"

Entity::Entity(const std::string& filePath) : GameObject(filePath) {
	// Load values from our config
	if (configFile->IsLoaded()) {
		name = configFile->Get<std::string>("entity.id");

		// Texture Setup
		int indexOfTexture = -1;
		std::string textureName = configFile->Get<std::string>("entity.rendering.texture");
		// Find the Texture for the Player.
		if ((indexOfTexture = Engine_Pointer->GetIndexOfTexture(textureName)) != -1) {
			// Desired text was found, set the texture.
			texture = Engine_Pointer->textureRegister[indexOfTexture];
		}
		else {
			// Texture could not be found, use the default.
			texture = Engine_Pointer->textureRegister[Engine_Pointer->indexOfDefaultTexture];
		}

		// Positions
		position = glm::vec3(configFile->Get<int>("entity.start position.x"), configFile->Get<int>("entity.start position.y"), 0);
		gridPosition = Engine_Pointer->ConvertToGridPosition(glm::vec2(this->position.x, this->position.y));
		drawOffset = glm::ivec2(configFile->Get<int>("entity.rendering.draw offset.x"), configFile->Get<int>("entity.rendering.draw offset.y"));
		drawPosition = this->position + glm::vec3(this->drawOffset, 0);

		// Attributes
		health = configFile->Get<int>("entity.attributes.health");
		healthBars = configFile->Get<int>("entity.attributes.health bars");
		energy = configFile->Get<int>("entity.attributes.energy");
		baseDamage = configFile->Get<int>("entity.attributes.base damage");
		baseSpecialDamage = configFile->Get<int>("entity.attributes.base special damage");

		// Physics
		isAffectedByGravity = configFile->Get<bool>("entity.physics.use gravity");
		timeForMaxFall = configFile->Get<float>("entity.physics.falling.timer");
		baseFallingSpeed = configFile->Get<float>("entity.physics.falling.base speed");
		maxFallingSpeed = configFile->Get<float>("entity.physics.falling.max speed");
		baseMovementSpeed = configFile->Get<float>("entity.physics.movement.base speed");
		maxMovementSpeed = configFile->Get<float>("entity.physics.movement.max speed");
		currentMovementSpeed = baseMovementSpeed;
		timeForMaxJump = configFile->Get<float>("entity.physics.jumping.timer");
		baseJumpingSpeed = configFile->Get<float>("entity.physics.jumping.base speed");

		// Bounding boxes
		const size_t numberOfBoundingBoxes = configFile->SizeOfObjectArray("entity.bounding boxes");
		for (size_t i = 0; i < numberOfBoundingBoxes; i++) {
			std::string nameOfBoundingBoxes = configFile->Get<std::string>("entity.bounding boxes." + std::to_string(i) + ".bounding box.id");
			if (nameOfBoundingBoxes == "default") {
				boundingBoxOffset = glm::vec2(configFile->Get<int>("entity.bounding boxes." + std::to_string(i) + ".bounding box.offset.x"), configFile->Get<int>("entity.bounding boxes." + std::to_string(i) + ".bounding box.offset.y"));
				glm::vec2 boundingBoxDimensions = glm::vec2(configFile->Get<int>("entity.bounding boxes." + std::to_string(i) + ".bounding box.dimensions.width"), configFile->Get<int>("entity.bounding boxes." + std::to_string(i) + ".bounding box.dimensions.height"));
				boundingBox = BoundingBox(glm::vec2(this->position.x, this->position.y) + boundingBoxOffset, boundingBoxDimensions);
			}
			else if (nameOfBoundingBoxes == "standing") {
				standingBoundingBoxOffset = glm::vec2(configFile->Get<int>("entity.bounding boxes." + std::to_string(i) + ".bounding box.offset.x"), configFile->Get<int>("entity.bounding boxes." + std::to_string(i) + ".bounding box.offset.y"));
				glm::vec2 boundingBoxDimensions = glm::vec2(configFile->Get<int>("entity.bounding boxes." + std::to_string(i) + ".bounding box.dimensions.width"), configFile->Get<int>("entity.bounding boxes." + std::to_string(i) + ".bounding box.dimensions.height"));
				standingBoundingBox = BoundingBox(glm::vec2(this->position.x, this->position.y) + standingBoundingBoxOffset, boundingBoxDimensions);
			}
			else if (nameOfBoundingBoxes == "crawling") {
				canCrawl = true;
				crawlingBoundingBoxOffset = glm::vec2(configFile->Get<int>("entity.bounding boxes." + std::to_string(i) + ".bounding box.offset.x"), configFile->Get<int>("entity.bounding boxes." + std::to_string(i) + ".bounding box.offset.y"));
				glm::vec2 boundingBoxDimensions = glm::vec2(configFile->Get<int>("entity.bounding boxes." + std::to_string(i) + ".bounding box.dimensions.width"), configFile->Get<int>("entity.bounding boxes." + std::to_string(i) + ".bounding box.dimensions.height"));
				crawlingBoundingBox = BoundingBox(glm::vec2(this->position.x, this->position.y) + crawlingBoundingBoxOffset, boundingBoxDimensions);
			}
		}
		// Model Setup
		model = Engine_Pointer->modelRegister[Engine_Pointer->indexOfSpriteModel];
		model.SetMeshParents();
		model.Translate(drawPosition);
		model.Rotate(rotation, glm::vec3(0.0f, 0.0f, 1.0f));
		model.Scale(scale);

		sourceFrameSize = Engine_Pointer->entityTextureFrameSize;

		LoadAnimations(); // Load our animations

		isLoaded = true;
	}
	else {
		Engine_Pointer->engineDebugger.WriteLine(">>>> Entity failed to load Config File: " + filePath);
		isLoaded = false;
	}
}
Entity::~Entity(void) {

}

const int Entity::GetHealth(void) {
	return this->health;
}
void Entity::TakeDamage(const int& damageValue) {
	health -= damageValue;
	// TODO: Improve calculations to support damage values over 100
	if (health <= 0) {
		// Entity was on its last health bar
		if (healthBars == 1) {
			// Call death function and animation
			// TODO: Implement Death
			return;
		}
		// Entity has more health bars left, wrap over the daamge
		if (healthBars > 1) {
			// Calculate the damage wrap
			int healthDeficit = health - damageValue;

			// Ensure the wrap doesn't go positive
			if (healthDeficit > 0) {
				healthDeficit = 0;
			}

			// Apply the calculations
			healthBars -= 1;				// Remove a health bar
			health = 100 + healthDeficit;	// Remove the wrapped damage

		}
	}
}
void Entity::Update(const float& deltaTime) {
	EntityController();
	GameObject::Update(deltaTime);
}

std::map<std::string, std::string> Entity::ExportToUserInterface(void) {
	return std::map<std::string, std::string>();
}

void Entity::UpdatePosition(void) {
	GameObject::UpdatePosition();
	standingBoundingBox.UpdatePosition(glm::vec2(position.x, position.y) + standingBoundingBoxOffset);
	crawlingBoundingBox.UpdatePosition(glm::vec2(position.x, position.y) + crawlingBoundingBoxOffset);

}
void Entity::PhysicsHandlerCrawling(const float& deltaTime) {
	if (canCrawl) {
		// Only run the standing up collision checks if we are actually crawling
		if (isCrawling) {
			// If we are crawling, Check for a collision above the crawling entity
			bool isColliding = false;
			bool doubleBreak = false;	// Hacky way to break out of a double for-loop
			int deltaGridX = abs(boundingBox.TopLeftGridPosition().x - boundingBox.TopRightGridPosition().x);					// Calculate the difference between the topleft and topright
			int deltaGridY = abs(crawlingBoundingBox.TopLeftGridPosition().y - standingBoundingBox.TopLeftGridPosition().y);	// Calculate the difference between the crouching AABB and the Standing AABB

			for (int x = 0; x <= deltaGridX && !doubleBreak; x++) {
				for (int y = 0; y <= deltaGridY; y++) {
					glm::ivec2 gridPositionToCheck = boundingBox.TopLeftGridPosition() + glm::ivec2(x, -y);
					isColliding = LevelManager::GetInstance()->GetCurrentLevel()->IsTileSolid(gridPositionToCheck);
					if (isColliding) {
						doubleBreak = true;	// Break out both for-loops
						break;				// Break out this nested for-loop
					}
				}
			}
			// If there is a collision, decide if we should force a crawl 
			if (isColliding) {
				if (isTryingToCrawl) {
					isCrawling = true;
				}
			}
			else {
				// If there is no collision and the entity isn't trying to crawl, let them stand up
				if (!isTryingToCrawl) {
					isCrawling = false;
				}
			}
		}

		// Toggle between the AABB sizes and positions for crawling and standing
		if (isCrawling) {
			// Amend Bounding Box size and positions
			boundingBox = crawlingBoundingBox;
			boundingBoxOffset = crawlingBoundingBoxOffset;
		}
		else {
			// Check the position above the entity, if the position is a collision set crawling to true
			boundingBox = standingBoundingBox;
			boundingBoxOffset = standingBoundingBoxOffset;
		}
	}
}
void Entity::PhysicsHandlerJumping(const float& deltaTime) {
	// If the entity is not crawling or falling, try and jump
	if (!isFalling && isJumping) {
		// Declare the variables used for the calculations
		glm::vec2 newVelocity = glm::vec2(0.0f, currentJumpingSpeed * deltaTime);
		glm::vec2 newPosition = glm::vec2(position) + newVelocity;
		BoundingBox newBoundingBox = BoundingBox(newPosition + boundingBoxOffset, boundingBox.GetDimensions());
		bool isColliding = false;

		// Calculate the difference in grid cells between the top left of the AABB and the top right, giving us the AABB width in cells
		int deltaGridX = abs(newBoundingBox.TopLeftGridPosition().x - newBoundingBox.TopRightGridPosition().x);
		// For every delta, do another check until we run out of checks or find a collision
		for (size_t i = 0; i <= deltaGridX; i++) {
			glm::ivec2 gridPositionToCheck = newBoundingBox.TopLeftGridPosition() + glm::ivec2(i, 0);					// Position of the overlap
			BoundingBox* overlapToCheck = LevelManager::GetInstance()->GetCurrentLevel()->GetTileBoundingBox(gridPositionToCheck);	// The AABB of the grid cell
			if (overlapToCheck != nullptr) {
				isColliding = newBoundingBox.Intersect(*overlapToCheck);													// Do the actual intersection check
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

		// If there is no collision, Fall
		if (!isColliding) {
			// Apply physics
			velocity.y = newVelocity.y;

			// Work out the objects Jumping speed for the next update cycle
			jumpingTimer += deltaTime;
			if (jumpingTimer >= Engine_Pointer->physicsInterval) {
				float forceIncrement = (timeForMaxJump / Engine_Pointer->physicsInterval);	// Increment the Jumping force is applied at
				currentJumpingSpeed -= (baseJumpingSpeed / forceIncrement);
				jumpingTimer = 0.0f;
			}

			// The jumping upwards force has reached 0, allow a fall to begin
			if (currentJumpingSpeed >= 0) {
				currentJumpingSpeed = baseJumpingSpeed;
				isJumping = false;
				jumpingTimer = 0.0f;
			}
		}
		else {
			// If there was a collision, reset the jump variables
			currentJumpingSpeed = baseJumpingSpeed;
			isJumping = false;
			jumpingTimer = 0.0f;
		}
	}
}
void Entity::PhysicsHandlerFalling(const float& deltaTime) {
	if (!isJumping) {
		GameObject::PhysicsHandlerFalling(deltaTime);
	}
}
void Entity::PhysicsHandlerMovement(const float& deltaTime) {
	// Declare the variables used for the calculations
	glm::vec2 newVelocity = glm::vec2(0.0f);
	glm::vec2 newPosition = glm::vec2(position);
	BoundingBox newBoundingBox = BoundingBox(newPosition + boundingBoxOffset, boundingBox.GetDimensions());

	// Left & Right
	if (movementDirection == Directions::Left) {
		// Set the values used for the calculations
		newVelocity.x = -currentMovementSpeed * deltaTime;
		newPosition.x += newVelocity.x;
		newBoundingBox = BoundingBox(newPosition + boundingBoxOffset, boundingBox.GetDimensions());
		bool isColliding = false;

		// Calculate the difference in grid cells between the top of the AABB and the bottom
		int deltaGridY = abs(newBoundingBox.TopLeftGridPosition().y - newBoundingBox.BottomLeftGridPosition().y);
		// For every delta, do another check until we run out of checks or find a collision
		for (size_t i = 0; i <= deltaGridY; i++) {
			glm::ivec2 gridPositionToCheck = newBoundingBox.TopLeftGridPosition() + glm::ivec2(0, i);					// Position of the overlap
			BoundingBox* overlapToCheck = LevelManager::GetInstance()->GetCurrentLevel()->GetTileBoundingBox(gridPositionToCheck);	// The AABB of the position
			if (overlapToCheck != nullptr) {
				isColliding = newBoundingBox.Intersect(*overlapToCheck);	// Do the actual intersection check
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

		// If there is no collision, Move Left
		if (!isColliding) {
			velocity.x = newVelocity.x;
		}
	}
	else if (movementDirection == Directions::Right) {
		// Set the values used for the calculations
		newVelocity.x = currentMovementSpeed * deltaTime;
		newPosition.x += newVelocity.x;
		newBoundingBox = BoundingBox(newPosition + boundingBoxOffset, boundingBox.GetDimensions());
		bool isColliding = false;

		// Calculate the difference in grid cells between the top of the AABB and the bottom
		int deltaGridY = abs(newBoundingBox.TopRightGridPosition().y - newBoundingBox.BottomRightGridPosition().y);
		// For every delta, do another check until we run out of checks or find a collision
		for (size_t i = 0; i <= deltaGridY; i++) {
			glm::ivec2 gridPositionToCheck = newBoundingBox.TopRightGridPosition() + glm::ivec2(0, i);					// Position of the overlap
			BoundingBox* overlapToCheck = LevelManager::GetInstance()->GetCurrentLevel()->GetTileBoundingBox(gridPositionToCheck);	// The AABB of the position
			if (overlapToCheck != nullptr) {
				isColliding = newBoundingBox.Intersect(*overlapToCheck);	// Do the actual intersection check
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

		// If there is no collision, Move Right
		if (!isColliding) {
			velocity.x = newVelocity.x;
		}
	}
}
void Entity::PhysicsController(const float& deltaTime) {
	PhysicsHandlerMovement(deltaTime);
	PhysicsHandlerCrawling(deltaTime);
	PhysicsHandlerJumping(deltaTime);
	PhysicsHandlerFalling(deltaTime);
}
void Entity::LoadAnimations(void) {
	animations.clear();
	if (configFile->IsLoaded()) {
		const size_t numberOfAnimations = configFile->SizeOfObjectArray("entity.animations");
		if (numberOfAnimations > 0) {
			for (size_t animationIterator = 0; animationIterator < numberOfAnimations; animationIterator++) {
				std::string animationName = configFile->Get<std::string>("entity.animations." + std::to_string(animationIterator) + ".animation.id");
				Animation newAnimation = Animation(animationName);
				const size_t numberOfFrames = configFile->SizeOfObjectArray("entity.animations." + std::to_string(animationIterator) + ".animation.frames");
				for (size_t frameIterator = 0; frameIterator < numberOfFrames; frameIterator++) {
					int frameX = configFile->Get<int>("entity.animations." + std::to_string(animationIterator) + ".animation.frames." + std::to_string(frameIterator) + ".frame.x");
					int frameY = configFile->Get<int>("entity.animations." + std::to_string(animationIterator) + ".animation.frames." + std::to_string(frameIterator) + ".frame.y");
					float frameLength = configFile->Get<float>("entity.animations." + std::to_string(animationIterator) + ".animation.frames." + std::to_string(frameIterator) + ".frame.length");
					newAnimation.AddFrame(glm::ivec2(frameX, frameY), frameLength);
				}
				animations.push_back(newAnimation);
			}
		}
		else {
			Engine_Pointer->engineDebugger.WriteLine(">>>> No animations were present");
		}
	}
}
void Entity::AnimationStateHandler(void) {
	switch (spriteDirection) {
		case Directions::Up:
			animationState = AnimationState::AnimationJumpingLeft;
			break;
		case Directions::Down:
			animationState = AnimationState::AnimationFallingLeft;
			break;
		case Directions::Left:
			if (velocity.x != 0.0f) {
				if (isCrawling) {
					animationState = AnimationState::AnimationMovingCrawlingLeft;
				}
				else {
					animationState = AnimationState::AnimationMovingStandingLeft;
				}
			}
			else {
				if (isCrawling) {
					animationState = AnimationState::AnimationIdleCrawlingLeft;
				}
				else {
					animationState = AnimationState::AnimationIdleStandingLeft;
				}
			}
			break;
		case Directions::Right:
			if (velocity.x != 0.0f) {
				if (isCrawling) {
					animationState = AnimationState::AnimationMovingCrawlingRight;
				}
				else {
					animationState = AnimationState::AnimationMovingStandingRight;
				}
			}
			else {
				if (isCrawling) {
					animationState = AnimationState::AnimationIdleCrawlingRight;
				}
				else {
					animationState = AnimationState::AnimationIdleStandingRight;
				}
			}
			break;
		default:
			break;
	}
}
void Entity::AnimationIndexHandler(void) {
	// Using the current animaiton state, work out which animation in the list to use.
	switch (animationState) {
		case Entity::AnimationIdleStandingLeft:
			animationIndex = 0;
			break;
		case Entity::AnimationIdleStandingRight:
			animationIndex = 1;
			break;
		case Entity::AnimationMovingStandingLeft:
			animationIndex = 2;
			break;
		case Entity::AnimationMovingStandingRight:
			animationIndex = 3;
			break;
		case Entity::AnimationIdleCrawlingLeft:
			animationIndex = 4;
			break;
		case Entity::AnimationIdleCrawlingRight:
			animationIndex = 5;
			break;
		case Entity::AnimationMovingCrawlingLeft:
			animationIndex = 6;
			break;
		case Entity::AnimationMovingCrawlingRight:
			animationIndex = 7;
			break;
		case Entity::AnimationJumpingLeft:
			animationIndex = 0;
			break;
		case Entity::AnimationJumpingRight:
			animationIndex = 0;
			break;
		case Entity::AnimationFallingLeft:
			animationIndex = 0;
			break;
		case Entity::AnimationFallingRight:
			animationIndex = 0;
			break;
		default:
			break;
	}
}
