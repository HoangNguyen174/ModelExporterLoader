#pragma once
#include <map>

/**
* 3dsmax exporter plugin.
*/
struct boneInfo
{
	int			boneID;
	float		boneWeight;
	std::string boneName;
};

struct BoneNode
{
	IGameNode*	gameNode;
	IGameNode*  boneNode;
};

class My3dsmaxExporter : public SceneExport
{
	public:
		std::set<IGameMaterial*>	m_materials;
		std::set<BoneNode*>			m_bones;
		std::vector<std::string>	m_meshWithNodeNameList;
		int							m_numMeshes;
		int							m_sceneStartTime;
		int							m_sceneEndTime;

	public:
		My3dsmaxExporter();
		~My3dsmaxExporter();
		/** Exports the scene. */
		int				DoExport( const TCHAR* name, ExpInterface *ei, Interface* i, BOOL suppressprompts=FALSE, DWORD options=0 );

		/** Show DLL's "About..." box */
		void			ShowAbout( HWND hwnd );

		/** Number of extensions supported */
		int				ExtCount();
	
		/** Extension #n (i.e. "3DS") */
		const TCHAR*	Ext( int n );					
	
		/** Long ASCII description (i.e. "Autodesk 3D Studio File") */
		const TCHAR*	LongDesc();
	
		/** Short ASCII description (i.e. "3D Studio") */
		const TCHAR*	ShortDesc();
	
		/** ASCII Author name */
		const TCHAR*	AuthorName();
	
		/** ASCII Copyright message */
		const TCHAR*	CopyrightMessage();
	
		/** Other message #1 */
		const TCHAR*	OtherMessage1();
	
		/** Other message #2 */
		const TCHAR*	OtherMessage2();
	
		/** Version number * 100 (i.e. v3.01 = 301) */
		unsigned int	Version();
	
		/** Returns TRUE if option supported. */
		BOOL			SupportsOptions( int ext, DWORD options );

	private:
		IGameScene*		m_igame;
		FILE*			m_outputFileHandle;
		FILE*			m_logFile;
		void			deinit();
		void			ExportMeshes( IGameMesh* gameMesh, int& meshIndex, const std::string& nodeName, const GMatrix& worldToLocalMatrix, const GMatrix& worldToLocalMatrixNoTranslation, IGameSkin* gameSkin );
		void			ExportNodeInfo( IGameNode* gameNode );
		void			ExportSkeletalAnimation();
		void			GetListOfBone( IGameSkin* gameSkin, IGameNode* gameNode );
		void			RecurseAndExportNode( IGameNode* gameNode );
		void			ExportMaterials( );
		void			Export( IGameScene* gameScene );
		void			ExportAnimations( IGameNode* gameNode );
		void			ExportBoneAnimations( BoneNode* boneNode );
		std::string		ConvertWStringToUTF8( const std::wstring& wstr );
		std::string		GetFileNameFromFullPath( const std::string& fullPath );
		My3dsmaxExporter( const My3dsmaxExporter& );
		My3dsmaxExporter& operator=( const My3dsmaxExporter& );
		
};
