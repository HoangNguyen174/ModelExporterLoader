#ifndef EXPORTER_LOADER_H
#define EXPORTER_LOADER_H
#include "./Utilities/GameCommon.hpp"
#include "Mesh.hpp"
#include "./Render/OpenGLShaderProgram.hpp"
#include "BaseObject.hpp"

class ExporterLoader
{		
	public:
		ExporterLoader() {};
		void ReconstructHierachyStructure( const std::vector<BaseObject*>& objectList, std::vector<BaseObject*>& hierachyObjectList );
		void LoadAndReconstructSceneFromExportFile( const std::string& fileName, OpenGLShaderProgram* shader, OpenGLShaderProgram* staticShader, std::vector<BaseObject*>& objectList, std::vector<BaseObject*>& skeletonList, std::vector<BaseObject*>& boneList );	
};

#endif