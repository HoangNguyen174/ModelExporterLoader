#include "Light.hpp"

Light::Light(LightType type, Vector3 position, Vector3 direction, RGBColor color)
{
	m_lightType = type;
	m_position = position;
	m_forwardDirection = direction;
	m_colorAndBrightness = color;
	m_ambientness = 0.f;
}


Light* Light::CreateInfinitePointLight(const Vector3& position, const Vector3& direction, const RGBColor& color)
{
	Light* temp = new Light(INFINITE_POINT_LIGHT, position, direction, color);
	temp->m_innerRadius = VERY_FAR;
	temp->m_outerRadius = VERY_FAR + 1.f;
	temp->m_innerApertureDot = NO_APERTURE;
	temp->m_outerApertureDot = NO_APERTURE - 1;
	temp->m_isDirectionalLight = 0;
	return temp;
}

Light* Light::CreateAttenuatedPointLight(const Vector3& position, const Vector3& direction, const RGBColor& color, float innerRadius, float outerRadius )
{
	Light* temp = new Light(ATTENUATED_POINT_LIGHT, position, direction, color);
	temp->m_innerRadius = innerRadius;
	temp->m_outerRadius = outerRadius;
	temp->m_innerApertureDot = NO_APERTURE;
	temp->m_outerApertureDot = NO_APERTURE - 1;
	temp->m_isDirectionalLight = 0;
	return temp;
}

Light* Light::CreateInfiniteSpotLight( const Vector3& position, const Vector3& direction, const RGBColor& color, float m_innerApertureDeg, float m_outerApertureDeg )
{
	Light* temp = new Light(INFINITE_SPOT_LIGHT, position, direction, color);
	temp->m_innerRadius = VERY_FAR;
	temp->m_outerRadius = VERY_FAR + 1.f;
	temp->m_innerApertureDot = cos( MathUtilities::DegToRad(m_innerApertureDeg) );
	temp->m_outerApertureDot = cos( MathUtilities::DegToRad(m_outerApertureDeg) );
	temp->m_isDirectionalLight = 0;
	return temp;
}

Light* Light::CreateAttenuatedSpotLight( const Vector3& position, const Vector3& direction, const RGBColor& color, float innerRadius, float outerRadius, float m_innerApertureDeg, float m_outerApertureDeg )
{
	Light* temp = new Light(ATTENUATED_SPOT_LIGHT, position, direction, color);
	temp->m_innerRadius = innerRadius;
	temp->m_outerRadius = outerRadius;
	temp->m_innerApertureDot = cos( MathUtilities::DegToRad(m_innerApertureDeg) );
	temp->m_outerApertureDot = cos( MathUtilities::DegToRad(m_outerApertureDeg) );
	temp->m_isDirectionalLight = 0;
	return temp;
}

Light* Light::CreateLocalAmbientLight( const Vector3& position, const Vector3& direction, const RGBColor& color,float ambientNess)
{
	Light* temp = new Light(ATTENUATED_SPOT_LIGHT, position, direction, color);
	temp->m_innerRadius = VERY_FAR;
	temp->m_outerRadius = VERY_FAR + 1.f;
	temp->m_innerApertureDot = NO_APERTURE;
	temp->m_outerApertureDot = NO_APERTURE - 1;
	temp->m_ambientness = ambientNess;
	temp->m_isDirectionalLight = 1;
	return temp;
}