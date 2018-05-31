#ifndef LIGHT_H
#define LIGHT_H
#include "../Utilities/Vector3.h"
#include "../Utilities/RGBColor.hpp"
#include "Material.hpp"

enum LightType { INFINITE_POINT_LIGHT, ATTENUATED_POINT_LIGHT, ATTENUATED_SPOT_LIGHT, INFINITE_SPOT_LIGHT, DIRECTIONAL_LIGHT, LOCAL_AMBIENT_LIGHT };
const float VERY_FAR = 1000000.0f;
const float	NO_APERTURE = -2.f;

class Light
{
	public:
		Vector3 m_position;
		RGBColor m_colorAndBrightness;
		float m_innerApertureDot;
		float m_outerApertureDot;
		float m_innerRadius;
		float m_outerRadius;
		Vector3 m_forwardDirection;
		LightType m_lightType;
		float m_ambientness;
		int m_isDirectionalLight;

	public:
		static Light* CreateInfinitePointLight( const Vector3& position, const Vector3& direction, const RGBColor& color );
		static Light* CreateAttenuatedPointLight( const Vector3& position, const Vector3& direction, const RGBColor& color, float innerRadius, float outerRadius );
		static Light* CreateAttenuatedSpotLight( const Vector3& position, const Vector3& direction, const RGBColor& color, float innerRadius, float outerRadius, float m_innerApertureDeg, float m_outerApertureDegs );
		static Light* CreateInfiniteSpotLight( const Vector3& position, const Vector3& direction, const RGBColor& color, float m_innerApertureDeg, float m_outerApertureDeg );
		static Light* CreateLocalAmbientLight( const Vector3& position, const Vector3& direction, const RGBColor& color, float ambientNess ); 

	private:
		Light(LightType type, Vector3 position, Vector3 direction, RGBColor color);
};


#endif;