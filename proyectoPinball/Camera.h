#pragma once

#include <glew.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>

#include <glfw3.h>

class Camera
{
public:
	Camera();
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);

	void keyControl(bool* keys, GLfloat deltaTime);
	void mouseControl(GLfloat xChange, GLfloat yChange);

	glm::vec3 getCameraPosition();
	glm::vec3 getCameraDirection();
	glm::mat4 calculateViewMatrix();
	//Getter de mi ubicación
	float getPosicionX() { return position.x; }
	float getPosicionY() { return position.y; }
	float getPosicionZ() { return position.z; }
	//Setter de mi ubicación
	float getVistaX() { return yaw; }
	float getVistaY() { return pitch; }
	float getVistaZ() { /*Se debe obtener la posición de la camara en Z*/ }
	//Setter para modificar la posición periodicamente desde Main
	void setPosicionX(float posicionX) { position.x = posicionX; }
	void setPosicionY(float posicionY) { position.y = posicionY; }
	void setPosicionZ(float posicionZ) { position.z = posicionZ; }
	//Setter para modificar la vista periodicamente desde Main
	void setVistaX(GLfloat vistaX) { yaw = vistaX; }
	void setVistaY(GLfloat vistaY) { pitch = vistaY; }
	void setVistaZ() { /*Se debe asignar la posición de la camara en Z*/ }
	~Camera();

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;

	GLfloat moveSpeed;
	GLfloat turnSpeed;

	void update();
};

