#include "Tile.hpp"

const int Tile::TILE_WIDTH = 100;
const int Tile::TILE_HEIGHT = 100;

Tile::Tile()
{
// 	m_tileSprite.LoadSprite("BK11q.png");
// 	m_tileSprite.SetAttributes(Vector2(0.f,0.f),0.f,10,10,100,400,400);
// 	m_tileSprite.LoadSprite("terrain.png");
// 	m_tileSprite.SetAttributes(Vector2(0.f,0.f),0.f,16,16,40,256,256);
	m_tileSprite.LoadSprite("terrain1.png");
	m_tileSprite.SetAttributes(Vector2(0.f,0.f),0.f,16,16,256,512,512);
 	m_tileSprite.SetAnimate(false);
 	m_tileSprite.SetExpired(false);
 	m_tileSprite.SetColor(1.f,1.f,1.f);
	m_isDestroyable = false;
	m_isSolid = false;
	m_topLeftTextureCoords = Vector2(0.f,0.f);
}

// Tile::Tile(const Tile& other)
// {
// 	m_isSolid = other.m_isSolid;
// }

Tile::~Tile()
{

}

void Tile::SetTileMaterial(int index)
{
	m_material = index;
	int materialIndex = 0;
	switch(m_material)
	{
		case 0: materialIndex = 206;
				break;
		case 1: materialIndex = 3;
				break;
		case 2: materialIndex = 222;
				break;
		case 3: materialIndex = 68;
				break;
		case 4: materialIndex = 16;
				break;
		case 5: materialIndex = 7;
				break;
		case 6: materialIndex = 81;
				break;
		case 7: materialIndex = 2;
	}
	m_topLeftTextureCoords = m_tileSprite.GetTopLeftCornerOfFrameIndex(materialIndex);

}

int Tile::GetTileMaterial()
{
	return m_material;
}

bool Tile::IsSolid()
{
	return m_isSolid;
}

void Tile::SetTileSolid()
{
	m_isSolid = true;
}

void Tile::SetBottomLeftCorner(Vector2 bottomLeftCorner)
{
	m_bottomLeftPosition = bottomLeftCorner;
}

void Tile::Update()
{

}

void Tile::Render()
{
	glEnable( GL_TEXTURE_2D );
	glPushMatrix();

	glColor3f(1.f,1.f,1.f);
	if(m_material == 0)
	{
		glDisable( GL_TEXTURE_2D );
		glColor3f(0.f,0.7f,.9f);
	}
	//glBlendFunc(GL_ONE,GL_ONE);
	float width = m_tileSprite.GetFrameWidth();
	float left = m_topLeftTextureCoords.x;
	float right = left + width;
	float top =  m_topLeftTextureCoords.y;
	float bottom = top + m_tileSprite.GetFrameHeight();

	glBindTexture( GL_TEXTURE_2D, m_tileSprite.m_texture->m_openglTextureID );
	glBegin(GL_QUADS);
	{
		//glTexCoord2f( m_topLeftTextureCoords.x , m_topLeftTextureCoords.y + m_tileSprite.GetFrameHeight() );
		//glTexCoord2f( 0.f , .2f );
		glTexCoord2f(left,bottom);
		glVertex2f( m_bottomLeftPosition.x , m_bottomLeftPosition.y );

		//glTexCoord2f( m_topLeftTextureCoords.x + m_tileSprite.GetFrameWidth(),m_topLeftTextureCoords.y + m_tileSprite.GetFrameHeight() );
		//glTexCoord2f( .1f , .2f );
		glTexCoord2f(right,bottom);
		glVertex2f( m_bottomLeftPosition.x + TILE_WIDTH, m_bottomLeftPosition.y );

		//glTexCoord2f( m_topLeftTextureCoords.x + m_tileSprite.GetFrameWidth(),m_topLeftTextureCoords.y );
			//glTexCoord2f( .1f , .1f );
		glTexCoord2f(right,top);
		glVertex2f( m_bottomLeftPosition.x + TILE_WIDTH, m_bottomLeftPosition.y + TILE_HEIGHT );

		//glTexCoord2f( m_topLeftTextureCoords.x , m_topLeftTextureCoords.y );
		//	glTexCoord2f( 0.f , .1f );
		glTexCoord2f(left,top);
		glVertex2f( m_bottomLeftPosition.x , m_bottomLeftPosition.y + TILE_HEIGHT );
	}
	glEnd();
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glPopMatrix();
}
