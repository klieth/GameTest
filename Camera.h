#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera {
	public:
		Camera(glm::vec3,glm::vec3,glm::vec3);
		void onKey();
		void onMouse(double,double);
		glm::mat4 view() const;
	private:
		glm::vec3 m_pos;
		glm::vec3 m_tar;
		glm::vec3 m_up;
		float stepSize;
};

#endif
