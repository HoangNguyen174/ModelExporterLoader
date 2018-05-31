#include "stdafx.h"
#include "My3dsmaxExporter.h"

My3dsmaxExporter::My3dsmaxExporter() :
	m_igame( 0 ),
	m_outputFileHandle( 0 ),
	m_numMeshes ( 0 ),
	m_logFile( 0 )
{
}

My3dsmaxExporter::~My3dsmaxExporter()
{
	deinit();
}

void My3dsmaxExporter::deinit()
{
	if ( m_outputFileHandle != 0 )
	{
		fclose( m_outputFileHandle );
		m_outputFileHandle = 0;
	}

	if ( m_logFile != 0 )
	{
		fclose( m_logFile );
		m_logFile = 0;
	}

	if ( m_igame != 0 )
	{
		m_igame->ReleaseIGame();
		m_igame = 0;
	}
}

int My3dsmaxExporter::DoExport( const TCHAR* name, ExpInterface* ei, Interface* i, BOOL suppressprompts, DWORD options )
{
	try
	{
		const bool exportselected = 0 != (options & SCENE_EXPORT_SELECTED); // export only selected objects

		// initialize IGame
		m_igame = GetIGameInterface();
		if ( !m_igame )
			throw std::exception( "Failed to initialize IGame interface" );
		m_igame->InitialiseIGame( exportselected );
		
		// open file for writing
		std::wstring nameWstr = name;
		std::string nameStr = ConvertWStringToUTF8( nameWstr );
		
		std::string logFileName = nameStr + "Log";

		m_outputFileHandle = fopen( nameStr.c_str(), "wb" );
		m_logFile = fopen( logFileName.c_str(), "w" );

		// TODO: export materials, meshes, ...
		Export( m_igame );
		// release initialized stuff
		deinit();
	}
	catch ( std::exception& e )
	{
		TSTR tstr( (const wchar_t*)e.what() );
		MessageBox( i->GetMAXHWnd(), tstr, _T("Export Error"), MB_OK|MB_ICONERROR );
		deinit();
	}
	return TRUE;
}

void My3dsmaxExporter::Export( IGameScene* gameScene )
{
	std::vector<IGameNode*> nodeStack;
	m_sceneStartTime = gameScene->GetSceneStartTime();
	m_sceneEndTime = gameScene->GetSceneEndTime();

	for( int i = 0; i < m_igame->GetTopLevelNodeCount(); i++ )
	{
		nodeStack.clear();
		IGameNode* gameNode = m_igame->GetTopLevelNode(i);
		nodeStack.push_back(gameNode);

		while( nodeStack.size() != 0 )
		{
			IGameNode* node = nodeStack.back();
			IGameObject* gameObject = node->GetIGameObject();
			nodeStack.pop_back();

			if( gameObject->GetIGameType() == IGameObject::IGAME_MESH )
			{
				m_numMeshes++;
				IGameMesh* gameMesh = (IGameMesh*)gameObject;
				gameMesh->InitializeData();

				for( int i = 0; i < gameMesh->GetNumberOfFaces();i++ )
				{
					IGameMaterial* mat = gameMesh->GetMaterialFromFace(i);
					if( mat != nullptr )
						m_materials.insert( mat );
				}
			}

			for( unsigned int childNodeIndex = 0; childNodeIndex < node->GetChildCount(); childNodeIndex++ )
			{
				nodeStack.push_back( node->GetNodeChild(childNodeIndex) );
			}
		}
	}

	ExportMaterials();
	fwrite( &m_numMeshes, sizeof(int), 1, m_outputFileHandle );

	int meshCount = 0;
	nodeStack.clear();

	for( int i = 0; i < m_igame->GetTopLevelNodeCount(); i++ )
	{
		nodeStack.clear();
		IGameNode* gameNode = m_igame->GetTopLevelNode(i);
		nodeStack.push_back(gameNode);
		while( nodeStack.size() != 0 )
		{
			IGameNode* node = nodeStack.back();
			IGameObject* gameObject = node->GetIGameObject();
			nodeStack.pop_back();
			
			if( gameObject->GetIGameType() == IGameObject::IGAME_MESH )
			{
				IGameMesh* gameMesh = (IGameMesh*)gameObject;

				gameMesh->InitializeData();

				IGameSkin* gameSkin = nullptr;
				int modifierNum = gameObject->GetNumModifiers();
				for( int modiferIndex = 0; modiferIndex < modifierNum; modiferIndex++ )
				{
					IGameModifier* modifier = gameObject->GetIGameModifier(modiferIndex);
					if( modifier->IsSkin() )
						gameSkin = static_cast<IGameSkin*>( modifier );
				}
				GetListOfBone( gameSkin, node );
			}

			for( unsigned int childNodeIndex = 0; childNodeIndex < node->GetChildCount(); childNodeIndex++ )
			{
				nodeStack.push_back( node->GetNodeChild(childNodeIndex) );
			}
		}
	}

	ExportSkeletalAnimation();

	for( int i = 0; i < m_igame->GetTopLevelNodeCount(); i++ )
	{
		nodeStack.clear();
		IGameNode* gameNode = m_igame->GetTopLevelNode(i);
		nodeStack.push_back(gameNode);

		while( nodeStack.size() != 0 )
		{
			IGameNode* node = nodeStack.back();
			IGameObject* gameObject = node->GetIGameObject();
			nodeStack.pop_back();
			GMatrix worldToLocalMatrix;
			GMatrix worldToLocalMatrixNoTranslation;

			worldToLocalMatrix = node->GetWorldTM().Inverse();
			worldToLocalMatrixNoTranslation = worldToLocalMatrix;
			worldToLocalMatrixNoTranslation.SetRow( 3, Point4( 0.0, 0.0, 0.0, 1.0) );

			std::wstring nodeNameWstr = node->GetName();
			std::string nodeName = ConvertWStringToUTF8( nodeNameWstr );
		
			if( gameObject->GetIGameType() == IGameObject::IGAME_MESH )
			{
				IGameMesh* gameMesh = (IGameMesh*)gameObject;
				
				gameMesh->InitializeData();

				m_meshWithNodeNameList.push_back(nodeName);

				IGameSkin* gameSkin = nullptr;
				int modifierNum = gameObject->GetNumModifiers();
				for( int modiferIndex = 0; modiferIndex < modifierNum; modiferIndex++ )
				{
					IGameModifier* modifier = gameObject->GetIGameModifier(modiferIndex);
					if( modifier->IsSkin() )
						gameSkin = static_cast<IGameSkin*>( modifier );
				}

				ExportMeshes( gameMesh, meshCount, nodeName, worldToLocalMatrix, worldToLocalMatrixNoTranslation, gameSkin );

				meshCount += 1;
			}

			for( unsigned int childNodeIndex = 0; childNodeIndex < node->GetChildCount(); childNodeIndex++ )
			{
				nodeStack.push_back( node->GetNodeChild(childNodeIndex) );
			}
		}
		meshCount += 1;
	}

	int totalNode = m_igame->GetTotalNodeCount();
	fwrite( &totalNode, sizeof(int), 1, m_outputFileHandle );
	fprintf( m_logFile, "%i\n", totalNode );

	for( int i = 0; i < m_igame->GetTopLevelNodeCount(); i++ )
	{
		IGameNode* gameNode = m_igame->GetTopLevelNode(i);
		ExportNodeInfo(gameNode);
	}

}

void My3dsmaxExporter::ExportMaterials()
{
	char beginMatTag[] = "MAT";
	int numberOfBytesToWrite = 0;

	int matCount = m_materials.size();

	//write number of material
	fwrite( &matCount, sizeof(int), 1, m_outputFileHandle );
	fprintf( m_logFile, "Number of Material: %i\n", matCount );

	for( auto materialIter = m_materials.begin(); materialIter != m_materials.end(); ++materialIter )
	{
		IGameMaterial* mat = *materialIter;
		if( mat != nullptr )
		{
			// MAT
			fwrite( beginMatTag, sizeof(char), sizeof(beginMatTag), m_outputFileHandle );
			fprintf( m_logFile, "MAT\n", beginMatTag );

			std::wstring matNameWStr = mat->GetMaterialName();
			std::string matName = ConvertWStringToUTF8( matNameWStr );
			//numberOfBytesToWrite = sizeof( matName.c_str() );
			numberOfBytesToWrite = matName.size() + 1;
			
			// ex: 12Default - 01
			fwrite( &numberOfBytesToWrite, sizeof(int), 1, m_outputFileHandle );
			fwrite( matName.c_str(), numberOfBytesToWrite , 1, m_outputFileHandle );

			fprintf( m_logFile, "%i", numberOfBytesToWrite );
			fprintf( m_logFile, "%s\n", matName.c_str() );

			int supportedTextureCount = 0;
			for( int t = 0; t < mat->GetNumberOfTextureMaps(); t++ )
			{
				IGameTextureMap* texture = mat->GetIGameTextureMap(t);
				int textureType = texture->GetStdMapSlot();
				if( textureType == ID_BU || textureType == ID_SP || textureType == ID_DI )
				{
					supportedTextureCount++;
				}
			}

			// ex: 3
			fwrite( &supportedTextureCount, sizeof(int), 1, m_outputFileHandle );
			fprintf( m_logFile, "Support texture count: %i\n", supportedTextureCount );

			char diffuseTag[] = "diff";
			char specTag[] = "spec";
			char bumpTag[] = "bump";

			for( int j = 0; j < mat->GetNumberOfTextureMaps(); j++ )
			{
				IGameTextureMap* texture = mat->GetIGameTextureMap(j);
				int textureType = texture->GetStdMapSlot();
				int numberOfBytes = 0;
				std::wstring texNameWStr;
				std::string texNameFull;
				std::string texName; 

				switch( textureType )
				{
					case ID_DI: fwrite( diffuseTag, sizeof(char), sizeof(diffuseTag), m_outputFileHandle );
								texNameWStr = texture->GetBitmapFileName();
								texNameFull = ConvertWStringToUTF8( texNameWStr );
								texName = str::basename( texNameFull );
								numberOfBytes = texName.size() + 1;

								fwrite( &numberOfBytes, sizeof(int), 1, m_outputFileHandle );
								fwrite( texName.c_str(), numberOfBytes , 1, m_outputFileHandle );

								fprintf( m_logFile, "%i", numberOfBytes );
								fprintf( m_logFile, "%s\n", texName.c_str() );
								break;

					case ID_SP:	fwrite( specTag, sizeof(char), sizeof(specTag), m_outputFileHandle );
								texNameWStr = texture->GetBitmapFileName();
								texNameFull = ConvertWStringToUTF8( texNameWStr );
								texName = str::basename( texNameFull );
								numberOfBytes = texName.size() + 1;

								fwrite( &numberOfBytes, sizeof(int), 1, m_outputFileHandle );
								fwrite( texName.c_str(), numberOfBytes, 1, m_outputFileHandle );

								fprintf( m_logFile, "%i", numberOfBytes );
								fprintf( m_logFile, "%s\n", texName.c_str() );
								break;

					case ID_BU: fwrite( bumpTag, sizeof(char), sizeof(bumpTag), m_outputFileHandle );
								texNameWStr = texture->GetBitmapFileName();
								texNameFull = ConvertWStringToUTF8( texNameWStr );
								texName = str::basename( texNameFull );
								numberOfBytes = texName.size() + 1;

								fwrite( &numberOfBytes, sizeof(int), 1, m_outputFileHandle );
								fwrite( texName.c_str(), numberOfBytes, 1, m_outputFileHandle );

								fprintf( m_logFile, "%i", numberOfBytes );
								fprintf( m_logFile, "%s\n", texName.c_str() );
								break;
				}
			}
		}
	}
}

void My3dsmaxExporter::ExportMeshes( IGameMesh* gameMesh, int& meshIndex, const std::string& nodeName, const GMatrix& worldToLocalMatrix,const GMatrix& worldToLocalMatrixNoTranslation, IGameSkin* gameSkin )
{
	std::vector<FaceEx*> faces;
	std::set<IGameMaterial*> materials;
	
	char meshTag[] = "MESH";
	char triBatchTag[] = "TRIBAT";
	char noMatTag[] = "NOMAT";
	int numTribat = 0;

	fwrite( meshTag, sizeof(char), sizeof(meshTag), m_outputFileHandle );
	fprintf( m_logFile, "%s", meshTag );
	fprintf( m_logFile, " %i\n", meshIndex );
	fprintf( m_logFile, " %s\n", nodeName.c_str() );

	int bytesToWrite = 0; 
	//get material list from all faces
	for( int i = 0; i < gameMesh->GetNumberOfFaces();i++ )
	{
		IGameMaterial* mat = gameMesh->GetMaterialFromFace(i);
		if( mat != nullptr )
			materials.insert( gameMesh->GetMaterialFromFace(i) );
	}
	
	if( materials.size() == 0)
	{
		numTribat = 1;
		fwrite( &numTribat, sizeof(int),1 , m_outputFileHandle );
		fprintf( m_logFile, "%i\n", numTribat );

		fwrite( triBatchTag, sizeof(char), sizeof(triBatchTag), m_outputFileHandle );
		fprintf( m_logFile, "%s\n", triBatchTag );

		int noMatSize = sizeof(noMatTag);

		fwrite( &noMatSize, sizeof(int), 1, m_outputFileHandle );
		fwrite( noMatTag, sizeof(char), sizeof(noMatTag), m_outputFileHandle );

		fprintf( m_logFile, "%i", noMatSize );
		fprintf( m_logFile, "%s\n", noMatTag );

		int vertexNum = 3 * gameMesh->GetNumberOfFaces();
		fwrite( &vertexNum, sizeof(int), 1, m_outputFileHandle );
		fprintf( m_logFile, "%i\n", vertexNum );

		for( int faceIndex = 0; faceIndex < gameMesh->GetNumberOfFaces(); faceIndex++ )
		{
			FaceEx* f = gameMesh->GetFace(faceIndex);
			for( int vertIndex = 0; vertIndex < 3; vertIndex++ )
			{
				int binormalTangentIndex = gameMesh->GetFaceVertexTangentBinormal( faceIndex, vertIndex );
				Point3 tempVertexPos = gameMesh->GetVertex(f->vert[vertIndex]) * worldToLocalMatrix;
				Point2 tempVertexTexCoords = gameMesh->GetTexVertex(f->texCoord[vertIndex]);
				Point3 tempVertexColor = gameMesh->GetColorVertex(f->color[vertIndex]);

				Point3 tempVertexNorm = gameMesh->GetNormal(f->norm[vertIndex]) * worldToLocalMatrixNoTranslation;
				Point3 tempTangent = gameMesh->GetTangent( binormalTangentIndex ) * worldToLocalMatrixNoTranslation;
				Point3 tempBinormal = gameMesh->GetBinormal( binormalTangentIndex ) * worldToLocalMatrixNoTranslation;
				int vertexIndex = f->vert[vertIndex];

				//write triangle bat to file
				fwrite( &tempVertexPos.x, sizeof(float), 3, m_outputFileHandle );
				fwrite( &tempVertexColor.x, sizeof(float), 3, m_outputFileHandle );
				fwrite( &tempVertexTexCoords.x, sizeof(float), 2, m_outputFileHandle );
				fwrite( &tempVertexNorm.x, sizeof(float), 3, m_outputFileHandle );
				fwrite( &tempTangent.x, sizeof(float), 3, m_outputFileHandle );
				fwrite( &tempBinormal.x, sizeof(float), 3, m_outputFileHandle );

				fprintf( m_logFile, "%f, ", tempVertexPos.x );
				fprintf( m_logFile, "%f, ", tempVertexPos.y );
				fprintf( m_logFile, "%f\n", tempVertexPos.z );
				fprintf( m_logFile, "%f, ", tempVertexColor.x );
				fprintf( m_logFile, "%f, ", tempVertexColor.y );
				fprintf( m_logFile, "%f\n", tempVertexColor.z );
				fprintf( m_logFile, "%f ,", tempVertexTexCoords.x );
				fprintf( m_logFile, "%f\n", tempVertexTexCoords.y );
				fprintf( m_logFile, "%f ,", tempVertexNorm.x );
				fprintf( m_logFile, "%f ,", tempVertexNorm.y );
				fprintf( m_logFile, "%f\n", tempVertexNorm.z );
				fprintf( m_logFile, "%f ,", tempTangent.x );
				fprintf( m_logFile, "%f ,", tempTangent.y );
				fprintf( m_logFile, "%f\n", tempTangent.z );
				fprintf( m_logFile, "%f ,", tempBinormal.x );
				fprintf( m_logFile, "%f ,", tempBinormal.y );
				fprintf( m_logFile, "%f\n", tempBinormal.z );

				int numBones = 0;
				if( gameSkin != nullptr )
				{
					numBones = gameSkin->GetNumberOfBones( vertexIndex );
					fprintf( m_logFile, "Bone Num: %i\n", numBones );
					std::vector<boneInfo> boneList;
					std::vector<boneInfo> sortedBone;
					int boneIndex = 0;

					for( boneIndex = 0; boneIndex < numBones; boneIndex++ )
					{
						boneInfo bone;
						IGameNode* boneNode = gameSkin->GetIGameBone( vertexIndex , boneIndex );
						const wchar_t* boneNameWstr = boneNode->GetName();
						std::string boneName = ConvertWStringToUTF8( boneNameWstr );

						bone.boneName = boneName;
						bone.boneWeight = gameSkin->GetWeight( vertexIndex , boneIndex );
						bone.boneID = gameSkin->GetBoneID( vertexIndex , boneIndex );
						boneList.push_back(bone);
					}

					int count = 0;
					while( boneList.size() > 0 && count < 4 )
					{
						int highestWeightIndex = 0;
						for( boneIndex = 0; boneIndex < boneList.size(); boneIndex++ )
						{
							if( boneList[highestWeightIndex].boneWeight < boneList[boneIndex].boneWeight )
								highestWeightIndex = boneIndex;
						}
						boneInfo temp = boneList[highestWeightIndex];
						sortedBone.push_back( temp );
						boneList.erase( boneList.begin() + highestWeightIndex );
						count++;
					}

					float sumWeight = 0.f;
					for( boneIndex = 0; boneIndex < sortedBone.size(); boneIndex++ )
					{
						sumWeight += sortedBone[boneIndex].boneWeight;
					}

					numBones = sortedBone.size();
					fwrite( &numBones, sizeof(int), 1, m_outputFileHandle );
					for( boneIndex = 0; boneIndex < sortedBone.size(); boneIndex++ )
					{
						std::string name = sortedBone[boneIndex].boneName;
						std::set<BoneNode*>::iterator boneIter;
						int vertBoneIndex = 0;
						count = 0;
						for( boneIter = m_bones.begin(); boneIter != m_bones.end(); ++boneIter )
						{
							std::string boneName = ConvertWStringToUTF8( (*boneIter)->boneNode->GetName() );
							if( name.compare( boneName ) == 0 )
							{
								vertBoneIndex = count;
								break;
							}
							count++;
						}

						fwrite( &vertBoneIndex, sizeof(int),1, m_outputFileHandle );
						float boneWeight = sortedBone[boneIndex].boneWeight / sumWeight;
						fwrite( &boneWeight, sizeof(float), 1, m_outputFileHandle );
						fprintf( m_logFile, "Bone Info: %s %i %f\n", sortedBone[boneIndex].boneName.c_str(), vertBoneIndex, boneWeight );
					}
				}
				else
				{
					fwrite( &numBones, sizeof(int), 1, m_outputFileHandle );
					fprintf( m_logFile, "Bone Num: %i\n", numBones );
				}
			}
		}
	}
	else
	{
		numTribat = materials.size();
		fwrite( &numTribat, sizeof(int),1 , m_outputFileHandle );
		fprintf( m_logFile, "%i\n", numTribat );
		//export mesh list

		faces.reserve( gameMesh->GetNumberOfFaces() );
		for( auto iter = materials.begin(); iter != materials.end(); ++iter )
		{
			faces.clear();
			IGameMaterial* material = *iter;
			if( material != nullptr )
			{
				for( int i = 0; i < gameMesh->GetNumberOfFaces(); i++ )
				{
					if( material == gameMesh->GetMaterialFromFace(i) )
					{
						faces.push_back( gameMesh->GetFace(i) );
					}
				}

				fwrite( triBatchTag, sizeof(char), sizeof(triBatchTag), m_outputFileHandle );
				fprintf( m_logFile, "%s\n", triBatchTag );

				std::wstring matNameWStr = material->GetMaterialName();
				std::string matName = ConvertWStringToUTF8( matNameWStr );
				bytesToWrite = matName.size() + 1;

				fwrite( &bytesToWrite, sizeof(int), 1, m_outputFileHandle );
				fwrite( matName.c_str(), bytesToWrite, 1, m_outputFileHandle );

				fprintf( m_logFile, "%i", bytesToWrite );
				fprintf( m_logFile, "%s\n", matName.c_str() );

				int vertexNum = 3 * faces.size();
				fwrite( &vertexNum, sizeof(int), 1, m_outputFileHandle );
				fprintf( m_logFile, "%i\n", vertexNum );

				for( unsigned int faceIndex = 0; faceIndex < faces.size(); faceIndex++ )
				{
					FaceEx* f = faces[faceIndex];
					for( int vertIndex = 0; vertIndex < 3; vertIndex++ )
					{
						int binormalTangentIndex = gameMesh->GetFaceVertexTangentBinormal( faceIndex, vertIndex );
						Point3 tempVertexPos = gameMesh->GetVertex(f->vert[vertIndex]) * worldToLocalMatrix;;
						Point2 tempVertexTexCoords = gameMesh->GetTexVertex(f->texCoord[vertIndex]);
						Point3 tempVertexColor = gameMesh->GetColorVertex(f->color[vertIndex]);
						Point3 tempVertexNorm = gameMesh->GetNormal(f->norm[vertIndex]) * worldToLocalMatrixNoTranslation;
						Point3 tempTangent = gameMesh->GetTangent( binormalTangentIndex ) * worldToLocalMatrixNoTranslation;
						Point3 tempBinormal = gameMesh->GetBinormal( binormalTangentIndex ) * worldToLocalMatrixNoTranslation;
						int vertexIndex = f->vert[vertIndex];

						//write triangle bat to file
						fwrite( &tempVertexPos.x, sizeof(float), 3, m_outputFileHandle );
						fwrite( &tempVertexColor.x, sizeof(float), 3, m_outputFileHandle );
						fwrite( &tempVertexTexCoords.x, sizeof(float), 2, m_outputFileHandle );
						fwrite( &tempVertexNorm.x, sizeof(float), 3, m_outputFileHandle );
						fwrite( &tempTangent.x, sizeof(float), 3, m_outputFileHandle );
						fwrite( &tempBinormal.x, sizeof(float), 3, m_outputFileHandle );

						fprintf( m_logFile, "%f, ", tempVertexPos.x );
						fprintf( m_logFile, "%f, ", tempVertexPos.y );
						fprintf( m_logFile, "%f\n", tempVertexPos.z );
						fprintf( m_logFile, "%f, ", tempVertexColor.x );
						fprintf( m_logFile, "%f, ", tempVertexColor.y );
						fprintf( m_logFile, "%f\n", tempVertexColor.z );
						fprintf( m_logFile, "%f ,", tempVertexTexCoords.x );
						fprintf( m_logFile, "%f\n", tempVertexTexCoords.y );
						fprintf( m_logFile, "%f ,", tempVertexNorm.x );
						fprintf( m_logFile, "%f ,", tempVertexNorm.y );
						fprintf( m_logFile, "%f\n", tempVertexNorm.z );
						fprintf( m_logFile, "%f ,", tempTangent.x );
						fprintf( m_logFile, "%f ,", tempTangent.y );
						fprintf( m_logFile, "%f\n", tempTangent.z );
						fprintf( m_logFile, "%f ,", tempBinormal.x );
						fprintf( m_logFile, "%f ,", tempBinormal.y );
						fprintf( m_logFile, "%f\n", tempBinormal.z );

						int numBones = 0;
						if( gameSkin != nullptr )
						{
							numBones = gameSkin->GetNumberOfBones(vertexIndex);
							int boneIndex = 0;
							fprintf( m_logFile, "Bone Num: %i\n", numBones );
							std::vector<boneInfo> boneList;
							std::vector<boneInfo> sortedBone;

							for( boneIndex = 0; boneIndex < numBones; boneIndex++ )
							{
								boneInfo bone;
								IGameNode* boneNode = gameSkin->GetIGameBone( vertexIndex, boneIndex );
								const wchar_t* boneNameWstr = boneNode->GetName();
								std::string boneName = ConvertWStringToUTF8( boneNameWstr );

								bone.boneName = boneName;
								bone.boneWeight = gameSkin->GetWeight( vertexIndex, boneIndex );
								bone.boneID = gameSkin->GetBoneID( vertexIndex, boneIndex );
								boneList.push_back(bone);
							}

							int count = 0;
							while( boneList.size() > 0 && count < 4 )
							{
								int highestWeightIndex = 0;
								for( boneIndex = 0; boneIndex < boneList.size(); boneIndex++ )
								{
									if( boneList[highestWeightIndex].boneWeight < boneList[boneIndex].boneWeight )
										highestWeightIndex = boneIndex;
								}
								boneInfo temp = boneList[highestWeightIndex];
								sortedBone.push_back( temp );
								boneList.erase( boneList.begin() + highestWeightIndex );
								count++;
							}

							float sumWeight = 0.f;
							for( boneIndex = 0; boneIndex < sortedBone.size(); boneIndex++ )
							{
								sumWeight += sortedBone[boneIndex].boneWeight;
							}

							numBones = sortedBone.size();
							fwrite( &numBones, sizeof(int), 1, m_outputFileHandle );
							for( boneIndex = 0; boneIndex < sortedBone.size(); boneIndex++ )
							{
								std::string name = sortedBone[boneIndex].boneName;
								std::set<BoneNode*>::iterator boneIter;
								int vertBoneIndex = 0;
								count = 0;
								for( boneIter = m_bones.begin(); boneIter != m_bones.end(); ++boneIter )
								{
									std::string boneName = ConvertWStringToUTF8( (*boneIter)->boneNode->GetName() );
									if( name.compare( boneName ) == 0 )
									{
										vertBoneIndex = count;
										break;
									}
									count++;
								}

								fwrite( &vertBoneIndex , sizeof(int),1, m_outputFileHandle );
								float boneWeight = sortedBone[boneIndex].boneWeight / sumWeight;
								fwrite( &boneWeight, sizeof(float), 1, m_outputFileHandle );
								fprintf( m_logFile, "Bone Info: %s %i %f\n", sortedBone[boneIndex].boneName.c_str(), vertBoneIndex, boneWeight );
							}

						}
						else
						{
							fwrite( &numBones, sizeof(int), 1, m_outputFileHandle );
							fprintf( m_logFile, "Bone Num: %i\n", numBones );
						}
					}
				}
			}
		}
	}
}


void My3dsmaxExporter::ExportSkeletalAnimation()
{
	char tag[] = "SKELETON";
	int totalBone = m_bones.size();

	fprintf( m_logFile, "%s\n", "SKELETON" );
	fprintf( m_logFile, "Total Bone: %i\n", totalBone );

	fwrite( tag, sizeof(char), 9, m_outputFileHandle );
	fwrite( &totalBone, sizeof(int), 1, m_outputFileHandle );

	char noParent[] = "No Parent";
	std::set<BoneNode*>::iterator boneIter;
	for( boneIter = m_bones.begin(); boneIter != m_bones.end(); ++boneIter )
	{
		IGameNode* parent = nullptr;
		parent = (*boneIter)->boneNode->GetNodeParent();

		const wchar_t* boneName = (*boneIter)->boneNode->GetName();
		std::string boneNameStr = ConvertWStringToUTF8( boneName );
		int nameSize = boneNameStr.size() + 1;
		fwrite( &nameSize, sizeof(int), 1, m_outputFileHandle );
		fwrite( boneNameStr.c_str(), nameSize, 1, m_outputFileHandle );
		fprintf( m_logFile, "Bone Name: %s.\n", boneNameStr.c_str() );

		if( parent == nullptr )
		{
			int noParentSize = 10;
			fwrite( &noParentSize, sizeof(int), 1, m_outputFileHandle );
			fwrite( noParent, sizeof(char), 10, m_outputFileHandle );
			fprintf( m_logFile, "No Parent.\n" );
		}
		else
		{
			const wchar_t* parentName = parent->GetName();
			std::string name = ConvertWStringToUTF8( parentName );
			int nameSize = name.size() + 1;
			fwrite( &nameSize, sizeof(int), 1, m_outputFileHandle );
			fwrite( name.c_str(), nameSize, 1, m_outputFileHandle );
			fprintf( m_logFile, "Parent Name: %s.\n", name.c_str() );
		}

		GMatrix localTM = (*boneIter)->boneNode->GetLocalTM();

		for( int i = 0; i < 4; i++ )
		{
			Point4 column = localTM.GetColumn(i);
			fwrite( &column.x, sizeof(float), 4, m_outputFileHandle );
			fprintf( m_logFile, "%f ,", column.x );
			fprintf( m_logFile, "%f ,", column.y );
			fprintf( m_logFile, "%f ,", column.z );
			fprintf( m_logFile, "%f\n", column.w );
		}

		fprintf( m_logFile, "%s.\n", "Skeletal Animation" );
		ExportBoneAnimations( (*boneIter) );
	}
}

void My3dsmaxExporter::GetListOfBone( IGameSkin* gameSkin, IGameNode* gameNode )
{
	if( gameSkin == nullptr )
	{
		return;
	}

	int boneNum = gameSkin->GetTotalSkinBoneCount();
	for( int boneIndex = 0; boneIndex < boneNum; boneIndex++ )
	{
		BoneNode* boneNode = new BoneNode;
		IGameNode* gameBone = gameSkin->GetIGameBone( boneIndex );
		if( gameBone != nullptr )
		{
			boneNode->boneNode = gameBone;
			boneNode->gameNode = gameNode;
			m_bones.insert( boneNode );
		}
	}
}


void My3dsmaxExporter::ExportNodeInfo( IGameNode* gameNode )
{
	char nodeTag[] = "NODE";

	fwrite( nodeTag, sizeof(char), 5, m_outputFileHandle );
	fprintf( m_logFile, "%s\n", nodeTag );

	std::wstring nodeNameWStr = gameNode->GetName();
	std::string nodeName = ConvertWStringToUTF8( nodeNameWStr );
	int sizeOfNodeName = nodeName.size() + 1;

	fwrite( &sizeOfNodeName, sizeof(int), 1, m_outputFileHandle );
	fwrite( nodeName.c_str(), sizeOfNodeName, 1, m_outputFileHandle );
	
	fprintf( m_logFile, "%i", sizeOfNodeName );
	fprintf( m_logFile, "%s\n", nodeName.c_str() );

	GMatrix localTM = gameNode->GetLocalTM();
	for( int i = 0; i < 4; i++ )
	{
		Point4 column = localTM.GetColumn(i);
		fwrite( &column.x, sizeof(float), 4, m_outputFileHandle );
		fprintf( m_logFile, "%f ,", column.x );
		fprintf( m_logFile, "%f ,", column.y );
		fprintf( m_logFile, "%f ,", column.z );
		fprintf( m_logFile, "%f\n", column.w );
	}

	IGameNode* parentNode = gameNode->GetNodeParent();

	if( parentNode != nullptr )
	{
		std::wstring parentNodeNameWstr = parentNode->GetName();
		std::string parentNodeName = ConvertWStringToUTF8( parentNodeNameWstr );
		int sizeOfName = parentNodeName.size() + 1;
		fwrite( &sizeOfName, sizeof(int), 1, m_outputFileHandle );
		fwrite( parentNodeName.c_str(), sizeOfName, 1, m_outputFileHandle );

		fprintf( m_logFile, "%i", sizeOfName );
		fprintf( m_logFile, "%s\n", parentNodeName.c_str() );
		
	}
	else
	{
		int noParent = -1;
		fwrite( &noParent, sizeof(int), 1, m_outputFileHandle );

		fprintf( m_logFile, "%i\n", noParent );
	}

	int index = -1;

	for( unsigned int i = 0; i < m_meshWithNodeNameList.size(); i++ )
	{
		std::string& meshNodeName = m_meshWithNodeNameList[i];

		if( meshNodeName.compare(nodeName) == 0 )
		{
			fwrite( &i, sizeof(int), 1, m_outputFileHandle );

			fprintf( m_logFile, "Node Index: %i\n", i );
			index = i;
			break;
		}
	}
	
	//if no mesh
	if( index == -1 )
	{
		fwrite( &index, sizeof(int), 1, m_outputFileHandle );
		fprintf( m_logFile, "No Mesh %i\n", index );
	}

	ExportAnimations( gameNode );

	int nodeChildCount = gameNode->GetChildCount();

	if( nodeChildCount == 0 )
		return;

	for( int i = 0; i < nodeChildCount; i++ )
	{
		IGameNode* childNode = gameNode->GetNodeChild(i);
		ExportNodeInfo( childNode );
	}
}

void My3dsmaxExporter::ExportBoneAnimations( BoneNode* boneNode )
{
	int timeStep = 4800 / 30;
	bool isStatic = true;

	//number of frame
	int frameNum = 0;

	for( int time = m_sceneStartTime; time < m_sceneEndTime; time += timeStep )
	{
		frameNum++;
	}

	GMatrix current, next;

	IGameNode* mesh = boneNode->gameNode;
	IGameNode* bone = boneNode->boneNode;

	for( int time = m_sceneStartTime; time < m_sceneEndTime; time += timeStep )
	{
		current = bone->GetLocalTM(time);
		next = bone->GetLocalTM( time + timeStep );
		if( !(current == next) )
		{
			isStatic = false;
			break;
		}
	}

	if( isStatic )
		frameNum = 1;


	fwrite( &frameNum, sizeof(int), 1, m_outputFileHandle );
	fprintf( m_logFile, "Number of Frame: %i\n", frameNum );
	
	GMatrix boneTM;
	GMatrix meshTM = mesh->GetWorldTM(0);

	if( !isStatic )
	{
		for( int time = m_sceneStartTime; time < m_sceneEndTime; time += timeStep )
		{
			boneTM = meshTM * bone->GetWorldTM(0).Inverse() * bone->GetWorldTM(time);

			for( int i = 0; i < 4; i++ )
			{
				Point4 column = boneTM.GetColumn(i);
				fwrite( &column.x, sizeof(float), 4, m_outputFileHandle );
				fprintf( m_logFile, "%f ,", column.x );
				fprintf( m_logFile, "%f ,", column.y );
				fprintf( m_logFile, "%f ,", column.z );
				fprintf( m_logFile, "%f\n", column.w );
			}
		}
	}
	else
	{
		boneTM = meshTM;

		for( int i = 0; i < 4; i++ )
		{
			Point4 column = boneTM.GetColumn(i);
			fwrite( &column.x, sizeof(float), 4, m_outputFileHandle );
			fprintf( m_logFile, "%f ,", column.x );
			fprintf( m_logFile, "%f ,", column.y );
			fprintf( m_logFile, "%f ,", column.z );
			fprintf( m_logFile, "%f\n", column.w );
		}
	}
}


void My3dsmaxExporter::ExportAnimations( IGameNode* gameNode )
{
	int timeStep = 4800 / 30;
	bool isStatic = true;

	//number of frame
	int frameNum = 0;

	for( int time = m_sceneStartTime; time < m_sceneEndTime; time += timeStep )
	{
		frameNum++;
	}

	GMatrix current, next;

	for( int time = m_sceneStartTime; time < m_sceneEndTime; time += timeStep )
	{
		current = gameNode->GetLocalTM(time);
		next = gameNode->GetLocalTM( time + timeStep );
		if( !(current == next) )
		{
			isStatic = false;
			break;
		}
	}

	if( isStatic )
		frameNum = 1;

	fwrite( &frameNum, sizeof(int), 1, m_outputFileHandle );
	fprintf( m_logFile, "Number of Frame: %i\n", frameNum );
	GMatrix localTM;

	if( !isStatic )
	{
		for( int time = m_sceneStartTime; time < m_sceneEndTime; time += timeStep )
		{
			localTM = gameNode->GetLocalTM(time);

			for( int i = 0; i < 4; i++ )
			{
				Point4 column = localTM.GetColumn(i);
				fwrite( &column.x, sizeof(float), 4, m_outputFileHandle );
				fprintf( m_logFile, "%f ,", column.x );
				fprintf( m_logFile, "%f ,", column.y );
				fprintf( m_logFile, "%f ,", column.z );
				fprintf( m_logFile, "%f\n", column.w );
			}
		}
	}
	else
	{
		localTM = gameNode->GetLocalTM(m_sceneStartTime);

		for( int i = 0; i < 4; i++ )
		{
			Point4 column = localTM.GetColumn(i);
			fwrite( &column.x, sizeof(float), 4, m_outputFileHandle );
			fprintf( m_logFile, "%f ,", column.x );
			fprintf( m_logFile, "%f ,", column.y );
			fprintf( m_logFile, "%f ,", column.z );
			fprintf( m_logFile, "%f\n", column.w );
		}
	}
}

std::string My3dsmaxExporter::ConvertWStringToUTF8( const std::wstring& wstr )
{
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string strTo( size_needed, 0 );
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
}

std::string My3dsmaxExporter::GetFileNameFromFullPath( const std::string& fullPath )
{
	std::string name = fullPath;
	std::string slash = "\\";
	std::size_t slashPos = 0;

	while( ( slashPos = name.find(slash) ) != std::string::npos )
	{
		name.erase( 0, slashPos + slash.size() );
	}
	return name;
}

void My3dsmaxExporter::ShowAbout( HWND hwnd )
{
}

int My3dsmaxExporter::ExtCount()
{
	return 1;
}

const TCHAR* My3dsmaxExporter::Ext( int /*n*/ )
{
	return _T("my3d");
}

const TCHAR* My3dsmaxExporter::LongDesc()
{
	return _T("My 3dsmax Exporter");
}

const TCHAR* My3dsmaxExporter::ShortDesc()
{
	return _T("My3dsmaxExporter");
}

const TCHAR* My3dsmaxExporter::AuthorName()
{
	return _T("<YourName>");
}

const TCHAR* My3dsmaxExporter::CopyrightMessage()
{
	return _T("Copyright (C) <YourName> 2013");
}

const TCHAR* My3dsmaxExporter::OtherMessage1()
{
	return _T("");
}

const TCHAR* My3dsmaxExporter::OtherMessage2()
{
	return _T("");
}

unsigned int My3dsmaxExporter::Version()
{
	return 1;
}

BOOL My3dsmaxExporter::SupportsOptions( int ext, DWORD options )
{
	return TRUE;
}


