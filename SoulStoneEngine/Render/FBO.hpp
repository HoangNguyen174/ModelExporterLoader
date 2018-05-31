#ifndef FBO_H
#define FBO_H
#include <Windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

class FBO
{
	public:
		GLuint m_fboID;
		GLuint m_fboColorTextureID;
		GLuint m_fboBufferDepthTextureID;

	private:
		FBO();
		void Initialize();
		void GetUniforLocation();

	public:
		static FBO* CreateFBO();
		
};

#endif;