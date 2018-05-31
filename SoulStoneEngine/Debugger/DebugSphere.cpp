#include "DebugSphere.hpp"

void DebugSphere::Render()
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

void DebugSphere::RenderDepthTestOn()
{
	g_glRender->PushMatrix();
	g_glRender->Enable( GL_DEPTH_TEST );
	g_glRender->Color4f( m_color.m_red, m_color.m_green, m_color.m_blue, m_color.m_alpha);
	g_glRender->Translatef( m_center.x, m_center.y, m_center.z );
	g_glRender->Scalef( m_radius, m_radius, m_radius );
	g_glRender->PolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	g_glRender->LineWidth( 3.f );
	DrawSphere( 1.f, 20, 20);
	g_glRender->PopMatrix();
	g_glRender->PolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	g_glRender->LineWidth( 1.f );
}

void DebugSphere::RenderDepthTestOff()
{
	g_glRender->PushMatrix();
	g_glRender->Disable( GL_DEPTH_TEST );
	g_glRender->Color4f( m_color.m_red * 0.8f, m_color.m_green * 0.8f, m_color.m_blue * 0.8f, m_color.m_alpha);
	g_glRender->Translatef( m_center.x, m_center.y, m_center.z );
	g_glRender->Scalef( m_radius, m_radius, m_radius );
	g_glRender->PolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	DrawSphere( 1.f, 20, 20);
	g_glRender->PopMatrix();
	g_glRender->PolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	g_glRender->Enable( GL_DEPTH_TEST );
}

void DebugSphere::RenderDualMode()
{
	RenderDepthTestOff();
	RenderDepthTestOn();
}

void DebugSphere::DrawSphere(float, int lats, int longs)
{
	int i, j;
    for(i = 0; i <= lats; i++) 
	{
	    double lat0 = 3.14f * (-0.5 + (double) (i - 1) / lats);
		double z0  = sin(lat0);
		double zr0 =  cos(lat0);

		double lat1 = 3.14f * (-0.5 + (double) i / lats);
		double z1 = sin(lat1);
		double zr1 = cos(lat1);

		g_glRender->BeginDraw( GL_QUAD_STRIP );
		for(j = 0; j <= longs; j++) 
		{
			double lng = 2 * 3.14f * (double) (j - 1) / longs;
			double x = cos(lng);
			double y = sin(lng);

			glNormal3d(x * zr0, y * zr0, z0);
			glVertex3d(x * zr0, y * zr0, z0);
			glNormal3d(x * zr1, y * zr1, z1);
			glVertex3d(x * zr1, y * zr1, z1);
		}
		g_glRender->EndDraw();
	}
}