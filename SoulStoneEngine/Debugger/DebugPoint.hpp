#ifndef DEBUG_POINT_H
#define DEBUG_POINT_H
#include "DebugObject.hpp"
#include "../Utilities/Vector3.h"
#include "../Utilities/RGBColor.hpp"

class DebugPoint : public DebugObject
{
	public:
		Vector3 m_position;
		float m_size;
		RGBColor m_color;
		
	public:
		DebugPoint() 
		{
			m_size = 0.01f;
		};
		~DebugPoint() {};
		void Update(float elapsedTime) { DebugObject::Update(elapsedTime); };
		void Render();
		void RenderDepthTestOn();
		void RenderDepthTestOff();
		void RenderDualMode();
};

#endif