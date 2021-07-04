#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

//defaults
const float YAW = -90.0F;
const float PITCH = 0.0f;
const float SPEED = 3.0f;
const float SENSITIVITY = 0.1f;
const float FOV = 45.0f;

typedef enum{FLY_MODE, FPS_MODE, DOOM_MODE}MoveMode;	//FLY_MODE: full movement in view direction, FPS_MODE: movement in view direction without vertical movement, DOOM_MODE FPS_MODE without vertical head movement

class camera
{
public:
	glm::vec3 camPos;
	glm::vec3 camViewFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 camMoveFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 camUp;
	glm::vec3 worldUp;
	glm::vec3 camRight;

	float yaw = YAW;
	float pitch = PITCH;

	float speed = SPEED;
	float sensitivity = SENSITIVITY;
	float fov = FOV;

	MoveMode mode = FLY_MODE;

	camera(glm::vec3 CamPos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));	//contructor
	glm::mat4 getViewMat();
	void setMoveMode(MoveMode mode);	//set movement mode
	void setFOV(float Fov = FOV);
	void setSpeed(float Speed = SPEED);
	void setSensitivity(float Sensitivity = SENSITIVITY);
	void mouseYawPitch(float xOffset, float yOffset);
	void setYawPitch(float Yaw, float Pitch);
	void VectorUpdate();
	void camMove(float factorX, float factorY, float factorZ, float deltaT = 0.01f);
	void camTranslate(float coordX, float coordY, float coordZ);
};

#endif