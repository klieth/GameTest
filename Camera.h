#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera {
	public:
		Camera();
		bool onKeyboard(int key);
		glm::vec3 pos() const;
		glm::vec3 tar() const;
		glm::vec3 up() const;
	private:
		glm::vec3 m_pos;
		glm::vec3 m_tar;
		glm::vec3 m_up;
		float stepSize;
};

#endif
