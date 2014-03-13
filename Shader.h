#ifndef SHADER_H
#define SHADER_H

class Shader {
	public:
		static bool addShader(const char* pShaderText, GLenum shaderType);
		static bool compileShaders();
		static bool setWorldVars();
		static bool init();
		static const char* getDefaultVertexShader();
		static const char* getDefaultFragmentShader();
	private:
		static const char* vS;
		static const char* fS;
		static GLuint shaderProgram;
};

#endif
