#include "Camera3D.hpp"

Camera3D::Camera3D()
{
	m_cameraPosition = Vector3(0.f,0.f,0.f);
	m_pitchDegreesAboutY = 0.f;
	m_yawDegreesAboutZ = 0.f;
	m_rollDegreesAboutX = 0.f;
}

Camera3D::~Camera3D()
{

}