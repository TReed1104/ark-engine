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
	static Engine* Engine_Pointer;

	// Public Member Variables.
	enum CameraMode { Follow, Manual, };
	CameraMode controlMode;
	bool isCameraCenter;

	glm::vec2 viewPort;
	glm::vec3 position;
	glm::vec3 centerPoint;

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

	// Constructors and Deconstructors
	Camera(const glm::vec3& cameraPosition = glm::vec3(0.0f), const CameraMode& cameraMode = CameraMode::Follow);
	~Camera(void);

	// Public Member Function Declarations
	void Update(const float& deltaTime, GameObject& object);
	void FollowObject(const float& deltaTime, GameObject& object);
	void ManualControl(void);
	void SetControlMode(const CameraMode& newMode);
};
#endif