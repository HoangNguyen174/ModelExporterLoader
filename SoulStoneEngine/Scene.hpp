#ifndef SCENE_H
#define SCENE_H
#include "./Utilities/GameCommon.hpp"
#include "BaseObject.hpp"
#include "ExporterLoader.hpp"

class Scene
{
	public:
		std::vector<BaseObject*>		m_rootObjectList;
		std::vector<BaseObject*>		m_skeletonList;
		std::vector<BaseObject*>		m_boneList;
		int								m_maxFrameNumber;

	private:
		ExporterLoader					m_loader;
	
	public:
		Scene( const std::string& filePath, OpenGLShaderProgram* shader, OpenGLShaderProgram* staticShader );
		BaseObject* GetObjectByName( const std::string& objectName );
		void CalculateBoneTM();
		void PlayAnimationManually(int frameIndex);
		void PlaySkeletalAnimation( int frameIndex );
		void Render();
};


#endif