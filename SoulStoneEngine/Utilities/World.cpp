#include "World.hpp"

World* theWorld = nullptr;
bool IsKeyDownKeyboard[256];
bool IsKeyDownLastFrame[256];
MatrixStack g_matrixStack;
Camera3D g_camera3D;

World::World()
{
	InitializeTime();
	Initialize();	
	m_animateShaderProgram = new OpenGLShaderProgram( "./Shader/ModernVertexShader.glsl","./Shader/ModernFragmentShader.glsl" );
	m_staticShaderProgram = new OpenGLShaderProgram( "./Shader/VertexShaderStaticMesh.glsl","./Shader/ModernFragmentShader.glsl" );
	m_currentSceneIndex = 0;
	m_timeSinceLastUpdate = 0.f;
	
	g_glRender->CreateInfiniteSpotLight( Vector3(0.f,0.f, 300.0), Vector3( 0.f,0.f, -1.f), RGBColor(1.f,1.f,1.f,1.f), 30.f, 45.f);
	//g_glRender->CreateInfinitePointLight( Vector3(-100.f,0.f, 150.0), Vector3( 0.f,0.f, -1.f), RGBColor(1.f,1.f,1.f,1.f) );

	std::string root = "./Scene/";
	for( int i = 0; i < 11; i++ )
	{
		std::string fileName = "s" + std::to_string( static_cast<long double>(i + 1)) + ".MY3D";
		fileName = root + fileName;
		Scene* temp = new Scene( fileName, 	m_animateShaderProgram, m_staticShaderProgram );
		m_sceneList.push_back(temp);
	}

	m_frame = 0;
}

World::~World()
{
}

void World::Initialize()
{
	g_theConsole->InitializeConsole();

	for(int i = 0; i < 256; i++)
	{
		IsKeyDownKeyboard[i] = false;
		IsKeyDownLastFrame[i] = false;
	}
	m_renderWorldOriginAxes = false;
	m_camera3D.m_cameraPosition = Vector3( -200.f, 0.f,0.f);
	m_vboID = 0;
}

void World::SetupPerspectiveProjection()
{
	float aspect = (16.f/9.f);
	float fovX = 70.f;
	float fovY = (fovX / aspect);
	float zNear = 0.1f;
	float zFar = 1000.f;
	//g_glRender->LoadIdentityMatrix();
	//glLoadMatrixf(m_matrixStack.StackTop());
	//gluPerspective(fovY,aspect,zNear,zFar);
	m_matrixStack.LoadIdentity();
	m_matrixStack.PushMatrix(Matrix44::CreatePerspectiveMatrix(fovY,aspect,zNear,zFar));
}

void World::ApplyCameraTransform(Camera3D camera)
{
// 		g_glRender->Rotatef(-90.f,1.f,0.f,0.f);
// 		g_glRender->Rotatef(90.f, 0.f,0.f,1.f);
// 
// 		g_glRender->Rotatef(-camera.m_rollDegreesAboutX, 1.f,0.f,0.f);
// 		g_glRender->Rotatef(-camera.m_pitchDegreesAboutY, 0.f,1.f,0.f);
// 		g_glRender->Rotatef(-camera.m_yawDegreesAboutZ , 0.f,0.f,1.f);
// 
// 		g_glRender->Translatef(-camera.m_cameraPosition.x, -camera.m_cameraPosition.y, -camera.m_cameraPosition.z);

	m_matrixStack.PushMatrix(Matrix44::CreateRotationAboutAxisDegrees(Vector3(1.f,0.f,0.f),-90.f));
	m_matrixStack.PushMatrix(Matrix44::CreateRotationAboutAxisDegrees(Vector3(0.f,0.f,1.f), 90.f));

	m_matrixStack.PushMatrix(Matrix44::CreateRotationAboutAxisDegrees(Vector3(1.f,0.f,0.f), -camera.m_rollDegreesAboutX));
	m_matrixStack.PushMatrix(Matrix44::CreateRotationAboutAxisDegrees(Vector3(0.f,1.f,0.f), -camera.m_pitchDegreesAboutY));
	m_matrixStack.PushMatrix(Matrix44::CreateRotationAboutAxisDegrees(Vector3(0.f,0.f,1.f), -camera.m_yawDegreesAboutZ));

	m_matrixStack.PushMatrix(Matrix44::CreateTranslationMatrix(Vector3(-camera.m_cameraPosition.x, -camera.m_cameraPosition.y, -camera.m_cameraPosition.z)));
	g_matrixStack = m_matrixStack;
	glLoadMatrixf(m_matrixStack.StackTop());
}

bool World::ProcessKeyDownEvent(HWND , UINT wmMessageCode, WPARAM wParam, LPARAM )
{
	unsigned char asKey = (unsigned char) wParam;
	switch( wmMessageCode )
	{
		case WM_KEYDOWN:
			IsKeyDownKeyboard[asKey] = true;
			return true;
			break;

		case WM_KEYUP:
			IsKeyDownKeyboard[asKey] = false;
			return true;
			break;
	}
	return true;
}

Vector2 World::GetMouseSinceLastChecked()
{
	POINT centerCursorPos = { 800, 450 };
	POINT cursorPos;
	GetCursorPos( &cursorPos );
	SetCursorPos( centerCursorPos.x, centerCursorPos.y );
	Vector2i mouseDeltaInts( cursorPos.x - centerCursorPos.x, cursorPos.y - centerCursorPos.y );
	Vector2 mouseDeltas( (float) mouseDeltaInts.x, (float) mouseDeltaInts.y );
	return mouseDeltas;
}

void World::OpenOrCloseConsole()
{
	if( IsKeyDownKeyboard[ VK_OEM_3 ] && IsKeyDownKeyboard[ VK_OEM_3 ] != IsKeyDownLastFrame[ VK_OEM_3 ] )
	{
		if(g_theConsole->m_isOpen == false)
			g_theConsole->m_isOpen = true;
		else
			g_theConsole->m_isOpen = false;
	}
}

void World::UpdateCameraFromKeyboardAndMouse(Camera3D& camera, float elapsedTime)
{
	const float degreesPerMouseDelta = 0.02f;
	Vector2 mouseDeltas = GetMouseSinceLastChecked();
	camera.m_yawDegreesAboutZ -= (degreesPerMouseDelta * mouseDeltas.x);

	camera.m_pitchDegreesAboutY += (degreesPerMouseDelta * mouseDeltas.y);
	camera.m_pitchDegreesAboutY = MathUtilities::Clamp(-90.f,90.f,camera.m_pitchDegreesAboutY);

	float cameraYawRadians = MathUtilities::DegToRad(camera.m_yawDegreesAboutZ);
	Vector3 cameraForwardXYVector( cos(cameraYawRadians), sin( cameraYawRadians ),0.f);

	m_cameraForwardDirection = Vector3( cameraForwardXYVector.x * cos(MathUtilities::DegToRad(camera.m_pitchDegreesAboutY)),
									    cameraForwardXYVector.y * cos(MathUtilities::DegToRad(camera.m_pitchDegreesAboutY)),
									    -sin(MathUtilities::DegToRad(camera.m_pitchDegreesAboutY)));

	m_cameraForwardDirection = m_cameraForwardDirection.Normalize();

	Vector3 movementVector( 0.f,0.f,0.f );

	if( IsKeyDownKeyboard[ 'W' ] )
		movementVector += cameraForwardXYVector * CAMERA_FACTOR_SPEED;

	if( IsKeyDownKeyboard[ 'A' ] )
		movementVector += Vector3(-cameraForwardXYVector.y,cameraForwardXYVector.x,0.f) * CAMERA_FACTOR_SPEED;

	if( IsKeyDownKeyboard[ 'D' ] )
		movementVector += Vector3(cameraForwardXYVector.y,-cameraForwardXYVector.x,0.f) * CAMERA_FACTOR_SPEED;

	if( IsKeyDownKeyboard[ 'S' ] )
		movementVector += Vector3(-cameraForwardXYVector.x,-cameraForwardXYVector.y,0.f) * CAMERA_FACTOR_SPEED;

	if( IsKeyDownKeyboard[ 'E' ] )
		movementVector += Vector3(0.f,0.f,CAMERA_FACTOR_SPEED);

	if( IsKeyDownKeyboard[ 'C' ] )
		movementVector += Vector3(0.f,0.f,-CAMERA_FACTOR_SPEED);

	if( IsKeyDownKeyboard[ VK_PRIOR ] && IsKeyDownKeyboard[ VK_PRIOR ] != IsKeyDownLastFrame[ VK_PRIOR ] )
	{
		m_currentSceneIndex++;
		m_frame = 0;
	}

	if( IsKeyDownKeyboard[ VK_NEXT ] && IsKeyDownKeyboard[ VK_NEXT ] != IsKeyDownLastFrame[ VK_NEXT ] )
	{
		m_currentSceneIndex--;
		m_frame = 0;
	}

	if( IsKeyDownKeyboard[ 'O' ] && IsKeyDownKeyboard[ 'O' ] != IsKeyDownLastFrame[ 'O' ] )
	{
		if(m_renderWorldOriginAxes == false)
			m_renderWorldOriginAxes = true;
		else
			m_renderWorldOriginAxes = false;
	}

	int isNormalMapOnUniformLoc = glGetUniformLocation( m_animateShaderProgram->m_shaderProgramID, "u_isNormalMapOn" );

	if( IsKeyDownKeyboard[ '1' ] )
	{
		glUseProgram( m_animateShaderProgram->m_shaderProgramID );
		glUniform1i( isNormalMapOnUniformLoc, 1 );
	}

	if( IsKeyDownKeyboard[ '2' ] )
	{
		glUseProgram( m_animateShaderProgram->m_shaderProgramID );
		glUniform1i( isNormalMapOnUniformLoc, 0 );
	}

	if( IsKeyDownKeyboard[ 'Q' ] )
	{
		g_theConsole->ExecuteCommand("Quit","Quit Program.");
	}

	camera.m_cameraPosition += movementVector * elapsedTime;
}

void World::Update(float elapsedTime)
{
	OpenOrCloseConsole();

	if(g_theConsole->m_isOpen)
		elapsedTime = 0.f;
	else
		UpdateCameraFromKeyboardAndMouse(m_camera3D,elapsedTime);

	g_glRender->SetUniformValueOfLightToShader( m_animateShaderProgram->m_shaderProgramID );
	g_glRender->SetUniformValueOfLightToShader( m_staticShaderProgram->m_shaderProgramID );

	UpdateLightPosition( elapsedTime );

	if( m_currentSceneIndex > 10 )
		m_currentSceneIndex = 0;

	if( m_currentSceneIndex < 0 )
		m_currentSceneIndex = 10;

	float time = GetCurrentTimeSeconds() - m_timeSinceLastUpdate;

	if( time > 1.f / 40.f )
	{
		m_frame++;
		m_timeSinceLastUpdate = GetCurrentTimeSeconds();
	}

	if( m_frame > m_sceneList[m_currentSceneIndex]->m_maxFrameNumber - 1 )
		m_frame = 0;

	g_camera3D = m_camera3D;

	for(int i = 0; i < 256; i++)
	{
		IsKeyDownLastFrame[i] = IsKeyDownKeyboard[i];
	}
}

void World::Render()
{
	SetupPerspectiveProjection();
	ApplyCameraTransform( m_camera3D );
	glClearColor(0.3f,0.5f,1.f,1.f);
	glClearDepth(1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_animateShaderProgram->DisableShaderProgram();

	g_glRender->RenderLightSource();
	
	m_sceneList[m_currentSceneIndex]->PlayAnimationManually(m_frame);
	//m_testScene->PlayAnimationManually(m_frame);

	//m_testScene->Render();

	if( !m_renderWorldOriginAxes )
		RenderWorldAxes();
		
	if(g_theConsole->m_isOpen)
		g_theConsole->Render();
}


void World::RenderWorldAxes()
{
	g_glRender->PushMatrix();
	g_glRender->Disable(GL_TEXTURE_2D);
	g_glRender->Disable(GL_DEPTH_TEST);
	//g_glRender->Scalef(2.f,2.f,2.f);
 	g_glRender->LineWidth(1.f);
 	g_glRender->Enable(GL_LINE_SMOOTH);
	g_glRender->BeginDraw(GL_LINES);
	{
		g_glRender->Color4f(1.f,0.f,0.f,1.f);
		g_glRender->Vertex3f(0.f,0.f,0.f);
		g_glRender->Vertex3f(1.f,0.f,0.f);

		g_glRender->Color4f(0.f,1.f,0.f,1.f);
		g_glRender->Vertex3f(0.f,0.f,0.f);
		g_glRender->Vertex3f(0.f,1.f,0.f);

		g_glRender->Color4f(0.f,0.f,1.f,1.f);
		g_glRender->Vertex3f(0.f,0.f,0.f);
		g_glRender->Vertex3f(0.f,0.f,1.f);
	}
	g_glRender->EndDraw();
	
	g_glRender->Enable(GL_DEPTH_TEST);
	g_glRender->LineWidth(3.f);
	g_glRender->BeginDraw(GL_LINES);
	{
		g_glRender->Color4f(1.f,0.f,0.f,1.f);
		g_glRender->Vertex3f(0.f,0.f,0.f);
		g_glRender->Vertex3f(1.f,0.f,0.f);

		g_glRender->Color4f(0.f,1.f,0.f,1.f);
		g_glRender->Vertex3f(0.f,0.f,0.f);
		g_glRender->Vertex3f(0.f,1.f,0.f);

		g_glRender->Color4f(0.f,0.f,1.f,1.f);
		g_glRender->Vertex3f(0.f,0.f,0.f);
		g_glRender->Vertex3f(0.f,0.f,1.f);
	}
	g_glRender->EndDraw();

	g_glRender->LineWidth(1.f);
	g_glRender->Color4f(1.f,1.f,1.f,1.f);
 	g_glRender->Enable(GL_TEXTURE_2D);
 	g_glRender->Disable(GL_DEPTH_TEST);
	g_glRender->PopMatrix();
}

void World::ClearWorld()
{
	m_matrixStack.ClearMatrixStack();
}

void World::UpdateLightPosition(float elapsedTime)
{
 	g_glRender->m_lightList[0]->m_position = m_camera3D.m_cameraPosition;
 	g_glRender->m_lightList[0]->m_forwardDirection = m_cameraForwardDirection;
// 
// 	static float time = 0.f;
// 	time += elapsedTime;
// 
// 	for(unsigned int i = 1; i < g_glRender->m_lightList.size();i++)
// 	{
// 		g_glRender->m_lightList[i]->m_position = Vector3( (i + 1 )* sin(time * 0.5f), (i + 1) * sin( 2.f * time * 0.5f ), g_glRender->m_lightList[i]->m_position.z );
// 	}
}

void World::Load3DScene( const std::string& name )
{
	if( name.size() == 0 )
		return;

	std::string pathName = "./Scenes/" + name;
	
}

void World::Unload3dScene()
{
	
}



