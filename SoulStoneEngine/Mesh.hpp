#ifndef MESH_H
#define MESH_H
#include "./Utilities/GameCommon.hpp"
#include "TriBatch.hpp"
#include "./Utilities/Matrix44.hpp"

class Mesh
{
	public:
		std::vector<TriBatch*> m_triBatList;
		Matrix44*			   m_localToParentTransformation;
		Matrix44*			   m_toWorldTransformation;
		Matrix44*			   m_toWorldTransformationNoTranslation;
		Matrix44			   m_boneTM[100];

	public:
		Mesh() {};
		void Render();
};

#endif