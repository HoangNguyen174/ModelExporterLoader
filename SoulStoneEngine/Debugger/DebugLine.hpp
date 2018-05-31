#ifndef DEBUG_LINE_H
#define DEBUG_LINE_H
#include "../Utilities/GameCommon.hpp"
#include "DebugObject.hpp"

class DebugLine : public DebugObject
{
	public:
		Vector3 m_startPosition;
		Vector3 m_endPosition;
		RGBColor m_startPosColor;
		RGBColor m_endPosColor;
		float m_size;

	public:
		DebugLine()
		{
			m_size = 1.f;
		};
		~DebugLine() {};
		void Update(float elapsedTime) { DebugObject::Update(elapsedTime); };
		void Render();
		void RenderDepthTestOn();
		void RenderDepthTestOff();
		void RenderDualMode();
};


#endif