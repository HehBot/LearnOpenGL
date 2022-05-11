#ifndef GLM
#define GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#endif

#include <cmath>

#include "Camera.hpp"

Camera::Camera(glm::vec3 const &init_position, float const &init_yaw, float const &init_pitch, float const &init_fov):
	up(glm::vec3(0.0f, 1.0f, 0.0f)), position(init_position), yaw(init_yaw),
	pitch(init_pitch), fov(init_fov > 60.0f ? 60.0f : (init_fov < 1.0f ? 1.0f : init_fov))
{
	float yawR = glm::radians(init_yaw), pitchR = glm::radians(init_pitch);

	cameraFront = glm::vec3(std::cos(yawR) * std::cos(pitchR), std::sin(pitchR), std::sin(yawR) * std::cos(pitchR));
	glm::vec3 right = glm::normalize(glm::cross(cameraFront, up));
	cameraUp = glm::cross(right, cameraFront);
}
	
glm::mat4 Camera::view() const
{
	return glm::lookAt(position, position + cameraFront, cameraUp);
}
	
void Camera::updateOrientation(float const &yaw_change, float const &pitch_change)
{
	yaw += yaw_change;
	pitch += pitch_change;
		
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
		
	float yawR = glm::radians(yaw), pitchR = glm::radians(pitch);
	cameraFront = glm::vec3(cos(yawR) * cos(pitchR), sin(pitchR), sin(yawR) * cos(pitchR));
	glm::vec3 right = glm::normalize(glm::cross(cameraFront, up));
	cameraUp = glm::cross(right, cameraFront);
}
	
void Camera::updatePosition(glm::vec3 const &pos_change)
{
	position += pos_change;
}
	
void Camera::updateFov(float const &fov_change)
{
	fov += fov_change;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 60.0f)
		fov = 60.0f;
}
	
void Camera::forward(float const &d)
{
	position += d * cameraFront;
}
	
void Camera::strafeRight(float const &d)
{
	position += d * glm::normalize(glm::cross(cameraFront, cameraUp));
}
