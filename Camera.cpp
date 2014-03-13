
#include <cstdio>

#include "GameState.h"

#include <GL/glut.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"

Camera::Camera(glm::vec3 p, glm::vec3 t, glm::vec3 u) {
	m_pos = p;
	m_tar = t;
	m_up = u;
	stepSize = 0.05f;
}

void Camera::onKey() {
	if (GameState::forward) {
		m_pos += m_tar * stepSize;
	}
	if (GameState::backward) {
		m_pos -= m_tar * stepSize;
	}
	if (GameState::left) {
		glm::vec3 left = glm::cross(m_up,m_tar);
		left = glm::normalize(left);
		left *= stepSize;
		m_pos += left;
	}
	if (GameState::right) {
		glm::vec3 right = glm::cross(m_tar,m_up);
		right = glm::normalize(right);
		right *= stepSize;
		m_pos += right;
	}
	if (GameState::up) {
		m_pos += m_up * stepSize;
	}
	if (GameState::down) {
		m_pos -= m_up * stepSize;
	}
	/*
	if (GameState::yaw) {
		glm::vec4 r = glm::vec4(m_tar,0.0f) * glm::rotate(glm::mat4(1.0f),0.05f,m_up);
		m_tar.x = r.x;
		m_tar.y = r.y;
		m_tar.z = r.z;
	}
	*/
}

float use_x = 512, use_y = 384;
float springiness = 10;
void smooth_mouse(float time_d, double x, double y) {
	double d = 1-exp(log(0.5)*springiness*time_d);

	use_x += (x-use_x)*d;
	use_y += (y-use_y)*d;
}

void Camera::onMouse(double x, double y) {
	static double lastTime = glfwGetTime();
	double currTime = glfwGetTime();

	smooth_mouse(currTime - lastTime, x, y);

	float dx = use_x - 512;
	float dy = use_y - 384;
	//printf("%f,%f\n",use_x,use_y);

	glm::vec3 horiz = glm::normalize(glm::cross(m_tar,m_up));
	glm::vec4 r = glm::vec4(m_tar,0.0f) * glm::rotate( glm::mat4(1.0f), 0.0005f * dx, m_up);
	r = r * glm::rotate( glm::mat4(1.0f), 0.0005f * dy, horiz);
	m_tar.x = r.x;
	m_tar.y = r.y;
	m_tar.z = r.z;
	m_tar = glm::normalize(m_tar);
	r = glm::vec4(m_up,0.0f) * glm::rotate( glm::mat4(1.0f), 0.0005f * dy, horiz);
	m_up.x = r.x;
	m_up.y = r.y;
	m_up.z = r.z;
	m_up = glm::normalize(m_up);

	lastTime = currTime;
}

glm::mat4 Camera::view() const {
	return glm::lookAt(m_pos, m_pos + m_tar, m_up);
}
