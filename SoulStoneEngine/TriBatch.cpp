#include "TriBatch.hpp"
#include "./Utilities/World.hpp"

TriBatch::TriBatch()
{
	m_vboID = 0;
	m_isVBODirty = true;
}

void TriBatch::CreateVBO()
{
	if( m_vboID == 0 )
	{
		g_glRender->GenerateBuffer( 1, &m_vboID );
	}

	g_glRender->BindBuffer( GL_ARRAY_BUFFER, m_vboID);
	g_glRender->BufferData( GL_ARRAY_BUFFER, sizeof( Vertex3D ) * m_vertexInLocalSpaceList.size(), m_vertexInLocalSpaceList.data(), GL_STATIC_DRAW );

	m_numVertex = m_vertexInLocalSpaceList.size();
	m_vertexInLocalSpaceList.clear();
	vector<Vertex3D>().swap( m_vertexInLocalSpaceList );
	m_isVBODirty = false;

// 	g_glRender->BindBuffer( GL_ARRAY_BUFFER, m_vboID);
// 	g_glRender->BufferData( GL_ARRAY_BUFFER, sizeof( Vertex3D ) * m_vertexInWorldSpaceList.size(), m_vertexInWorldSpaceList.data(), GL_STATIC_DRAW );
// 	g_glRender->BindBuffer( GL_ARRAY_BUFFER, 0 );
// 	m_numVertex = m_vertexInWorldSpaceList.size();
// 	m_vertexInWorldSpaceList.clear();
// 	vector<Vertex3D>().swap( m_vertexInWorldSpaceList );
// 	m_isVBODirty = false;

}

void TriBatch::Render()
{
	m_material->ActivateMaterial();
	m_material->m_worldToScreenMatrix = g_matrixStack.StackTop();
	m_material->m_localToWorldMatrix = m_worldTM->m_matrix;
	m_material->m_lightWorldPosition = Vector3( 0.f,0.f,2.5f );
	m_material->m_time = 1.f;
	m_material->m_deltaTime = 1.f;
	m_material->m_cameraWorldPosition = g_camera3D.m_cameraPosition;
	m_material->m_fogColor = RGBColor( 1.f,1.f,1.f,0.f );
	m_material->m_fogStartDistance = 2.f;
	m_material->m_fogEndDistance = 5.f;
	m_material->m_scaleBias = Vector2(0.04f,0.03f);
	for( int i = 0; i < 100; i++ )
	{
		m_material->m_boneTM[i] = m_boneTM->m_matrix;
	}
	m_material->SetUniformValues();
	m_material->BindTexture();

	if( m_isVBODirty )
	{
		//TransformToWorldSpace();
		CreateVBO();
	}

	g_glRender->Enable( GL_TEXTURE_2D );
	g_glRender->Enable( GL_DEPTH_TEST );
	g_glRender->Enable(GL_CULL_FACE);
	glEnableVertexAttribArray( VERTEX_ATTRIB_POSITIONS );
	glEnableVertexAttribArray( VERTEX_ATTRIB_COLORS );
	glEnableVertexAttribArray( VERTEX_ATTRIB_TEXCOORDS );
	glEnableVertexAttribArray( VERTEX_ATTRIB_NORMALS );
	glEnableVertexAttribArray( VERTEX_ATTRIB_TANGENT );
	glEnableVertexAttribArray( VERTEX_ATTRIB_BITANGENT );
	glEnableVertexAttribArray( VERTEX_ATTRIB_BONE_INDEX );
	glEnableVertexAttribArray( VERTEX_ATTRIB_BONE_WEIGHT );
	glEnableVertexAttribArray( VERTEX_ATTRIB_IS_STATIC );

	g_glRender->BindBuffer( GL_ARRAY_BUFFER, m_vboID );

	glVertexAttribPointer( VERTEX_ATTRIB_POSITIONS, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex3D ), (const GLvoid*) offsetof(Vertex3D,m_position) );
	glVertexAttribPointer( VERTEX_ATTRIB_COLORS, 4, GL_FLOAT, GL_FALSE, sizeof( Vertex3D ), (const GLvoid*) offsetof(Vertex3D,m_color) );
	glVertexAttribPointer( VERTEX_ATTRIB_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof( Vertex3D ), (const GLvoid*) offsetof(Vertex3D,m_texCoords) );
	glVertexAttribPointer( VERTEX_ATTRIB_NORMALS, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex3D ), (const GLvoid*) offsetof(Vertex3D,m_normal) );
	glVertexAttribPointer( VERTEX_ATTRIB_TANGENT, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex3D ), (const GLvoid*) offsetof(Vertex3D,m_tangent) );
	glVertexAttribPointer( VERTEX_ATTRIB_BITANGENT, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex3D ), (const GLvoid*) offsetof(Vertex3D,m_bitangent) );
	glVertexAttribPointer( VERTEX_ATTRIB_BONE_INDEX, 4, GL_FLOAT, GL_FALSE, sizeof( Vertex3D ), (const GLvoid*) (offsetof(Vertex3D,m_boneIndex) ) );
	glVertexAttribPointer( VERTEX_ATTRIB_BONE_WEIGHT, 4, GL_FLOAT, GL_FALSE, sizeof( Vertex3D ), (const GLvoid*) (offsetof(Vertex3D,m_boneWeight ) ) );
	glVertexAttribPointer( VERTEX_ATTRIB_IS_STATIC, 1, GL_FLOAT, GL_FALSE, sizeof( Vertex3D ), (const GLvoid*) (offsetof(Vertex3D,m_isStatic ) ) );

	g_glRender->DrawArray( GL_TRIANGLES ,0, m_numVertex );

	glDisableVertexAttribArray( VERTEX_ATTRIB_POSITIONS );
	glDisableVertexAttribArray( VERTEX_ATTRIB_COLORS );
	glDisableVertexAttribArray( VERTEX_ATTRIB_TEXCOORDS );
	glDisableVertexAttribArray( VERTEX_ATTRIB_NORMALS );
	glDisableVertexAttribArray( VERTEX_ATTRIB_TANGENT );
	glDisableVertexAttribArray( VERTEX_ATTRIB_BITANGENT );
	glDisableVertexAttribArray( VERTEX_ATTRIB_BONE_INDEX );
	glDisableVertexAttribArray( VERTEX_ATTRIB_BONE_WEIGHT );
	glDisableVertexAttribArray( VERTEX_ATTRIB_IS_STATIC );

	g_glRender->BindBuffer( GL_ARRAY_BUFFER, 0 );
	g_glRender->Disable( GL_TEXTURE_2D );
	g_glRender->Disable( GL_DEPTH_TEST );

	m_material->DisableMaterial();
}

void TriBatch::SetMaterial(const std::string& materialName)
{
	std::set<Material*>::iterator iter;
	for( iter = g_glRender->s_materialRegisteredList.begin(); iter != g_glRender->s_materialRegisteredList.end(); ++iter )
	{
		Material* mat = *iter;
		if( mat->m_materialName.compare( materialName ) == 0 )
		{
			m_material = mat;
		}
	}
}

void TriBatch::TransformToWorldSpace()
{
	m_vertexInWorldSpaceList.reserve( m_vertexInLocalSpaceList.size() );

	m_vertexInWorldSpaceList.clear();
	for( unsigned int i = 0; i < m_vertexInLocalSpaceList.size(); i++ )
	{
		Vertex3D vertex = m_vertexInLocalSpaceList[i];
		vertex.m_position = m_worldTM->TransformPoint( vertex.m_position );
		vertex.m_normal = m_worldTMNoTranslation->TransformDirection( vertex.m_normal );
		vertex.m_normal = vertex.m_normal.Normalize();
		vertex.m_tangent = m_worldTMNoTranslation->TransformDirection( vertex.m_tangent );
		vertex.m_bitangent = m_worldTMNoTranslation->TransformDirection( vertex.m_bitangent );
		m_vertexInWorldSpaceList.push_back( vertex );
		int k = 1;
	}
}
