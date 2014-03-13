
#include <cstdio>
#include <cstring>

#include <GL/glew.h>

#include "Shader.h"
#include "GameState.h"

GLuint Shader::shaderProgram;
const char* Shader::vS;
const char* Shader::fS;

bool Shader::addShader(const char* pShaderText, GLenum shaderType) {
	GLuint shaderObj = glCreateShader(shaderType);

	if (shaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", shaderType);
		return 0;
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
		return 0;
	}
	glAttachShader(shaderProgram, shaderObj);
	return 1;
}

bool Shader::compileShaders() {
	GLint success = 0;
	GLchar errorLog[1024] = { 0 };
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (success == 0) {
		glGetProgramInfoLog(shaderProgram, sizeof(errorLog), NULL, errorLog);
		fprintf(stderr, "Error linking shader program: %s\n", errorLog);
		return 0;
	}

	glValidateProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, sizeof(errorLog), NULL, errorLog);
		fprintf(stderr, "Invalid shader program: %s\n", errorLog);
		return 0;
	}

	glUseProgram(shaderProgram);
	return 1;
}

bool Shader::setWorldVars() {
	GameState::gWorldLocation = glGetUniformLocation(shaderProgram, "gWorld");
	if (GameState::gWorldLocation == 0xFFFFFFFF) {
		fprintf(stderr, "Invalid uniform location");
		return 0;
	}
	return 1;
}

bool Shader::init() {
	shaderProgram =  glCreateProgram();
	if (shaderProgram == 0) {
		fprintf(stderr, "Error creating shader program");
		return 0;
	}


	vS = "\
#version 330 \n\
layout(location = 0) in vec3 pos; \n\
out vec4 calc_color; \n\
uniform mat4 gWorld; \n\
void main() \n\
{ \n\
	gl_Position = gWorld * vec4(pos, 1.0); \n\
	calc_color = vec4(clamp(pos,0.0,1.0),1.0); \n\
}";



	fS = "\
#version 330 \n\
in vec4 calc_color; \n\
out vec4 color; \n\
void main() \n\
{ \n\
	color = calc_color; \n\
}";

	return 1;
}

const char* Shader::getDefaultVertexShader() {
	return vS;
}
const char* Shader::getDefaultFragmentShader() {
	return fS;
}

