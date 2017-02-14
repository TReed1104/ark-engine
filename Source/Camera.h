#ifndef Camera_h_included
#define Camera_h_included

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>


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
	Camera(Engine &engine, glm::vec3 &cameraPosition = glm::vec3(0.0f), glm::vec3 &cameraLookAt = glm::vec3(0.0f), glm::vec3 &upVector = glm::vec3(0.0f, 1.0f, 0.0f));
	~Camera();

	// Public Member Function Declarations
	void Update(float deltaTime, GameObject &object);
	void FollowObject(GameObject &object);
	void ManualControl(void);
	void SetControlMode(CameraMode newMode);
};
#endif