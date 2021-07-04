#include "Camera.h"

camera::camera(glm::vec3 CamPos, glm::vec3 up)
{
	camPos = CamPos;
	worldUp = up;
	VectorUpdate();
}

glm::mat4 camera::getViewMat()
{
	return glm::lookAt(camPos, camPos + camViewFront, camUp);
}

void camera::setMoveMode(MoveMode Mode)
{
	mode = Mode;
}

void camera::setFOV(float Fov)
{
	fov = Fov;
}

void camera::setSpeed(float Speed)
{
	speed = Speed;
}

void camera::setSensitivity(float Sensitivity)
{
	sensitivity = Sensitivity;
}

void camera::mouseYawPitch(float xOffset, float yOffset)
{
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	yaw += xOffset;
	pitch += yOffset;

	//clamping
	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	else if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}
	VectorUpdate();
}

void camera::setYawPitch(float Yaw, float Pitch)
{
	yaw = Yaw;
	pitch = Pitch;
}

void camera::VectorUpdate()
{
	glm::vec3 ViewFront;
	glm::vec3 moveFront;
	switch (mode)
	{
	case FLY_MODE:
		{
			ViewFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
			ViewFront.y = sin(glm::radians(pitch));
			ViewFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
			moveFront = ViewFront;
			break;
		}
	case FPS_MODE:
		{
			ViewFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
			ViewFront.y = sin(glm::radians(pitch));
			ViewFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

			moveFront.x = cos(glm::radians(yaw));
			moveFront.y = 0;
			moveFront.z = sin(glm::radians(yaw));

			break;
		}
	case DOOM_MODE:
		{
			ViewFront.x = cos(glm::radians(yaw));
			ViewFront.y = 0;
			ViewFront.z = sin(glm::radians(yaw));
			moveFront = ViewFront;
			break;
		}
	}
	camViewFront = glm::normalize(ViewFront);
	camMoveFront = glm::normalize(moveFront);

	camRight = glm::normalize(glm::cross(camViewFront, worldUp));
	camUp = glm::normalize(glm::cross(camRight, camViewFront));
}

void camera::camMove(float factorX, float factorY, float factorZ, float deltaT)
{
	switch (mode)
	{
	case DOOM_MODE:
	case FLY_MODE:
		{
			camPos += (camMoveFront * ((speed * deltaT) * factorX));
			camPos += (camRight * ((speed * deltaT) * factorY));
			camPos += (camUp * ((speed * deltaT) * factorZ));
			break;
		}
	case FPS_MODE:
		{
			float yPos = camPos.y;
			glm::vec3 moveFront;
			camPos += (camMoveFront * ((speed * deltaT) * factorX));
			camPos += (camRight * ((speed * deltaT) * factorY));
			camPos.y = yPos;
			camPos += (worldUp * ((speed * deltaT) * factorZ));
			break;
		}
	}
}

void camera::camTranslate(float coordX, float coordY, float coordZ)
{
	camPos.x = coordX;
	camPos.y = coordY;
	camPos.z = coordZ;
}