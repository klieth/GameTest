// Basic includes
#include <cstdio>
#include <cstring>
#include <vector>
#include <cmath>

// GLEW and GLUT
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// GL math
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Camera.h"
#include "GameState.h"

GLuint VBO;
GLuint IBO;
GLuint gWorldLocation;

Camera gameCamera(glm::vec3(0,0,5),glm::vec3(0,0,0),glm::vec3(0,1,0));

static const char* vS = "\n\
#version 330 \n\
layout(location = 0) in vec3 pos; \n\
out vec4 calc_color; \n\
uniform mat4 gWorld; \n\
void main() \n\
{ \n\
	gl_Position = gWorld * vec4(pos, 1.0); \n\
	calc_color = vec4(clamp(pos,0.0,1.0),1.0);\n\
}";

static const char* fS = "\n\
#version 330 \n\
in vec4 calc_color; \n\
out vec4 color; \n\
void main() \n\
{ \n\
	//color = vec4(1.0, 0.0, 0.0, 1.0); \n\
	color = calc_color; \n\
}";

static void renderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// set up the scale for each frame

	static float param = 0.0f;
	param += 0.01f;
	//float sin_calc = sinf(param);

	gameCamera.onKey();
	glm::mat4 scale = glm::mat4(1.0f);
	//glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), sin_calc * 10.0f, glm::vec3( 0,1,0 ) );
	glm::mat4 rotate(1.0f);
	glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3( 0,0,0.5f ) );
	glm::mat4 perspective = glm::perspective(glm::pi<float>() / 6.0f, 4.0f/3.0f, 1.0f, 100.0f);
	//glm::mat4 view = glm::lookAt( gameCamera.pos(), gameCamera.tar(), gameCamera.up());
	//printf("Camera: %s\n",glm::to_string(gameCamera.pos()).c_str());
	//glm::mat4 world = perspective * view * translate * rotate * scale;
	glm::mat4 world = perspective * gameCamera.view() * translate * rotate * scale;

	glUniformMatrix4fv(gWorldLocation, 1, GL_FALSE, &world[0][0]);

	// load the buffer
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	// Do the drawing
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);

	// clean up
	glDisableVertexAttribArray(0);

}

/*
static void onKeyPress(unsigned char key, int x, int y) {
	printf("%s %d %d\n",&key, x, y);
	gameCamera.onKeyboard(key);
}
*/

static void createVertexBuffer() {
	/*
	glm::vec3 vertices[4];
	vertices[0] = glm::vec3(-1.0f,-1.0f,0.0f);
	vertices[1] = glm::vec3(0.0f,-1.0f,1.0f);
	vertices[2] = glm::vec3(1.0f,-1.0f,0.0f);
	vertices[3] = glm::vec3(0.0f,1.0f,0.0f);
	*/
	glm::vec3 vertices[5];
	vertices[0] = glm::vec3(-1.0f,-1.0f,0.0f);
	vertices[1] = glm::vec3(0.0f,-1.0f,1.0f);
	vertices[2] = glm::vec3(1.0f,-1.0f,0.0f);
	vertices[3] = glm::vec3(0.0f,-1.0f,-1.0f);
	vertices[4] = glm::vec3(0.0f,1.0f,0.0f);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

static void createIndexBuffer() {
	/*
	unsigned int indices[] = { 0, 3, 1,
							   1, 3, 2,
							   2, 3, 0,
							   0, 1, 2  };
							   */
	unsigned int indices[] = { 0, 1, 2,
							   0, 2, 3,
							   0, 1, 4,
							   1, 2, 4,
							   2, 3, 4,
							   3, 0, 4  };

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

static void addShader(GLuint shaderProgram, const char* pShaderText, GLenum shaderType) {
	GLuint shaderObj = glCreateShader(shaderType);
	
	if (shaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", shaderType);
		exit(0);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint lengths[1];
	lengths[0] = strlen(pShaderText);
	glShaderSource(shaderObj, 1, p, lengths);
	glCompileShader(shaderObj);
	GLint success;
	glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar infoLog[1024];
		glGetShaderInfoLog(shaderObj, 1024, NULL, infoLog);
		fprintf(stderr, "Error compiling shader type %d: %s\n", shaderType, infoLog);
		exit(1);
	}
	glAttachShader(shaderProgram, shaderObj);
}

static void compileShaders() {
	GLuint shaderProgram = glCreateProgram();

	if (shaderProgram == 0) {
		fprintf(stderr, "Error creating shader program\n");
		exit(1);
	}

	addShader(shaderProgram, vS, GL_VERTEX_SHADER);
	addShader(shaderProgram, fS, GL_FRAGMENT_SHADER);

	GLint success = 0;
	GLchar errorLog[1024] = { 0 };
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (success == 0) {
		glGetProgramInfoLog(shaderProgram, sizeof(errorLog), NULL, errorLog);
		fprintf(stderr, "Error linking shader program: %s\n", errorLog);
		exit(1);
	}

	glValidateProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, sizeof(errorLog), NULL, errorLog);
		fprintf(stderr, "Invalid shader program: %s\n", errorLog);
		exit(1);
	}

	glUseProgram(shaderProgram);

	gWorldLocation = glGetUniformLocation(shaderProgram, "gWorld");
	if (gWorldLocation == 0xFFFFFFFF) {
		fprintf(stderr, "Invalid uniform location");
		exit(1);
	}
}

void error_callback(int error, const char* description) {
	fprintf(stderr, "ERROR: %s\n", description);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	switch (key) {
		case GLFW_KEY_ESCAPE:
			if (action == GLFW_PRESS) {
				glfwSetWindowShouldClose(window, GL_TRUE);
			}
			break;
		case GLFW_KEY_W:
			if (action == GLFW_PRESS) {
				GameState::forward = 1;
			} else if (action == GLFW_RELEASE) {
				GameState::forward = 0;
			}
			break;
		case GLFW_KEY_S:
			if (action == GLFW_PRESS) {
				GameState::backward = 1;
			} else if (action == GLFW_RELEASE) {
				GameState::backward = 0;
			}
			break;
		case GLFW_KEY_A:
			if (action == GLFW_PRESS) {
				GameState::left = 1;
			} else if (action == GLFW_RELEASE) {
				GameState::left = 0;
			}
			break;
		case GLFW_KEY_D:
			if (action == GLFW_PRESS) {
				GameState::right = 1;
			} else if (action == GLFW_RELEASE) {
				GameState::right = 0;
			}
			break;
		case GLFW_KEY_E:
			if (action == GLFW_PRESS) {
				GameState::yaw = 1;
			} else if (action == GLFW_RELEASE) {
				GameState::yaw = 0;
			}
			break;
		default:
			if (action == GLFW_PRESS) {
				printf("Not a known key.\n");
			}
			break;
	}
}

int main(int argc, char** argv) {
	if (!glfwInit()) {
		fprintf(stderr, "Could not initialize glfw\n");
		return 1;
	}
	glfwSetErrorCallback(error_callback);

	GLFWwindow *win = glfwCreateWindow(1024, 768, "Game", NULL, NULL);
	if (!win) {
		fprintf(stderr, "Could not create glfw window\n");
		return 1;
	}
	glfwMakeContextCurrent(win);
	glfwSetKeyCallback(win,key_callback);

	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(res));
		return 1;
	}

	glClearColor(0.0f,0.0f,0.4f,0.0f);

	createVertexBuffer();
	createIndexBuffer();
	compileShaders();

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(win)) {
		renderScene();
		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	glfwDestroyWindow(win);
	glfwTerminate();
	return 0;
}
