#include "Sprite.hpp"

Sprite::Sprite()
{
	m_isAnimate = false;
	m_isExpired = false;
	m_isBlending = false;
}

void Sprite::LoadSprite(string textureName)
{
	m_texture = nullptr;
	//m_texture = new Texture();
	m_texture = m_texture->CreateOrGetTexture(textureName);
	//m_texture = new Texture(textureName);
}

void Sprite::SetAttributes( Vector2 position,float orientation,int numberFrameWide,
						  int numberFrameHeight, int numberFrameTotal, int textureWidth, int textureHeight)
{
	m_position = position;
	m_orientaion = orientation;
	m_numberFrameWide = numberFrameWide;
	m_numberFrameHeight = numberFrameHeight;
	m_numberFrameTotal = numberFrameTotal;
	m_textureWidth = textureWidth;
	m_texureHeight = textureHeight;
	m_frameWidth = 1.f/m_numberFrameWide;
	m_frameHeight = 1.f/m_numberFrameHeight;
	//m_isAnimate = false;
	//m_isExpired = true;
	m_frameTime = 0.05f;
	m_frameTimeRemaining = m_frameTime;
	m_currentFrame = 0;
	m_scaleX = 1.f;
	m_scaleY = 1.f;
	CreateFrameList();
}

void Sprite::SetFrameTime(float time)
{
	m_frameTime = time;
	m_frameTimeRemaining = m_frameTime;
}

Sprite::~Sprite()
{
	m_texture = nullptr;
	delete m_texture;
}

bool Sprite::IsSpriteExpired()
{
	return m_isExpired;
}

void Sprite::SetColor(float red, float green , float blue)
{
	m_red = red;
	m_green = green;
	m_blue = blue;
}

void Sprite::SetAnimate(bool value)
{
	m_isAnimate = value;
}

void Sprite::SetPosition(Vector2 position)
{
	m_position = position;
}

void Sprite::SetExpired(bool value)
{
	m_isExpired = value;
}

void Sprite::SetCurrentFrameIndex(int value)
{
	m_currentFrame = value;
}

void Sprite::SetScaleXY(float xScale,float yScale)
{
	m_scaleX = xScale;
	m_scaleY = yScale;
}

int Sprite::GetCurrentFrameIndex()
{
	return m_currentFrame;
}

void Sprite::CreateFrameList()
{
	int frameRows;
	int frameCols;
	FrameCoords frameCoords; 
	for(frameCols = 0 ; frameCols < m_numberFrameHeight;frameCols++)
	{
		for(frameRows = 0; frameRows < m_numberFrameWide;frameRows++)
		{
			frameCoords.topLeft = Vector2( frameRows * m_frameWidth, frameCols * m_frameHeight);
			//frameCoords.bottomRight = Vector2( (frameRows + 1) * m_frameWidth, (frameCols + 1) * m_frameHeight );
			frameCoordsList.push_back(frameCoords);
		}
	}
}

float Sprite::GetFrameWidth()
{
	return m_frameWidth;
}

float Sprite::GetFrameHeight()
{
	return m_frameHeight;
}

void Sprite::Update(float elapsedTime)
{
	if(m_isAnimate)
	{
		m_frameTimeRemaining -= elapsedTime;
		if(m_frameTimeRemaining <= 0 && m_currentFrame < m_numberFrameTotal)
		{
			m_currentFrame++;
			m_frameTimeRemaining = m_frameTime;
		}
		if(m_currentFrame == m_numberFrameTotal)
		{
			m_isExpired = true;
			m_currentFrame = 0;
		}
	}
}

void Sprite::SetOrientationDegree(float value)
{
	m_orientaion = value;
}

Vector2 Sprite::GetTopLeftCornerOfFrameIndex(int index)
{
	return frameCoordsList[index].topLeft;
}

void Sprite::Render()
{
	glEnable( GL_TEXTURE_2D );
	//glDisable(GL_TEXTURE_2D);
	glPushMatrix();
	if(m_isBlending)
		glBlendFunc(GL_ONE,GL_ONE);
	glColor3f(m_red,m_green,m_blue);
	glTranslatef( m_position.x,m_position.y,0.f );
	glRotatef( m_orientaion,0.f,0.f,1.f);
	glScalef(m_scaleX,m_scaleY,0.f);
	glBindTexture( GL_TEXTURE_2D, m_texture->m_openglTextureID );
	glBegin( GL_QUADS );
	{
		glTexCoord2f(frameCoordsList[m_currentFrame].topLeft.x,frameCoordsList[m_currentFrame].topLeft.y + m_frameHeight);
		glVertex2f( -50.f, -50.f );
		glTexCoord2f(frameCoordsList[m_currentFrame].topLeft.x + m_frameWidth,frameCoordsList[m_currentFrame].topLeft.y + m_frameHeight);
		glVertex2f( 50.f, -50.f );
		glTexCoord2f(frameCoordsList[m_currentFrame].topLeft.x + m_frameWidth,frameCoordsList[m_currentFrame].topLeft.y);
		glVertex2f( 50.f, 50.f );
		glTexCoord2f(frameCoordsList[m_currentFrame].topLeft.x,frameCoordsList[m_currentFrame].topLeft.y);
		glVertex2f( -50.f, 50.f );
	}
	glEnd();
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glPopMatrix();
}