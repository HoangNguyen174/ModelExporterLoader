#ifndef OPENGL_SHADER_H
#define OPENGL_SHADER_H

#include <iostream>
#include <string>
#include <fstream>

enum ShaderType { VERTEX_SHADER, FRAGMENT_SHADER };

class OpenGLShader
{
	public:
		int m_shaderID;
		ShaderType m_shaderType;
		std::string m_shaderName;

	public:
		OpenGLShader(const ShaderType& type);
		~OpenGLShader() {};
		char* LoadShaderSourceFromFile(const std::string& fileName);
		void LoadAndCompileShader(const std::string& fileName);
	
	private:
		bool IsShaderCompilationSuccess(const std::string& fileName);
};


#endif