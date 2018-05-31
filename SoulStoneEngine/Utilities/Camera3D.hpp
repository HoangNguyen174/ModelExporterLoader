#ifndef CAMERA3D_H
#define CAMERA3D_H
#include "Vector3.h"

class Camera3D
{
	public:
		Vector3 m_cameraPosition;
		float m_yawDegreesAboutZ;
		float m_pitchDegreesAboutY;
		float m_rollDegreesAboutX;

	public:
		Camera3D();
		~Camera3D();
};

#endif