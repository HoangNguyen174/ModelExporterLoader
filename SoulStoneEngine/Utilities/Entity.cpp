#include "Entity.hpp"
#include <Windows.h>
#include <gl/gl.h>

Entity::Entity()
{
	m_position = Vector2(0.f,0.f);
	m_velocity = Vector2(0.f,0.f);
	m_accelerate = Vector2(0.f,0.f);
	m_opacity = 1.f;
	m_size = 0.f;
	m_expiredTime = 1.5f;
	m_isExpired = false;
}

Entity::Entity(Vector2 position,Vector2 velocity,Vector2 accelerate, float size,float lifeTime,float opacity)
{
	m_position = position;
	m_velocity = velocity;
	m_accelerate = accelerate;
	m_size = size;
	m_expiredTime = lifeTime;
	m_opacity = opacity;
}

Entity::~Entity()
{

}

void Entity::SetPosition(Vector2 position)
{
	m_position.x = position.x;
	m_position.y = position.y; 
}

void Entity::SetSize(float size)
{
	m_size = size;
}

void Entity::Update(float elapsedTime)
{
	if(m_opacity > 0.1)
		m_opacity -= elapsedTime;
	m_expiredTime -= elapsedTime;
	if(m_expiredTime < 0)
		m_isExpired = true;
	m_position = m_position + m_velocity*elapsedTime;
}

void Entity::Render()
{
	glPushMatrix();
	glColor4f(1.f,1.f,1.f,m_opacity);
	glTranslatef(m_position.x,m_position.y,0.f);
	glPointSize(m_size);
	glEnable(GL_POINT_SMOOTH);
	glBegin(GL_POINTS);
	{
		glVertex2f(0.f,0.f);	
	}
	glEnd();
	glPopMatrix();
}

