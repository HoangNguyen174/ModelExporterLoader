#include "Mesh.hpp"

void Mesh::Render()
{
	for( unsigned int i = 0; i < m_triBatList.size(); i++ )
	{
		m_triBatList[i]->m_worldTM = m_toWorldTransformation;
		m_triBatList[i]->m_worldTMNoTranslation = m_toWorldTransformationNoTranslation;
		for( int j = 0; j < 100; j++ )
		{
			m_triBatList[i]->m_boneTM[j] = m_boneTM[j]; 
		}
		m_triBatList[i]->Render();
	}
}