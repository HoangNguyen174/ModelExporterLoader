#ifndef TEXTURE_H
#define TEXTURE_H
#include "Vector2.hpp"
#include "string"
#include <map>
#include <Windows.h>
#include <gl/gl.h>
#include "../Render/glext.h"
#include "Vector2i.hpp"
using namespace std;

class Texture
{
	private:	
		Texture(const string& imageFilePath);

	public:
		Vector2i m_size;
		int m_component;
		//char *m_image;
		int m_openglTextureID;
		static map<string, Texture*> s_textureRegistry;
		
	public:
		Texture();
		static Texture *GetTextureByName(const string& imageFilePath);
		static Texture *CreateOrGetTexture(const string& imageFilePath);
};


#endif