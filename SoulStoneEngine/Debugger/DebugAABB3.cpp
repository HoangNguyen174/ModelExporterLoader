#include "DebugAABB3.hpp"

void DebugAABB3::Render()
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

void DebugAABB3::RenderDepthTestOn()
{
	g_glRender->PushMatrix();
	g_glRender->Enable( GL_DEPTH_TEST );

	//Draw Edges
	g_glRender->Color4f( m_edgeColor.m_red, m_edgeColor.m_green, m_edgeColor.m_blue, m_edgeColor.m_alpha );
	g_glRender->LineWidth( 3.f );
	g_glRender->PolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	g_glRender->BeginDraw( GL_QUADS );
	{
		g_glRender->Vertex3f( m_minPoint.x, m_minPoint.y, m_minPoint.z );
		g_glRender->Vertex3f( m_maxPoint.x, m_minPoint.y, m_minPoint.z );
		g_glRender->Vertex3f( m_maxPoint.x, m_minPoint.y, m_maxPoint.z );
		g_glRender->Vertex3f( m_minPoint.x, m_minPoint.y, m_maxPoint.z );

		g_glRender->Vertex3f( m_maxPoint.x, m_minPoint.y, m_minPoint.z );
		g_glRender->Vertex3f( m_maxPoint.x, m_maxPoint.y, m_minPoint.z );
		g_glRender->Vertex3f( m_maxPoint.x, m_maxPoint.y, m_maxPoint.z );
		g_glRender->Vertex3f( m_maxPoint.x, m_minPoint.y, m_maxPoint.z );

		g_glRender->Vertex3f( m_maxPoint.x, m_maxPoint.y, m_minPoint.z );
		g_glRender->Vertex3f( m_minPoint.x, m_maxPoint.y, m_minPoint.z );
		g_glRender->Vertex3f( m_minPoint.x, m_maxPoint.y, m_maxPoint.z );
		g_glRender->Vertex3f( m_maxPoint.x, m_maxPoint.y, m_maxPoint.z );

		g_glRender->Vertex3f( m_minPoint.x, m_maxPoint.y, m_minPoint.z );
		g_glRender->Vertex3f( m_minPoint.x, m_minPoint.y, m_minPoint.z );
		g_glRender->Vertex3f( m_minPoint.x, m_minPoint.y, m_maxPoint.z );
		g_glRender->Vertex3f( m_minPoint.x, m_maxPoint.y, m_maxPoint.z );
		
	}
	g_glRender->EndDraw();
	g_glRender->PolygonMode( GL_FRONT_AND_BACK, GL_FILL );

	//draw faces
	g_glRender->Color4f( m_faceColor.m_red, m_faceColor.m_green, m_faceColor.m_blue, m_faceColor.m_alpha );
	g_glRender->BeginDraw( GL_QUADS );
	{
		g_glRender->Vertex3f( m_minPoint.x, m_minPoint.y, m_minPoint.z );
		g_glRender->Vertex3f( m_maxPoint.x, m_minPoint.y, m_minPoint.z );
		g_glRender->Vertex3f( m_maxPoint.x, m_minPoint.y, m_maxPoint.z );
		g_glRender->Vertex3f( m_minPoint.x, m_minPoint.y, m_maxPoint.z );

		g_glRender->Vertex3f( m_maxPoint.x, m_minPoint.y, m_minPoint.z );
		g_glRender->Vertex3f( m_maxPoint.x, m_maxPoint.y, m_minPoint.z );
		g_glRender->Vertex3f( m_maxPoint.x, m_maxPoint.y, m_maxPoint.z );
		g_glRender->Vertex3f( m_maxPoint.x, m_minPoint.y, m_maxPoint.z );

		g_glRender->Vertex3f( m_maxPoint.x, m_maxPoint.y, m_minPoint.z );
		g_glRender->Vertex3f( m_minPoint.x, m_maxPoint.y, m_minPoint.z );
		g_glRender->Vertex3f( m_minPoint.x, m_maxPoint.y, m_maxPoint.z );
		g_glRender->Vertex3f( m_maxPoint.x, m_maxPoint.y, m_maxPoint.z );

		g_glRender->Vertex3f( m_minPoint.x, m_maxPoint.y, m_minPoint.z );
		g_glRender->Vertex3f( m_minPoint.x, m_minPoint.y, m_minPoint.z );
		g_glRender->Vertex3f( m_minPoint.x, m_minPoint.y, m_maxPoint.z );
		g_glRender->Vertex3f( m_minPoint.x, m_maxPoint.y, m_maxPoint.z );

		g_glRender->Vertex3f( m_minPoint.x, m_minPoint.y, m_minPoint.z );
		g_glRender->Vertex3f( m_minPoint.x, m_maxPoint.y, m_minPoint.z );
		g_glRender->Vertex3f( m_maxPoint.x, m_maxPoint.y, m_minPoint.z );
		g_glRender->Vertex3f( m_maxPoint.x, m_minPoint.y, m_minPoint.z );

		g_glRender->Vertex3f( m_minPoint.x, m_minPoint.y, m_maxPoint.z );
		g_glRender->Vertex3f( m_maxPoint.x, m_minPoint.y, m_maxPoint.z );
		g_glRender->Vertex3f( m_maxPoint.x, m_maxPoint.y, m_maxPoint.z );
		g_glRender->Vertex3f( m_minPoint.x, m_maxPoint.y, m_maxPoint.z );

	}
	g_glRender->EndDraw();
	g_glRender->LineWidth( 1.f );
	g_glRender->PopMatrix();
}

void DebugAABB3::RenderDepthTestOff()
{
	g_glRender->PushMatrix();

	//draw faces
	g_glRender->Disable( GL_DEPTH_TEST );
	g_glRender->Color4f( m_faceColor.m_red * 0.9f, m_faceColor.m_green * 0.9f, m_faceColor.m_blue * 0.9f, m_faceColor.m_alpha );
	g_glRender->BeginDraw( GL_QUADS );
	{
		g_glRender->Vertex3f( m_minPoint.x, m_minPoint.y, m_minPoint.z );
		g_glRender->Vertex3f( m_maxPoint.x, m_minPoint.y, m_minPoint.z );
		g_glRender->Vertex3f( m_maxPoint.x, m_minPoint.y, m_maxPoint.z );
		g_glRender->Vertex3f( m_minPoint.x, m_minPoint.y, m_maxPoint.z );

		g_glRender->Vertex3f( m_maxPoint.x, m_minPoint.y, m_minPoint.z );
		g_glRender->Vertex3f( m_maxPoint.x, m_maxPoint.y, m_minPoint.z );
		g_glRender->Vertex3f( m_maxPoint.x, m_maxPoint.y, m_maxPoint.z );
		g_glRender->Vertex3f( m_maxPoint.x, m_minPoint.y, m_maxPoint.z );

		g_glRender->Vertex3f( m_maxPoint.x, m_maxPoint.y, m_minPoint.z );
		g_glRender->Vertex3f( m_minPoint.x, m_maxPoint.y, m_minPoint.z );
		g_glRender->Vertex3f( m_minPoint.x, m_maxPoint.y, m_maxPoint.z );
		g_glRender->Vertex3f( m_maxPoint.x, m_maxPoint.y, m_maxPoint.z );

		g_glRender->Vertex3f( m_minPoint.x, m_maxPoint.y, m_minPoint.z );
		g_glRender->Vertex3f( m_minPoint.x, m_minPoint.y, m_minPoint.z );
		g_glRender->Vertex3f( m_minPoint.x, m_minPoint.y, m_maxPoint.z );
		g_glRender->Vertex3f( m_minPoint.x, m_maxPoint.y, m_maxPoint.z );

		g_glRender->Vertex3f( m_minPoint.x, m_minPoint.y, m_minPoint.z );
		g_glRender->Vertex3f( m_minPoint.x, m_maxPoint.y, m_minPoint.z );
		g_glRender->Vertex3f( m_maxPoint.x, m_maxPoint.y, m_minPoint.z );
		g_glRender->Vertex3f( m_maxPoint.x, m_minPoint.y, m_minPoint.z );

		g_glRender->Vertex3f( m_minPoint.x, m_minPoint.y, m_maxPoint.z );
		g_glRender->Vertex3f( m_maxPoint.x, m_minPoint.y, m_maxPoint.z );
		g_glRender->Vertex3f( m_maxPoint.x, m_maxPoint.y, m_maxPoint.z );
		g_glRender->Vertex3f( m_minPoint.x, m_maxPoint.y, m_maxPoint.z );

	}
	g_glRender->EndDraw();

	//Draw Edges
	g_glRender->Color4f( m_edgeColor.m_red * 0.9f, m_edgeColor.m_green * 0.9f, m_edgeColor.m_blue * 0.9f, m_edgeColor.m_alpha );
	g_glRender->LineWidth( 1.f );
	g_glRender->PolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	g_glRender->BeginDraw( GL_QUADS );
	{
		g_glRender->Vertex3f( m_minPoint.x, m_minPoint.y, m_minPoint.z );
		g_glRender->Vertex3f( m_maxPoint.x, m_minPoint.y, m_minPoint.z );
		g_glRender->Vertex3f( m_maxPoint.x, m_minPoint.y, m_maxPoint.z );
		g_glRender->Vertex3f( m_minPoint.x, m_minPoint.y, m_maxPoint.z );

		g_glRender->Vertex3f( m_maxPoint.x, m_minPoint.y, m_minPoint.z );
		g_glRender->Vertex3f( m_maxPoint.x, m_maxPoint.y, m_minPoint.z );
		g_glRender->Vertex3f( m_maxPoint.x, m_maxPoint.y, m_maxPoint.z );
		g_glRender->Vertex3f( m_maxPoint.x, m_minPoint.y, m_maxPoint.z );

		g_glRender->Vertex3f( m_maxPoint.x, m_maxPoint.y, m_minPoint.z );
		g_glRender->Vertex3f( m_minPoint.x, m_maxPoint.y, m_minPoint.z );
		g_glRender->Vertex3f( m_minPoint.x, m_maxPoint.y, m_maxPoint.z );
		g_glRender->Vertex3f( m_maxPoint.x, m_maxPoint.y, m_maxPoint.z );

		g_glRender->Vertex3f( m_minPoint.x, m_maxPoint.y, m_minPoint.z );
		g_glRender->Vertex3f( m_minPoint.x, m_minPoint.y, m_minPoint.z );
		g_glRender->Vertex3f( m_minPoint.x, m_minPoint.y, m_maxPoint.z );
		g_glRender->Vertex3f( m_minPoint.x, m_maxPoint.y, m_maxPoint.z );

	}
	g_glRender->EndDraw();
	g_glRender->PopMatrix();
	g_glRender->PolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	g_glRender->Enable( GL_DEPTH_TEST );
	g_glRender->LineWidth( 1.f );
}

void DebugAABB3::RenderDualMode()
{
	RenderDepthTestOff();
	RenderDepthTestOn();
}