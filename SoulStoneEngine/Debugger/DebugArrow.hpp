#ifndef DEBUG_ARROW_H
#define DEBUG_ARROW_H
#include "DebugObject.hpp"
#include "../Utilities/GameCommon.hpp"

class DebugArrow : public DebugObject
{
	public:
		Vector3 m_startPosition;
		Vector3 m_endPosition;
		float m_size;
		RGBColor m_startPosColor;
		RGBColor m_endPosColor;

	public:
		DebugArrow()
		{
			m_size = 1.f;
		};
		~DebugArrow() {};
		void Update(float elapsedTime) { DebugObject::Update(elapsedTime); };
		void Render();
		void RenderDepthTestOn();
		void RenderDepthTestOff();
		void RenderDualMode();
};

#endif