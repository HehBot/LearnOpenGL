#pragma once

#ifndef GLM
#define GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#endif

#include <cmath>

class Camera {
private:
	glm::vec3 const up;		// always normalized
	glm::vec3 cameraUp;		// always normalized

public:
	glm::vec3 cameraFront;	// always normalized
	glm::vec3 position;
	float yaw;
	float pitch;
	float fov;
	
	Camera(glm::vec3 const &init_position, float const &init_yaw, float const &init_pitch, float const &init_fov = 45.0f);
	
	glm::mat4 view() const;
	
	void updateOrientation(float const &yaw_change, float const &pitch_change);
	
	void updatePosition(glm::vec3 const &pos_change);
	void updateFov(float const &fov_change);
	
	void forward(float const &d);
	void strafeRight(float const &d);
};
