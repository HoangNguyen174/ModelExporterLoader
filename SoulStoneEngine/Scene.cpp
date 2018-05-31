#include "Scene.hpp"

Scene::Scene(const std::string& filePath, OpenGLShaderProgram* shader, OpenGLShaderProgram* staticShader )
{
	m_loader.LoadAndReconstructSceneFromExportFile( filePath, shader , staticShader, m_rootObjectList, m_skeletonList, m_boneList );
	m_maxFrameNumber = 1;
}

void Scene::Render()
{
	for( unsigned int objectIndex = 0; objectIndex < m_rootObjectList.size(); objectIndex++ )
	{
		m_rootObjectList[objectIndex]->Render();
	}
}

void Scene::PlayAnimationManually( int frameIndex )
{
	std::vector<BaseObject*> nodeStack;
	
	for( unsigned int rootObjectIndex = 0; rootObjectIndex < m_rootObjectList.size(); rootObjectIndex++ )
	{
		nodeStack.clear();
		nodeStack.push_back(m_rootObjectList[rootObjectIndex]);
		while( nodeStack.size() != 0 )
		{
			BaseObject* object = nodeStack.back();
			if( object->m_animationTM.size() != 1 )
				m_maxFrameNumber = object->m_animationTM.size();

			nodeStack.pop_back();
			Matrix44 temp;
			if( object->m_mesh != nullptr )
			{
				Matrix44 parentTM;
				Matrix44 parentTMNoTranslate;

				int frame;
				if( object->m_parent != nullptr )
				{
					if( object->m_parent->m_animationTM.size() != 1 )
						frame = frameIndex;
					else
						frame = 0;

					parentTM = object->m_parent->m_animationTM[frame];
					parentTMNoTranslate = Matrix44::CancelTranslation( parentTM );	
				}

				if( object->m_animationTM.size() != 1 )
					frame = frameIndex;
				else
					frame = 0;

				int boneFrame = frameIndex;
				for( unsigned int i = 0; i < m_boneList.size(); i++ )
				{
					object->m_mesh->m_boneTM[i] = m_boneList[i]->m_animationTM[boneFrame];
				}
				object->m_mesh->m_toWorldTransformation = &( object->m_animationTM[frame] * parentTM );
				object->m_mesh->m_toWorldTransformationNoTranslation = &( Matrix44::CancelTranslation( object->m_animationTM[frame] ) * parentTMNoTranslate );
				object->m_mesh->Render();
			}

			for( unsigned int childIndex = 0; childIndex < object->m_childList.size(); childIndex++ )
			{
				nodeStack.push_back(object->m_childList[childIndex]);
			}
		}
	}
}

void Scene::CalculateBoneTM()
{
// 	for( unsigned int i = 0; i < m_boneList.size(); i++ )
// 	{
// 		std::string boneName = m_boneList[i]->m_name;
// 		BaseObject* bone = m_boneList[i];
// 		BaseObject* boneInSkeleton = 
// 	}
}

void Scene::PlaySkeletalAnimation(int frameIndex)
{

}

