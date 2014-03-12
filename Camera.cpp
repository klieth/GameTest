
#include <cstdio>

#include <GL/glut.h>
#include <glm/glm.hpp>

#include "Camera.h"

Camera::Camera() {
	m_pos = glm::vec3(5,0,5);
	m_tar = glm::vec3(0,0,0);
	m_up  = glm::vec3(0,1,0);
	stepSize = 0.04f;
}

bool Camera::onKeyboard(int key) {
	bool ret = false;
	switch (key) {
		case 'w':
			m_pos -= (m_pos - m_tar) * stepSize;
			ret = true;
			break;
		case 's':
			m_pos += (m_pos - m_tar) * stepSize;
			ret = true;
			break;
	}
	return ret;
}

glm::vec3 Camera::pos() const {
	return m_pos;
}
glm::vec3 Camera::tar() const {
	return m_tar;
}
glm::vec3 Camera::up() const {
	return m_up;
}
