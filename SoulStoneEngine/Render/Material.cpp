#include "GLRender.hpp"
#include "Material.hpp"

Material::Material(const std::string& name, OpenGLShaderProgram* shaderProgram )
{
	m_materialName = name;
	m_shaderProgram = shaderProgram;
	m_shaderProgramID = m_shaderProgram->m_shaderProgramID;
	m_diffuseTexture = nullptr;
	m_normalTexture = nullptr;
	m_emissiveTexture = nullptr;
	m_specularTexture = nullptr;
	m_depthTexture = nullptr;
}

void Material::SetUniformValues()
{
	glUniform1i( m_shaderProgram->m_diffuseTextureUniformLoc, 0 );
	glUniform1i( m_shaderProgram->m_normalTextureUniformLoc, 1 );
	glUniform1i( m_shaderProgram->m_emissiveTextureUniformLoc,2 );
	glUniform1i( m_shaderProgram->m_specularTextureUniformLoc, 3);
	glUniform1i( m_shaderProgram->m_depthTextureUniformLoc, 4);
	glUniformMatrix4fv( m_shaderProgram->m_worldToScreenMatrixLoc, 1, GL_FALSE, m_worldToScreenMatrix );
	glUniformMatrix4fv( m_shaderProgram->m_localToWorldMatrixLoc, 1, GL_FALSE, m_localToWorldMatrix );
	glUniform3f( m_shaderProgram->m_lightWorldPosLoc, m_lightWorldPosition.x, m_lightWorldPosition.y, m_lightWorldPosition.z);
	glUniform3f( m_shaderProgram->m_cameraWorldPositionUniformLoc, m_cameraWorldPosition.x,m_cameraWorldPosition.y,m_cameraWorldPosition.z);
	glUniform1f( m_shaderProgram-> m_timeUniformLoc, m_time );
	glUniform1f( m_shaderProgram-> m_deltaTimeUniformLoc, m_deltaTime );
	glUniform1f( m_shaderProgram->m_fogStartDistanceUniformLoc, m_fogStartDistance );
	glUniform1f( m_shaderProgram->m_fogEndDistanceUniformLoc, m_fogEndDistance );
	glUniform4f( m_shaderProgram->m_fogColorUniformLoc, m_fogColor.m_red, m_fogColor.m_green, m_fogColor.m_blue, m_fogColor.m_alpha);
	glUniform2f( m_shaderProgram->m_scaleBiasUniformLoc, m_scaleBias.x, m_scaleBias.y );
	glUniformMatrix4fv( m_shaderProgram->m_boneTMLoc, 100, GL_FALSE, m_boneTM[0] );
}

void Material::BindTexture()
{
	if( m_diffuseTexture != nullptr)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_diffuseTexture->m_openglTextureID);
	}

	if( m_normalTexture != nullptr )
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_normalTexture->m_openglTextureID);
	}

	if( m_emissiveTexture != nullptr)
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_emissiveTexture->m_openglTextureID);
	}

	if( m_specularTexture != nullptr )
	{
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, m_specularTexture->m_openglTextureID);
	}

	if( m_depthTexture != nullptr )
	{
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, m_depthTexture->m_openglTextureID);
	}
}

void Material::ActivateMaterial()
{
	m_shaderProgram->UseShaderProgram();
}

void Material::DisableMaterial()
{
	m_shaderProgram->DisableShaderProgram();
}