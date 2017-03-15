#ifndef ARKENGINE_CAMERA_H_
#define ARKENGINE_CAMERA_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Engine;
class Tile;
class GameObject;

class Camera
{
public:
	// Public Member Variables.
	enum CameraMode {FreeFollow, LockedFollow, Manual,};
	Engine* engine;
	CameraMode controlMode;
	glm::vec3 position;
	glm::vec3 lookAt;
	glm::vec3 upVector;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

	// Constructors and Deconstructors
	Camera() {}
	Camera(Engine &engine, const glm::vec3& cameraPosition = glm::vec3(0.0f), const glm::vec3& cameraLookAt = glm::vec3(0.0f), const glm::vec3& upVector = glm::vec3(0.0f, 1.0f, 0.0f), const glm::mat4& projectionMatrix = glm::mat4());
	~Camera();

	// Public Member Function Declarations
	void Update(const float& deltaTime, const GameObject& object);
	void FollowObject(const GameObject& object);
	void ManualControl(void);
	void SetControlMode(const CameraMode& newMode);
};
#endif