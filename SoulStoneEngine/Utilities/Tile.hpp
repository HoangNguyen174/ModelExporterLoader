#ifndef TILE_H
#define TILE_H
#include <Windows.h>
#include <gl/gl.h>
#include "Texture.hpp"
#include "Vector2.hpp"
#include "Sprite.hpp"
class Tile
{
	private:
		const static int TILE_WIDTH;
		const static int TILE_HEIGHT;
		Sprite m_tileSprite;
		bool m_isSolid;
		bool m_isDestroyable;
		int m_material;
		Vector2 m_topLeftTextureCoords;

	public:
		Vector2 m_bottomLeftPosition;

	public:
		Tile();
		//Tile(const Tile& other);
		~Tile();
		void Render();
		void Update();
		void SetTileSolid();
		bool IsSolid();
		void SetBottomLeftCorner(Vector2 bottleLeftCorner);
		void SetTileMaterial(int indexMaterial);
		int GetTileMaterial();
};
#endif