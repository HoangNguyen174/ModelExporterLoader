#include "FBO.hpp"
#include "GLRender.hpp"
#include "../Utilities/GameCommon.hpp"

FBO::FBO()
{
	m_fboBufferDepthTextureID = 0;
	m_fboColorTextureID = 0;
	m_fboID = 0;
	Initialize();
}

void FBO::Initialize()
{
	// Create color frame buffer texture
	glActiveTexture( GL_TEXTURE0 );
	glGenTextures( 1, &m_fboColorTextureID );
	glBindTexture( GL_TEXTURE_2D, m_fboColorTextureID );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL ); // NOTE: RGBA and UNSIGNED BYTE

	// Create depth frame buffer texture
	glActiveTexture( GL_TEXTURE1 );
	glGenTextures( 1, &m_fboBufferDepthTextureID );
	glBindTexture( GL_TEXTURE_2D, m_fboBufferDepthTextureID );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL ); // NOTE: DEPTH and UNSIGNED INT

	glGenFramebuffers( 1, &m_fboID );
	glBindFramebuffer( GL_FRAMEBUFFER, m_fboID );

	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fboColorTextureID, 0 );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_fboBufferDepthTextureID, 0);
}

FBO* FBO::CreateFBO()
{
	FBO* fbo = new FBO();
	return fbo;
}