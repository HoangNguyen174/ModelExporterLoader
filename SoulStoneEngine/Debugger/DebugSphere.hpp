#ifndef DEBUG_SPHERE_H
#define DEBUG_SPHERE_H
#include "../Utilities/GameCommon.hpp"
#include "DebugObject.hpp"

class DebugSphere : public DebugObject
{
	public:
		Vector3 m_center;
		float m_radius; 
		RGBColor m_color;

	public:
		DebugSphere() 
		{
			m_radius = 0.f;
		};
		~DebugSphere() {};
		void Update(float elapsedTime) { DebugObject::Update(elapsedTime); };
		void Render();
		void RenderDepthTestOn();
		void RenderDepthTestOff();
		void RenderDualMode();

	private:
		void DrawSphere( float r, int lats, int longs);
};


#endif