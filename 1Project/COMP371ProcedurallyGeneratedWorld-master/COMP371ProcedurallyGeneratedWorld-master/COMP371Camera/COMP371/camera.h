#ifndef CAMERA_H
#define CAMERA_H


#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 10.0f;
const float SENSITIVTY = 0.1f;
const float ZOOM = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
	// Camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// Eular Angles
	float Yaw;
	float Pitch;
	// Camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	// Constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.2f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}
	// Constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	float getPositionZ() {
		float z;
		z = Position.z;
		return z;
	}

	float getPositionX() {
		float x;
		x = Position.x;
		return x;
	}

	float getPositionY() {
		float y;
		y = Position.y;
		return y;
	}

	void gravity(float crd) {
		if (Position.y > 1.2f)
			Position.y = Position.y - crd;
	}

	void standUp(float crd) {
		if (Position.y < 1.2f)
			Position.y = Position.y + crd;
	}

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		float velocity = MovementSpeed * deltaTime;
		if (direction == FORWARD) {
			if (Position.x + Right.x*velocity <= 4.8f && Position.x + Right.x*velocity >= -4.8f) {
				Position.z += Front.z * velocity;
				Position.x += Right.z * velocity;
			}
			if (Position.x + Right.x*velocity > 4.6f) {
				Position.x = 4.59f;
			}
			if (Position.x + Right.x*velocity < -4.6f) {
				Position.x = -4.59f;
			}
		}
		if (direction == BACKWARD) {
			if (Position.x - Right.x*velocity <= 4.8f && Position.x - Right.x*velocity >= -4.8f) {
				Position.z -= Front.z * velocity;
				Position.x -= Right.z * velocity;
			}
			if (Position.x - Right.x*velocity > 4.6f) {
				Position.x = 4.59f;
			}
			if (Position.x - Right.x*velocity < -4.6f) {
				Position.x = -4.59f;
			}
		}
		if (direction == LEFT) {
			if (Position.x - Right.x*velocity <= 4.8f && Position.x - Right.x*velocity >= -4.8f) {
				Position.z -= Front.x * velocity;
				Position.x -= Right.x * velocity;
			}
			if (Position.x + Right.x*velocity > 4.6f) {
				Position.x = 4.59f;
			}
			if (Position.x - Right.x*velocity < -4.6f) {
				Position.x = -4.59f;
			}
		}
		if (direction == RIGHT) {
			if (Position.x + Right.x*velocity <= 4.8f && Position.x + Right.x*velocity >= -4.8f) {
				Position.z += Front.x * velocity;
				Position.x += Right.x * velocity;
			}
			if (Position.x + Right.x*velocity > 4.6f) {
				Position.x = 4.59f;
			}
			if (Position.x - Right.x*velocity < -4.6f) {
				Position.x = -4.59f;
			}
		}
		if (direction == UP) {
			if (Position.y <= 1.3f)
				Position.y = 2.5f;
		}
		if (direction == DOWN) {
			if (Position.y != 1.2f)
				Position.y = 0.9f;
		}
	}

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		// Update Front, Right and Up Vectors using the updated Eular angles
		updateCameraVectors();
	}

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(float yoffset)
	{
		if (Zoom >= 1.0f && Zoom <= 45.0f)
			Zoom -= yoffset;
		if (Zoom <= 1.0f)
			Zoom = 1.0f;
		if (Zoom >= 45.0f)
			Zoom = 45.0f;
	}

private:
	// Calculates the front vector from the Camera's (updated) Eular Angles
	void updateCameraVectors()
	{
		// Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		// Also re-calculate the Right and Up vector
		Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = glm::normalize(glm::cross(Right, Front));
	}
};
#endif