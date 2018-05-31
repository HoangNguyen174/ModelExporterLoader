#ifndef SPRITE_H
#define SPRITE_H
#include "Texture.hpp"
#include "Vector2.hpp"
#include "MathUtilities.hpp"
#include <vector>
#include <string>

struct FrameCoords
{
	Vector2 topLeft;
	Vector2 bottomRight;
};

class Sprite
{
	public:
		Texture *m_texture;
		bool m_isBlending;
	private:
		float m_scaleX;
		float m_scaleY;
		float m_frameWidth;
		float m_frameHeight;
		int m_numberFrameWide;
		int m_numberFrameHeight;
		int m_numberFrameTotal;
		int m_currentFrame;
		int m_textureWidth;
		int m_texureHeight;
		float m_frameTime;
		float m_timeForCurrentFrame;
		Vector2 m_position;
		float m_orientaion;
		bool m_isAnimate;
		bool m_isExpired;
		float m_frameTimeRemaining;
		FrameCoords m_frameCoords;
		vector<FrameCoords> frameCoordsList;
		float m_red;
		float m_green;
		float m_blue;
		

	public:
		Sprite();
		void LoadSprite(string textureName);
		void SetAttributes(Vector2 position, float orientation,int numberFrameWide,
						  int numberFrameHeight,int numberFrameTotal,int textureWidth, int textureHeight);
		~Sprite();
		void Update(float elapsedTime);
		void Render();
		void SetPosition(Vector2 position);
		bool IsSpriteExpired();
		void SetExpired(bool value);
		void SetColor(float red,float green, float blue);
		int GetCurrentFrameIndex();
		void SetCurrentFrameIndex(int value);
		void SetAnimate(bool value);
		void SetScaleXY(float xScale,float yScale);
		void SetFrameTime(float time);
		Vector2 GetTopLeftCornerOfFrameIndex(int index);
		float GetFrameWidth();
		float GetFrameHeight();
		void SetOrientationDegree(float value);

	private:
		void CreateFrameList();
};

#endif
