#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera {
	public:
		Camera(glm::vec3,glm::vec3,glm::vec3);
		void onKey();
		glm::mat4 view() const;
	private:
		glm::vec3 m_up;
		glm::mat4 m_trans;
		glm::mat4 m_view;
		float m_rot;
		float stepSize;
		float rotSpeed;
};

#endif
