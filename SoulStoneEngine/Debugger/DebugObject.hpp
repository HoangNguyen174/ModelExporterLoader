#ifndef DEBUG_OBJECT_H
#define DEBUG_OBJECT_H

//#include "GameCommon.hpp"

enum DrawMode { DepthTestOn, DepthTestOff, Dual };

class DebugObject
{
	public:
		float m_lifeSpanInSec;
		DrawMode m_drawMode;

	public:
		DebugObject() 
		{ 
			m_lifeSpanInSec = 0.f;
			m_drawMode = Dual;
		};

		~DebugObject(){};
		void Update(float elapsedTime) 
		{
			if( m_lifeSpanInSec > 0.f )
				m_lifeSpanInSec -= elapsedTime;
		};
		virtual void Render() {};
		virtual void RenderDepthTestOn() {};
		virtual void RenderDepthTestOff() {};
		virtual void RenderDualMode() {};
};


#endif