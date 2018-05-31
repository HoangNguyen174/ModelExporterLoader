#ifndef RGBCOLOR_H
#define RGBCOLOR_H

#include "MathUtilities.hpp"

class RGBColor
{
	public: 
		float m_red;
		float m_green;
		float m_blue;
		float m_alpha;

	public:
		RGBColor()
		{
			m_red = 0.f;
			m_green = 0.f;
			m_blue = 0.f;
			m_alpha = 0.f;
		}

		RGBColor(float red, float green, float blue, float alpha)
		{
			m_red =  MathUtilities::Clamp(0.f,1.f,red);
			m_green = MathUtilities::Clamp(0.f,1.f,green);
			m_blue = MathUtilities::Clamp(0.f,1.f,blue);
			m_alpha = MathUtilities::Clamp(0.f,1.f,alpha);
		}

		~RGBColor() {}

		void SetColor(float red,float green, float blue, float alpha)
		{
			m_red =  MathUtilities::Clamp(0.f,1.f,red);
			m_green = MathUtilities::Clamp(0.f,1.f,green);
			m_blue = MathUtilities::Clamp(0.f,1.f,blue);
			m_alpha = MathUtilities::Clamp(0.f,1.f,alpha);
		}

		RGBColor operator+( const RGBColor& other)
		{
			RGBColor temp;
			temp.m_red = MathUtilities::Clamp( 0.f,1.f, m_red + other.m_red );
			temp.m_green = MathUtilities::Clamp( 0.f,1.f, m_green + other.m_green );
			temp.m_blue = MathUtilities::Clamp( 0.f,1.f, m_blue + other.m_blue );
			temp.m_alpha =MathUtilities::Clamp( 0.f,1.f, m_alpha + other.m_alpha ); 
			return temp;
		}

		static RGBColor White()
		{
			return RGBColor(1.f,1.f,1.f,1.f);
		}

		static RGBColor Black()
		{
			return RGBColor(0.f,0.f,0.f,1.f);
		}

		RGBColor operator*( const float scale )
		{
			RGBColor temp;
			temp.m_red = MathUtilities::Clamp( 0.f,1.f, m_red * scale );
			temp.m_green = MathUtilities::Clamp( 0.f,1.f, m_green * scale );
			temp.m_blue = MathUtilities::Clamp( 0.f,1.f, m_blue * scale );
			temp.m_alpha =MathUtilities::Clamp( 0.f,1.f, m_alpha * scale ); 
			return temp;
		}

		friend RGBColor operator*( const float scale, const RGBColor& color )
		{
			RGBColor temp;
			temp.m_red = MathUtilities::Clamp( 0.f,1.f, color.m_red * scale );
			temp.m_green = MathUtilities::Clamp( 0.f,1.f, color.m_green * scale );
			temp.m_blue = MathUtilities::Clamp( 0.f,1.f, color.m_blue * scale );
			temp.m_alpha =MathUtilities::Clamp( 0.f,1.f, color.m_alpha * scale ); 
			return temp;
		}
};

#endif