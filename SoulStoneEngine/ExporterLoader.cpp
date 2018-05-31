#include "ExporterLoader.hpp"
#include "BaseObject.hpp"

void ExporterLoader::LoadAndReconstructSceneFromExportFile( const std::string& fileName, OpenGLShaderProgram* shader, OpenGLShaderProgram* staticShader, std::vector<BaseObject*>& hierachyObjectList, std::vector<BaseObject*>& skeletonList, std::vector<BaseObject*>& boneList )
{
	std::vector<Mesh*> meshList;
	std::vector<BaseObject*> tempObjectList;

	meshList.clear();
	tempObjectList.clear();
	boneList.clear();

	FILE* myfile = nullptr;
	
	fopen_s( &myfile, fileName.c_str(), "rb" );

	if( myfile == nullptr )
	{
		DebuggerPrintf( "Fail to load %s file.\n", fileName.c_str() );
		exit(0);
	}

	while( !feof(myfile) && !ferror(myfile))
	{
		int matNum = 0;
		fread( &matNum, sizeof(int), 1, myfile );

		if( matNum == 0)
		{
			Material* tempMat = new Material( "CheckerBoard" , shader ) ;
			tempMat->m_diffuseTexture = Texture::CreateOrGetTexture( "./Textures/default.png" );
			GLRender::s_materialRegisteredList.insert( tempMat );
			tempMat = nullptr;
			delete tempMat;
		}

		for( int matIndex = 0; matIndex < matNum; matIndex++ )
		{
			char matTag[4];
			fread( matTag, sizeof(char), 4, myfile );
			if( strcmp( matTag, "MAT") != 0 )
			{
				DebuggerPrintf( "Error: Expected MAT tag. Terminate Program!.\n");
				exit(0);
			}
			else
			{
				int sizeOfMatName = 0;
				fread( &sizeOfMatName, sizeof(int), 1, myfile );
				char* matName = new char[sizeOfMatName];
				fread( matName, sizeof(char), sizeOfMatName, myfile );
				std::string name = matName;
				Material* tempMat = new Material( name, shader ) ;
				int texCount = 0;
				fread( &texCount, sizeof(int), 1, myfile );
				if( texCount == 0 )
				{
					tempMat->m_diffuseTexture = Texture::CreateOrGetTexture( "./Textures/default.png" );
				}

				for( int texIndex = 0; texIndex < texCount; texIndex++ )
				{
					char texType[5];
					fread( texType, sizeof(char),5, myfile );
					int texNameSize = 0;
					fread( &texNameSize, sizeof(int), 1 , myfile );
					char* texName = new char[texNameSize];
					fread( texName, sizeof(char), texNameSize, myfile );
					std::string texStr = texName;
					std::string texNameStr;
					if( str::substr( texStr, -4 ) == ".dds" )
					{
						texStr = str::replace( ".dds", ".png", texStr );
					}
					
					texNameStr = texStr;
					if( strcmp(texType, "diff" ) == 0 )
					{
						tempMat->m_diffuseTexture = Texture::CreateOrGetTexture( "./Textures/" + texNameStr );
					}
					else if( strcmp( texType, "spec" ) == 0 )
					{
						tempMat->m_specularTexture = Texture::CreateOrGetTexture( "./Textures/" + texNameStr );
					}
					else if( strcmp( texType, "bump") == 0 )
					{
						tempMat->m_normalTexture = Texture::CreateOrGetTexture( "./Textures/" + texNameStr );
					}
				
					texName = nullptr;
 					delete texName;
				}
				GLRender::s_materialRegisteredList.insert( tempMat );
 				matName = nullptr;
 				delete matName;
				tempMat = nullptr;
				delete tempMat;
			}
		}// end read material

		// Read Skeletal animation
		char skeletonTag[9];
		int boneNumber = 0;
		int numMeshes = 0;

		fread( &numMeshes, sizeof(int), 1, myfile );
		fread( skeletonTag, sizeof(char),9, myfile );
		fread( &boneNumber, sizeof(int), 1, myfile );

		if( strcmp( skeletonTag, "SKELETON" ) != 0 )
		{
			DebuggerPrintf( "Expected SKELETON tag. Terminate Program.\n" );
			exit(0);
		}

		for( int boneIndex = 0; boneIndex < boneNumber; boneIndex++ )
		{
			BaseObject* bone = new BaseObject();
			int nameSize = 0;
			
			fread( &nameSize, sizeof(int),1, myfile );
			char* boneName = new char[nameSize];
			fread( boneName, nameSize, 1, myfile );

			bone->m_name = boneName;

			fread( &nameSize, sizeof(int),1, myfile );
			char* parentName = new char[nameSize];
			fread( parentName, nameSize, 1, myfile );

			bone->m_parentName = parentName;

			Matrix44 transformationMatrix;

			fread( transformationMatrix.m_matrix, sizeof(float), 16, myfile );
			bone->m_localToParentTranformation = transformationMatrix;

			int frameNum = 0;
			fread( &frameNum, sizeof(int),1, myfile );

			for( int frameIndex = 0; frameIndex < frameNum; frameIndex++ )
			{
				Matrix44 tm;

				fread( tm.m_matrix, sizeof(float), 16, myfile );

				bone->m_animationTM.push_back( tm );
			}
			boneList.push_back( bone );
		}
		// End read Skeletal Animation

		//import mesh
		for( int meshIndex = 0; meshIndex < numMeshes; meshIndex++ )
		{
			Mesh* tempMesh = new Mesh;
			char meshTag[5];
			fread( meshTag, sizeof(char), 5, myfile );

			int tribatNum = 0;
			fread( &tribatNum, sizeof(int), 1, myfile );
			if( strcmp( meshTag, "MESH" ) != 0 )
			{
				DebuggerPrintf( "Error: Expected MESH tag. Terminate program!.\n" );
				exit(0);
			}
		
			for( int triIndex = 0; triIndex < tribatNum; triIndex ++ )
			{
				char triTag[7];
				fread( triTag, sizeof(char), 7, myfile );
				if( strcmp( triTag, "TRIBAT" ) != 0 )
				{
					DebuggerPrintf( "Error: Expected TRIBAT tag. Terminate program!.\n" );
					exit(0);
				}
				TriBatch* tempTribat = new TriBatch;

				int matNameSize = 0;
				fread( &matNameSize, sizeof(int), 1, myfile );
				char* matName = new char[matNameSize];
				fread( matName, sizeof(char), matNameSize, myfile );
				std::string matStr = matName;

				if( matStr.compare( "NOMAT" ) == 0 )
				{
					matStr = "CheckerBoard";
				}
					
				tempTribat->SetMaterial( matStr );

				int vertexNum = 0;
				fread( &vertexNum, sizeof(int), 1, myfile );
				
				for( int vertIndex = 0; vertIndex < vertexNum; vertIndex++ )
				{
					Vector3 pos;
					Vector2 texCoords;
					Vector3 norm;
					Vector3 color;
					Vector3 tangent;
					Vector3 bitangent;

					fread( &pos.x, sizeof(float),3, myfile );
					fread( &color.x, sizeof(float),3, myfile );
					fread( &texCoords.x, sizeof(float),2, myfile );
					fread( &norm.x, sizeof(float),3, myfile );
					fread( &tangent.x, sizeof(float),3, myfile );
					fread( &bitangent.x, sizeof(float),3, myfile );

					Vertex3D tempVertex;
					tempVertex.m_position = pos;
					tempVertex.m_color = RGBColor( abs(color.x), abs(color.y), abs(color.z) , 1.0 );
					tempVertex.m_texCoords = Vector2( texCoords.x, 1.f - texCoords.y );
					tempVertex.m_normal = norm.Normalize();
					tempVertex.m_tangent = tangent.Normalize();
					tempVertex.m_bitangent = bitangent.Normalize();

					for( int i = 0; i < 4; i++ )
					{
						tempVertex.m_boneIndex[i] = 0;
						tempVertex.m_boneWeight[i] = 0.f;
					}
					int numBone = 0;
					fread( &numBone, sizeof(int),1, myfile );

					for( int i = 0; i < numBone; i++ )
					{
						int boneIndex = 0;
						fread( &boneIndex, sizeof(int),1,myfile );
						float boneWeight = 0.f;
						fread( &boneWeight, sizeof(float),1,myfile );
						tempVertex.m_boneIndex[i] = boneIndex;
						tempVertex.m_boneWeight[i] = boneWeight;
					}

					float isStatic = 1.f;
					for( int i = 0; i < 4; i++ )
					{
						if( tempVertex.m_boneWeight[i] != 0.f )
						{
							isStatic = 0.f;
							break;
						}
					}
					tempVertex.m_isStatic = isStatic;
					tempTribat->m_vertexInLocalSpaceList.push_back( tempVertex );
				}

				tempMesh->m_triBatList.push_back( tempTribat );

				tempTribat = nullptr;
				delete tempTribat;
			}
			meshList.push_back(tempMesh);
			tempMesh = nullptr;
			delete tempMesh;
		}// end read mesh

		//Read Node data
		int totalNumberOfNode = 0;
		fread( &totalNumberOfNode, sizeof(int),1, myfile );

		for( int i = 0; i < totalNumberOfNode; i++ )
		{
			BaseObject* object = new BaseObject();
			char nodeTag[5];
			fread( nodeTag, sizeof(char), 5, myfile );
			if( strcmp( nodeTag, "NODE") != 0 )
			{
				DebuggerPrintf( "Expected NODE tag. Terminate Program.\n" );
				exit(0);
			}
			int sizeOfNodeName = 0;
			fread( &sizeOfNodeName, sizeof(int), 1, myfile );
			char* nodeName = new char[sizeOfNodeName];
			fread( nodeName, sizeof(char), sizeOfNodeName, myfile );
			
			std::string nameStr = nodeName;

			object->m_name = nodeName;

			Matrix44 transformationMatrix;

			fread( transformationMatrix.m_matrix, sizeof(float), 16, myfile );

			object->m_localToParentTranformation = transformationMatrix;

			int sizeOfParentName = 0;
			fread( &sizeOfParentName, sizeof(int), 1, myfile );

			if( sizeOfParentName != -1 )
			{
				char* parentName = new char[sizeOfParentName];
				fread( parentName, sizeOfParentName, 1, myfile );

				object->m_parentName = parentName;

				int meshIndex = 0;
				fread( &meshIndex, sizeof(int),1, myfile );

				if( meshIndex != -1 )
					object->m_mesh = meshList[meshIndex];

				parentName = nullptr;
				delete parentName;
			}
			else
			{
				int meshIndex = 0;
				fread( &meshIndex, sizeof(int),1, myfile );

				if( meshIndex != -1 )
					object->m_mesh = meshList[meshIndex];
			}

			int frameNum = 0;
			fread( &frameNum , sizeof(int),1, myfile );

			for( int frameIndex = 0; frameIndex < frameNum; frameIndex++ )
			{
				Matrix44 tm;

				fread( tm.m_matrix, sizeof(float), 16, myfile );

				object->m_animationTM.push_back( tm );
			}

			tempObjectList.push_back(object);

			nodeName = nullptr;
			delete nodeName;
			object = nullptr;
			delete object;
		}
	}

	fclose( myfile );

	ReconstructHierachyStructure( tempObjectList, hierachyObjectList );

	ReconstructHierachyStructure( boneList, skeletonList );

}

void ExporterLoader::ReconstructHierachyStructure( const std::vector<BaseObject*>& objectList, std::vector<BaseObject*>& hierachyObjectList )
{
	for( unsigned int i = 0; i < objectList.size(); i++ )
	{
		BaseObject* object = objectList[i];

		//if node does not have parent -> it is root node
		if( object->m_parentName.size() == 0 || object->m_parentName.compare( "No Parent") == 0 )
			hierachyObjectList.push_back(object);
		else
		{
			for( unsigned j = 0; j < objectList.size(); j++ )
			{
				if( i == j )
					continue;

				BaseObject* checkObject = objectList[j];

				if( object->m_parentName.compare( checkObject->m_name ) == 0 )
				{
					object->m_parent = checkObject;
					checkObject->m_childList.push_back( object );
					break;
				}
			}
		}
	}
	int k = 0;
}
