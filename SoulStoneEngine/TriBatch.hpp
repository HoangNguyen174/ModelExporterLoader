#ifndef TRI_BATCH_H
#define TRI_BATCH_H
#include "SimpleMaterial.hpp"
#include "./Render/GLRender.hpp"
#include "./Utilities/GameCommon.hpp"
#include "./Render/Material.hpp"
#include "./Utilities/Matrix44.hpp"

class TriBatch
{
	public:
		Material*				m_material;
		GLuint					m_vboID;
		std::vector<Vertex3D>	m_vertexInLocalSpaceList;
		std::vector<Vertex3D>	m_vertexInWorldSpaceList;
		bool					m_isVBODirty;
		int						m_numVertex;
		Matrix44*				m_worldTM;
		Matrix44*				m_worldTMNoTranslation;
		Matrix44				m_boneTM[100];
		
	public:
		TriBatch();
		void TransformToWorldSpace();
		void SetMaterial( const std::string& m_materialName );
		void Render();

	private:
		void CreateVBO();

};
#endif