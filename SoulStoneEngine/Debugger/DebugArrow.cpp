#include "DebugArrow.hpp"

void DebugArrow::Render()
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


void DebugArrow::RenderDepthTestOn()
{
	g_glRender->Enable(GL_DEPTH_TEST);
	g_glRender->PushMatrix();
	g_glRender->LineWidth(m_size * 3.f);
	g_glRender->BeginDraw(GL_LINES);
	{
		//draw body
		g_glRender->Color4f( m_startPosColor.m_red, m_startPosColor.m_green, m_startPosColor.m_blue, m_startPosColor.m_alpha );
		g_glRender->Vertex3f( m_startPosition.x, m_startPosition.y, m_startPosition.z );

		g_glRender->Color4f( m_endPosColor.m_red, m_endPosColor.m_green, m_endPosColor.m_blue, m_endPosColor.m_alpha );
		g_glRender->Vertex3f( m_endPosition.x, m_endPosition.y, m_endPosition.z );
	}
	g_glRender->EndDraw();
	g_glRender->PopMatrix();

	Vector3 v1 = m_endPosition - m_startPosition;

	Vector3 center = m_endPosition - ( 0.1f * v1 );
	//Vector3 v1 = m_startPosition - m_endPosition;
	v1.Normalize();

	Vector3 v2 = v1.CrossProduct( Vector3(1.f,0.f,0.f) );
	if( v2.CalcLength() < 0.001f )
	{
		v2 = v1.CrossProduct( Vector3(0.f,1.f,0.f) );
	}
	//v2.Normalize();

	Vector3 v3 = v1.CrossProduct(v2);
	//v3.Normalize();

	Vector3 h1 = center + 0.1f * v2;
	Vector3 h2 = center - 0.1f * v2;
	Vector3 h3 = center + 0.1f * v3;
	Vector3 h4 = center - 0.1f * v3;

	g_glRender->PushMatrix();
	//g_glRender->Translated(m_endPosition.x,m_endPosition.y,m_endPosition.z);
// 	glPointSize( 10.f );
// 	g_glRender->BeginDraw(GL_POINTS);
// 	{
// 		g_glRender->Color4f( m_endPosColor.m_red, m_endPosColor.m_green, m_endPosColor.m_blue, m_endPosColor.m_alpha );
// 		g_glRender->Vertex3f( center.x,center.y,center.z);
// 	}
// 	g_glRender->EndDraw();

	g_glRender->BeginDraw(GL_LINES);
	{
		//draw head
		g_glRender->Color4f( m_endPosColor.m_red, m_endPosColor.m_green, m_endPosColor.m_blue, m_endPosColor.m_alpha );
		g_glRender->Vertex3f( m_endPosition.x,m_endPosition.y,m_endPosition.z);
		g_glRender->Vertex3f( h1.x, h1.y, h1.z );

		g_glRender->Vertex3f( m_endPosition.x,m_endPosition.y,m_endPosition.z );
		g_glRender->Vertex3f( h2.x, h2.y, h2.z );

		g_glRender->Vertex3f( m_endPosition.x, m_endPosition.y, m_endPosition.z );
		g_glRender->Vertex3f( h3.x, h3.y, h3.z );

		g_glRender->Vertex3f( m_endPosition.x, m_endPosition.y, m_endPosition.z );
		g_glRender->Vertex3f( h4.x, h4.y, h4.z );
	}
	g_glRender->EndDraw();
	g_glRender->PopMatrix();

	g_glRender->LineWidth(1.f);
}

void DebugArrow::RenderDepthTestOff()
{
	g_glRender->Disable( GL_DEPTH_TEST );
	g_glRender->PushMatrix();
	g_glRender->LineWidth( 1.f );
	g_glRender->BeginDraw(GL_LINES);
	{
		//draw body
		g_glRender->Color4f( m_startPosColor.m_red * 0.7f, m_startPosColor.m_green * 0.7f, m_startPosColor.m_blue * 0.7f, m_startPosColor.m_alpha );
		g_glRender->Vertex3f( m_startPosition.x, m_startPosition.y, m_startPosition.z );

		g_glRender->Color4f( m_endPosColor.m_red * 0.7f, m_endPosColor.m_green * 0.7f, m_endPosColor.m_blue * 0.7f, m_endPosColor.m_alpha );
		g_glRender->Vertex3f( m_endPosition.x, m_endPosition.y, m_endPosition.z );
	}
	g_glRender->EndDraw();
	g_glRender->PopMatrix();

	Vector3 v1 = m_endPosition - m_startPosition;

	Vector3 center = m_endPosition - ( 0.1f * v1 );
	//Vector3 v1 = m_startPosition - m_endPosition;
	v1.Normalize();

	Vector3 v2 = v1.CrossProduct( Vector3(1.f,0.f,0.f) );
	if( v2.CalcLength() < 0.001f )
	{
		v2 = v1.CrossProduct( Vector3(0.f,1.f,0.f) );
	}
	//v2.Normalize();

	Vector3 v3 = v1.CrossProduct(v2);
	//v3.Normalize();

	Vector3 h1 = center + 0.1f * v2;
	Vector3 h2 = center - 0.1f * v2;
	Vector3 h3 = center + 0.1f * v3;
	Vector3 h4 = center - 0.1f * v3;

	g_glRender->PushMatrix();
	//g_glRender->Translated(m_endPosition.x,m_endPosition.y,m_endPosition.z);
	// 	glPointSize( 10.f );
	// 	g_glRender->BeginDraw(GL_POINTS);
	// 	{
	// 		g_glRender->Color4f( m_endPosColor.m_red, m_endPosColor.m_green, m_endPosColor.m_blue, m_endPosColor.m_alpha );
	// 		g_glRender->Vertex3f( center.x,center.y,center.z);
	// 	}
	// 	g_glRender->EndDraw();

	g_glRender->BeginDraw(GL_LINES);
	{
		//draw head
		g_glRender->Color4f( m_endPosColor.m_red * 0.7f, m_endPosColor.m_green * 0.7f, m_endPosColor.m_blue * 0.7f, m_endPosColor.m_alpha );
		g_glRender->Vertex3f( m_endPosition.x,m_endPosition.y,m_endPosition.z);
		g_glRender->Vertex3f( h1.x, h1.y, h1.z );

		g_glRender->Vertex3f( m_endPosition.x,m_endPosition.y,m_endPosition.z );
		g_glRender->Vertex3f( h2.x, h2.y, h2.z );

		g_glRender->Vertex3f( m_endPosition.x, m_endPosition.y, m_endPosition.z );
		g_glRender->Vertex3f( h3.x, h3.y, h3.z );

		g_glRender->Vertex3f( m_endPosition.x, m_endPosition.y, m_endPosition.z );
		g_glRender->Vertex3f( h4.x, h4.y, h4.z );
	}
	g_glRender->EndDraw();
	g_glRender->PopMatrix();

	g_glRender->LineWidth(1.f);
}

void DebugArrow::RenderDualMode()
{
	RenderDepthTestOff();
	RenderDepthTestOn();
}