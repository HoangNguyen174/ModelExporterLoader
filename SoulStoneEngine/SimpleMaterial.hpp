#ifndef SIMPLE_MATERIAL_H
#define SIMPLE_MATERIAL_H
#include <string>
#include "./Utilities/GameCommon.hpp"
#include "./Render/OpenGLShaderProgram.hpp"

class SimpleMaterial
{
	public:
		std::string				m_materialName; 
		Texture*				m_diffuseTexture;
		Texture*				m_specularTexture;
		Texture*				m_normalTexture;
		OpenGLShaderProgram*	m_shaderProgram;
		const float*			m_worldToScreenMatrix;
		Vector3					m_lightWorldPosition;
		Vector3					m_cameraWorldPosition;

	public:
		SimpleMaterial( const std::string& name, OpenGLShaderProgram* shaderProgram );
		void SetUniformValues();
		void BindTexture();
		void ActivateMaterial();
		void DisableMaterial();
};


#endif