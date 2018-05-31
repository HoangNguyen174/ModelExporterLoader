#include "SimpleMaterial.hpp"
#include "./Render/GLRender.hpp"

SimpleMaterial::SimpleMaterial(const std::string& name, OpenGLShaderProgram* shaderProgram)
{
	m_materialName = name;
	m_shaderProgram = shaderProgram;
}

void SimpleMaterial::SetUniformValues()
{
	glUniform1i( m_shaderProgram->m_diffuseTextureUniformLoc, 0 );
	glUniform1i( m_shaderProgram->m_specularTextureUniformLoc, 1 );
	glUniform1i( m_shaderProgram->m_normalTextureUniformLoc, 2 );
	glUniform3f( m_shaderProgram->m_lightWorldPosLoc, m_lightWorldPosition.x, m_lightWorldPosition.y, m_lightWorldPosition.z);
	glUniform3f( m_shaderProgram->m_cameraWorldPositionUniformLoc, m_cameraWorldPosition.x,m_cameraWorldPosition.y,m_cameraWorldPosition.z);
	glUniformMatrix4fv( m_shaderProgram->m_worldToScreenMatrixLoc, 1, GL_FALSE, m_worldToScreenMatrix );
}

void SimpleMaterial::BindTexture()
{
	if( m_diffuseTexture != nullptr)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_diffuseTexture->m_openglTextureID);
	}

	if( m_normalTexture != nullptr )
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_normalTexture->m_openglTextureID );
	}

	if( m_specularTexture != nullptr )
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_specularTexture->m_openglTextureID);
	}
}

void SimpleMaterial::ActivateMaterial()
{
	m_shaderProgram->UseShaderProgram();
}

void SimpleMaterial::DisableMaterial()
{
	m_shaderProgram->DisableShaderProgram();
}

