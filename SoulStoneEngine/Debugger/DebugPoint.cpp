#include "DebugPoint.hpp"
#include "../Utilities/GameCommon.hpp"

void DebugPoint::Render()
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

void DebugPoint::RenderDepthTestOn()
{
	g_glRender->PushMatrix();
	g_glRender->Enable(GL_DEPTH_TEST);
	g_glRender->Disable(GL_TEXTURE_2D);
	g_glRender->Color4f(m_color.m_red ,m_color.m_green ,m_color.m_blue ,m_color.m_alpha);
	g_glRender->LineWidth(3.f);
	g_glRender->BeginDraw(GL_LINES);
	{
		g_glRender->Vertex3f( m_position.x - m_size,  m_position.y + m_size, m_position.z + m_size);
		g_glRender->Vertex3f( m_position.x + m_size,  m_position.y - m_size, m_position.z - m_size);

		g_glRender->Vertex3f( m_position.x + m_size,  m_position.y + m_size, m_position.z + m_size);
		g_glRender->Vertex3f( m_position.x - m_size,  m_position.y - m_size, m_position.z - m_size);

		g_glRender->Vertex3f( m_position.x - m_size,  m_position.y - m_size, m_position.z + m_size);
		g_glRender->Vertex3f( m_position.x + m_size,  m_position.y + m_size, m_position.z - m_size);

		g_glRender->Vertex3f( m_position.x + m_size,  m_position.y - m_size, m_position.z + m_size);
		g_glRender->Vertex3f( m_position.x - m_size,  m_position.y + m_size, m_position.z - m_size);

	}
	g_glRender->EndDraw();
	g_glRender->Enable(GL_TEXTURE_2D);
	g_glRender->PopMatrix();
}

void DebugPoint::RenderDepthTestOff()
{
	g_glRender->PushMatrix();
	g_glRender->Disable(GL_DEPTH_TEST);
	g_glRender->Color4f(m_color.m_red * 0.5f,m_color.m_green * 0.5f ,m_color.m_blue * 0.5f,m_color.m_alpha);
	g_glRender->BeginDraw(GL_LINES);
	{
		g_glRender->Vertex3f( m_position.x - m_size,  m_position.y + m_size, m_position.z + m_size);
		g_glRender->Vertex3f( m_position.x + m_size,  m_position.y - m_size, m_position.z - m_size);
	
		g_glRender->Vertex3f( m_position.x + m_size,  m_position.y + m_size, m_position.z + m_size);
		g_glRender->Vertex3f( m_position.x - m_size,  m_position.y - m_size, m_position.z - m_size);

		g_glRender->Vertex3f( m_position.x - m_size,  m_position.y - m_size, m_position.z + m_size);
		g_glRender->Vertex3f( m_position.x + m_size,  m_position.y + m_size, m_position.z - m_size);

		g_glRender->Vertex3f( m_position.x + m_size,  m_position.y - m_size, m_position.z + m_size);
		g_glRender->Vertex3f( m_position.x - m_size,  m_position.y + m_size, m_position.z - m_size);

	}
	g_glRender->EndDraw();
	g_glRender->Enable(GL_DEPTH_TEST);
	g_glRender->PopMatrix();
}

void DebugPoint::RenderDualMode()
{
	g_glRender->PushMatrix();
	g_glRender->Disable(GL_DEPTH_TEST);
	g_glRender->Color4f(m_color.m_red * 0.5f,m_color.m_green * 0.5f ,m_color.m_blue * 0.5f,m_color.m_alpha);
	g_glRender->BeginDraw(GL_LINES);
	{
		g_glRender->Vertex3f( m_position.x - m_size,  m_position.y + m_size, m_position.z + m_size);
		g_glRender->Vertex3f( m_position.x + m_size,  m_position.y - m_size, m_position.z - m_size);

		g_glRender->Vertex3f( m_position.x + m_size,  m_position.y + m_size, m_position.z + m_size);
		g_glRender->Vertex3f( m_position.x - m_size,  m_position.y - m_size, m_position.z - m_size);

		g_glRender->Vertex3f( m_position.x - m_size,  m_position.y - m_size, m_position.z + m_size);
		g_glRender->Vertex3f( m_position.x + m_size,  m_position.y + m_size, m_position.z - m_size);

		g_glRender->Vertex3f( m_position.x + m_size,  m_position.y - m_size, m_position.z + m_size);
		g_glRender->Vertex3f( m_position.x - m_size,  m_position.y + m_size, m_position.z - m_size);

	}
	g_glRender->EndDraw();

	g_glRender->Enable(GL_DEPTH_TEST);

	g_glRender->Color4f(m_color.m_red,m_color.m_green,m_color.m_blue,m_color.m_alpha);
	g_glRender->LineWidth(3.f);
	g_glRender->BeginDraw(GL_LINES);
	{
		g_glRender->Vertex3f( m_position.x - m_size,  m_position.y + m_size, m_position.z + m_size);
		g_glRender->Vertex3f( m_position.x + m_size,  m_position.y - m_size, m_position.z - m_size);

		g_glRender->Vertex3f( m_position.x + m_size,  m_position.y + m_size, m_position.z + m_size);
		g_glRender->Vertex3f( m_position.x - m_size,  m_position.y - m_size, m_position.z - m_size);

		g_glRender->Vertex3f( m_position.x - m_size,  m_position.y - m_size, m_position.z + m_size);
		g_glRender->Vertex3f( m_position.x + m_size,  m_position.y + m_size, m_position.z - m_size);

		g_glRender->Vertex3f( m_position.x + m_size,  m_position.y - m_size, m_position.z + m_size);
		g_glRender->Vertex3f( m_position.x - m_size,  m_position.y + m_size, m_position.z - m_size);

	}
	g_glRender->EndDraw();
	g_glRender->PopMatrix();
	glLineWidth(1.f);
}