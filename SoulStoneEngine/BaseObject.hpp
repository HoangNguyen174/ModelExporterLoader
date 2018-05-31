#ifndef BASE_OBJECT_H
#define BASE_OBJECT_H
#include "./Utilities/GameCommon.hpp"
#include "./Render/OpenGLShaderProgram.hpp"
#include "Mesh.hpp"
#include <string>

class BaseObject
{
	public:
		BaseObject*						m_parent;
		std::vector<BaseObject*>		m_childList;
		std::vector<Matrix44>			m_animationTM;
		std::string						m_name;
		std::string						m_parentName;
		Matrix44						m_localToParentTranformation;
		Matrix44						m_toWorldTransformation;
		Mesh*							m_mesh;

	public:
		BaseObject();
		Matrix44 GetWorldTransformation();
		Matrix44 GetWorlTransformationForDirection();
		void Render();
};

#endif