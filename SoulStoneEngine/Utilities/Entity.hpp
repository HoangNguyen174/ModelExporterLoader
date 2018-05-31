#ifndef ENTITY_H
#define ENTITY_H
#include "Vector2.hpp"
#include "MathUtilities.hpp"

struct Color
{
	float Red;
	float Green;
	float Blue;
};

class Entity
{
	private:
		MathUtilities mathHelper;

	public:
		Vector2 m_position;
		Vector2 m_velocity;
		Vector2 m_accelerate;
		float m_gravityPerFrame;
		float m_isAffectdByGravity;
		float m_opacity;
		float m_expiredTime;
		float m_size;
		bool m_isExpired;
		Color m_color;

	public:
		Entity();
		Entity(Vector2 position,Vector2 velocity,Vector2 accelerate,float m_size,float lifeTime,float opacity);
		~Entity();
		void SetSize(float size);
		void SetPosition(Vector2 position);
		void Update(float elapsedTime);
		void Render();
};
#endif