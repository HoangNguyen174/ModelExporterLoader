#include "DebugLine.hpp"

void DebugLine::Render()
{
	switch(m_drawMode)
	{
		case DepthTestOn : RenderDepthTestOn();
			break;
		case DepthTestOff : RenderDepthTestOff();
			break;
		case Dual: RenderDualMode();
			break;
	}
}

void DebugLine::RenderDepthTestOn()
{
	g_glRender->Enable(GL_DEPTH_TEST);
	g_glRender->PushMatrix();
	g_glRender->LineWidth(m_size * 3.f);
	g_glRender->BeginDraw(GL_LINES);
	{
		g_glRender->Color4f( m_startPosColor.m_red, m_startPosColor.m_green, m_startPosColor.m_blue, m_startPosColor.m_alpha );
		g_glRender->Vertex3f( m_startPosition.x, m_startPosition.y, m_startPosition.z );

		g_glRender->Color4f( m_endPosColor.m_red, m_endPosColor.m_green, m_endPosColor.m_blue, m_endPosColor.m_alpha );
		g_glRender->Vertex3f( m_endPosition.x, m_endPosition.y, m_endPosition.z);
	}
	g_glRender->EndDraw();
	g_glRender->PopMatrix();
	g_glRender->LineWidth(1.f);
}

void DebugLine::RenderDepthTestOff()
{
	g_glRender->Disable(GL_DEPTH_TEST);
	g_glRender->PushMatrix();
	g_glRender->LineWidth(m_size);
	g_glRender->BeginDraw(GL_LINES);
	{
		g_glRender->Color4f( m_startPosColor.m_red, m_startPosColor.m_green, m_startPosColor.m_blue, m_startPosColor.m_alpha );
		g_glRender->Vertex3f( m_startPosition.x, m_startPosition.y, m_startPosition.z );

		g_glRender->Color4f( m_endPosColor.m_red, m_endPosColor.m_green, m_endPosColor.m_blue, m_endPosColor.m_alpha );
		g_glRender->Vertex3f( m_endPosition.x, m_endPosition.y, m_endPosition.z);
	}
	g_glRender->EndDraw();
	g_glRender->PopMatrix();
	g_glRender->LineWidth(1.f);
	g_glRender->Enable(GL_DEPTH_TEST);
}

void DebugLine::RenderDualMode()
{
	g_glRender->PushMatrix();

	g_glRender->Disable(GL_DEPTH_TEST);
	g_glRender->LineWidth(m_size);
	g_glRender->BeginDraw(GL_LINES);
	{
		g_glRender->Color4f( m_startPosColor.m_red, m_startPosColor.m_green, m_startPosColor.m_blue, m_startPosColor.m_alpha );
		g_glRender->Vertex3f( m_startPosition.x, m_startPosition.y, m_startPosition.z );

		g_glRender->Color4f( m_endPosColor.m_red, m_endPosColor.m_green, m_endPosColor.m_blue, m_endPosColor.m_alpha );
		g_glRender->Vertex3f( m_endPosition.x, m_endPosition.y, m_endPosition.z);

	}
	g_glRender->EndDraw();

	g_glRender->Enable(GL_DEPTH_TEST);

	g_glRender->LineWidth(m_size * 3.f);
	g_glRender->BeginDraw(GL_LINES);
	{
		g_glRender->Color4f( m_startPosColor.m_red, m_startPosColor.m_green, m_startPosColor.m_blue, m_startPosColor.m_alpha );
		g_glRender->Vertex3f( m_startPosition.x, m_startPosition.y, m_startPosition.z );

		g_glRender->Color4f( m_endPosColor.m_red, m_endPosColor.m_green, m_endPosColor.m_blue, m_endPosColor.m_alpha );
		g_glRender->Vertex3f( m_endPosition.x, m_endPosition.y, m_endPosition.z);
	}
	g_glRender->EndDraw();
	glLineWidth(1.f);
	g_glRender->PopMatrix();
}