
#include <cstdio>

#include <GL/glut.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"
#include "GameState.h"

Camera::Camera(glm::vec3 p, glm::vec3 t, glm::vec3 u) {
	m_up = u;
	m_view = glm::lookAt(p,t,u);
	stepSize = 0.05f;
	rotSpeed = 0.05f;
}

void Camera::onKey() {
	if (GameState::forward) {
		m_trans = glm::translate(m_trans, glm::vec3(0,0,stepSize*2));
	}
	if (GameState::backward) {
		m_trans = glm::translate(m_trans, glm::vec3(0,0,-stepSize*2));
	}
	if (GameState::left) {
		m_trans = glm::translate(m_trans, glm::vec3(stepSize,0,0));
	}
	if (GameState::right) {
		m_trans = glm::translate(m_trans, glm::vec3(-stepSize,0,0));
	}
	if (GameState::yaw) {
		m_rot += rotSpeed;
	}
	//m_view = glm::rotate(m_trans,m_rot,m_up);
}

glm::mat4 Camera::view() const {
	return glm::rotate(m_trans, m_rot, m_up);
}
