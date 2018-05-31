#ifndef OPENGL_SHADER_PROGRAM_H
#define OPENGL_SHADER_PROGRAM_H
#include <string>
#include <iostream>
#include "OpenGLShader.hpp"

class OpenGLShaderProgram
{
	public:
		int m_shaderProgramID;
		std::string m_fragmentShaderFullPath;
		int m_numberOfShader;
	
	//uniform location
	public:
		int m_diffuseTextureUniformLoc;
		int m_normalTextureUniformLoc;
		int m_emissiveTextureUniformLoc;
		int m_specularTextureUniformLoc;
		int m_depthTextureUniformLoc;
		int m_worldToScreenMatrixLoc;
		int m_lightWorldPosLoc;
		int m_timeUniformLoc;
		int m_deltaTimeUniformLoc;
		int m_cameraWorldPositionUniformLoc;
		int m_fogColorUniformLoc;
		int m_fogStartDistanceUniformLoc;
		int m_fogEndDistanceUniformLoc;
		int m_scaleBiasUniformLoc;
		int m_localToWorldMatrixLoc;
		int m_boneTMLoc;

	public:
		OpenGLShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
		~OpenGLShaderProgram();
		void AttachShader(const OpenGLShader& shader);
		void LinkProgram();
		void UseShaderProgram();
		void DisableShaderProgram();

	private:
		void GetUniformLocation();
};


#endif