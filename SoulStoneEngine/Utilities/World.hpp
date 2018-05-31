#ifndef WORLD_H
#define WORLD_H
#include "GameCommon.hpp"
#include "../Render/GLRender.hpp"
#include "../Render/OpenGLShader.hpp"
#include "../Render/OpenGLShaderProgram.hpp"
#include "../Debugger/DeveloperConsole.hpp"
#include "../Render/BitmapFont.hpp"
#include "Matrix44.hpp"
#include "MatrixStack.hpp"
#include "../Debugger/DebugPoint.hpp"
#include "../Render/Material.hpp"
#include "../Render/FBO.hpp"
#include "../ExporterLoader.hpp"
#include "../BaseObject.hpp"
#include "../Scene.hpp"

const float CAMERA_FACTOR_SPEED = 20.f;
class World
{
	public:
		Camera3D					 m_camera3D;
		MatrixStack					 m_matrixStack;
		bool						 m_renderWorldOriginAxes;
		ExporterLoader				 m_loader;
		GLuint						 m_vboID;
		std::vector<Vertex3D>		 m_vertexList;
		int						     m_numVertex;
		Texture*					 m_texture;
		OpenGLShaderProgram*		 m_animateShaderProgram;
		OpenGLShaderProgram*		 m_staticShaderProgram;
		Vector3						 m_cameraForwardDirection;
		std::vector<Scene*>			 m_sceneList;
		Scene*						 m_testScene;
		int							 m_frame;
		int							 m_currentSceneIndex;
		float						 m_timeSinceLastUpdate;
	
	public:
		World();
		~World();
		void Initialize();
		bool ProcessKeyDownEvent(HWND windowHandle, UINT wmMessageCode, WPARAM wParam, LPARAM lParam );
		bool ProcessKeyUpEnvent(HWND windowHandle, UINT wmMessageCode, WPARAM wParam, LPARAM lParam );
		void SetupPerspectiveProjection();
		void ApplyCameraTransform(Camera3D camera);
		void UpdateCameraFromKeyboardAndMouse(Camera3D& m_camera3D,float elapsedTime);
		void OpenOrCloseConsole();
		Vector2 GetMouseSinceLastChecked();
		void Update(float elapsedTime);
		void Render();
		void RenderWorldAxes();
		void ClearWorld();
		void UpdateLightPosition( float elapsedTime );
		void Load3DScene( const std::string& name );
		void Unload3dScene();
};

extern World* theWorld;
extern bool IsKeyDownKeyboard[256];
extern bool IsKeyDownLastFrame[256];
extern MatrixStack g_matrixStack;
extern Camera3D g_camera3D;


#endif