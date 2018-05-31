#ifndef MATERIAL_H
#define MATERIAL_H
#include "../Utilities/Texture.hpp"
#include <string>
#include "OpenGLShaderProgram.hpp"
#include "../Utilities/Vector3.h"

class Material
{
	public:
		Texture*				m_diffuseTexture;
		Texture*				m_normalTexture;
		Texture*				m_emissiveTexture;
		Texture*				m_specularTexture;
		Texture*				m_depthTexture;
		OpenGLShaderProgram*	m_shaderProgram;
		int						m_shaderProgramID;
		std::string				m_materialName;

	//uniform value
	public:
		const float*			m_worldToScreenMatrix;
		const float*			m_localToWorldMatrix;
		Vector3					m_lightWorldPosition;
		Vector3					m_cameraWorldPosition;
		RGBColor				m_fogColor;
		float					m_fogStartDistance;
		float					m_fogEndDistance;
		float					m_time;
		float					m_deltaTime;
		Vector2					m_scaleBias;
		const float*			m_boneTM[100];

	public:
		Material(const std::string& name, OpenGLShaderProgram* shaderProgram );
		void SetUniformValues();
		void BindTexture();
		void ActivateMaterial();
		void DisableMaterial();
		
};

#endif