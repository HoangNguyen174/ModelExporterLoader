#ifndef DEBUG_AABB3_H
#define DEBUG_AABB3_H
#include "../Utilities/GameCommon.hpp"
#include "DebugObject.hpp"

class DebugAABB3 : public DebugObject
{
	public:
		Vector3 m_minPoint;
		Vector3 m_maxPoint;
		RGBColor m_edgeColor;
		RGBColor m_faceColor;

	public:
		DebugAABB3() {};
		~DebugAABB3() {};
		void Update(float elapsedTime) { DebugObject::Update(elapsedTime); };
		void Render();
		void RenderDepthTestOn();
		void RenderDepthTestOff();
		void RenderDualMode();
};

#endif