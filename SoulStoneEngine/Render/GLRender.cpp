#include "GLRender.hpp"

PFNGLGENBUFFERSPROC					glGenBuffers				= nullptr;
PFNGLBINDBUFFERPROC					glBindBuffer				= nullptr;
PFNGLBUFFERDATAPROC					glBufferData				= nullptr;
PFNGLGENERATEMIPMAPPROC				glGenerateMipmap			= nullptr;
PFNGLDELETEBUFFERSPROC				glDeleteBuffers				= nullptr;
PFNGLSHADERSOURCEPROC				glShaderSource				= nullptr;
PFNGLCOMPILESHADERPROC				glCompileShader				= nullptr;
PFNGLGETSHADERIVPROC				glGetShaderiv				= nullptr;
PFNGLCREATEPROGRAMPROC				glCreateProgram				= nullptr;
PFNGLCREATESHADERPROC				glCreateShader				= nullptr;
PFNGLLINKPROGRAMPROC				glLinkProgram				= nullptr;
PFNGLGETPROGRAMIVPROC				glGetProgramiv				= nullptr;
PFNGLUSEPROGRAMPROC					glUseProgram				= nullptr;
PFNGLUNIFORM1FPROC					glUniform1f					= nullptr;
PFNGLUNIFORM1IPROC					glUniform1i					= nullptr;
PFNGLGETUNIFORMLOCATIONPROC			glGetUniformLocation		= nullptr;
PFNGLDELETESHADERPROC				glDeleteShader				= nullptr;
PFNGLATTACHSHADERPROC				glAttachShader				= nullptr;
PFNGLGETSHADERINFOLOGPROC			glGetShaderInfoLog			= nullptr;
PFNGLDELETEPROGRAMPROC				glDeleteProgram				= nullptr;
PFNGLACTIVETEXTUREPROC				glActiveTexture				= nullptr;
PFNGLBINDATTRIBLOCATIONPROC			glBindAttribLocation		= nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC	glEnableVertexAttribArray   = nullptr;
PFNGLVERTEXATTRIBPOINTERPROC		glVertexAttribPointer       = nullptr;
PFNGLGETATTRIBLOCATIONPROC			glGetAttribLocation			= nullptr;
PFNGLDISABLEVERTEXATTRIBARRAYPROC	glDisableVertexAttribArray  = nullptr;
PFNGLUNIFORMMATRIX4FVPROC			glUniformMatrix4fv			= nullptr;
PFNGLUNIFORM3FPROC					glUniform3f					= nullptr;
PFNGLUNIFORM4FPROC					glUniform4f					= nullptr;
PFNGLUNIFORM2FPROC					glUniform2f					= nullptr;
PFNGLUNIFORM4FVPROC					glUniform4fv				= nullptr;
PFNGLUNIFORM3FVPROC					glUniform3fv				= nullptr;
PFNGLUNIFORM2FVPROC					glUniform2fV				= nullptr;
PFNGLUNIFORM1FVPROC					glUniform1fv				= nullptr;
PFNGLUNIFORM1IVPROC					glUniform1iv				= nullptr;
PFNGLBINDFRAMEBUFFERPROC			glBindFramebuffer			= nullptr;
PFNGLGENFRAMEBUFFERSPROC			glGenFramebuffers			= nullptr;	
PFNGLFRAMEBUFFERTEXTURE2DPROC		glFramebufferTexture2D		= nullptr;

GLRender* g_glRender = nullptr;
set<Material*>				GLRender::s_materialRegisteredList;
set<OpenGLShaderProgram*>   GLRender::s_shaderProgramRegisteredList;

GLRender::GLRender()
{
	InitializeAdvancedOpenGLFunctions();
	m_defaultFont = new BitmapFont("./Data/Font/MainFont_EN.FontDef.xml","./Data/Font/MainFont_EN00.png");
	m_defaultShaderProgram = new OpenGLShaderProgram("./Shader/BasicVertexShader.glsl","./Shader/BasicFragmentShader.glsl");
}

GLRender::~GLRender()
{
	delete m_defaultFont;
	delete m_defaultShaderProgram;
}


void GLRender::BeginDraw(int value)
{
	glBegin(value);
}

void GLRender::EndDraw()
{
	glEnd();
}

void GLRender::Enable(int mode)
{
	glEnable( mode );
}

void GLRender::Disable(int mode)
{
	glDisable( mode );
}

void GLRender::PushMatrix()
{
	glPushMatrix();
}

void GLRender::PopMatrix()
{
	glPopMatrix();
}

void GLRender::LoadIdentityMatrix()
{
	glLoadIdentity();
}

void GLRender::Translatef(float x, float y, float z)
{
	glTranslatef(x,y,z);
}

void GLRender::Translated(double x, double y, double z)
{
	glTranslated(x,y,z);
}

void GLRender::Rotatef(float angle,float x, float y, float z)
{
	glRotatef(angle,x,y,z);
}

void GLRender::Rotated(double angle,double x, double y, double z)
{
	glRotated(angle,x,y,z);
}

void GLRender::Scalef(float x, float y, float z)
{
	glScalef(x,y,z);
}

void GLRender::Scaled(double x, double y, double z)
{
	glScaled(x,y,z);
}

void GLRender::BindTexture(int mode, int id)
{
	glBindTexture(mode, id);
}

void GLRender::Vertex3f(float x, float y, float z)
{
	glVertex3f(x,y,z);
}

void GLRender::Vertex3d(double x, double y, double z)
{
	glVertex3d(x,y,z);
}

void GLRender::Vertex3i(int x, int y, int z)
{
	glVertex3i(x,y,z);
}

void GLRender::TexCoord2d(double x, double y)
{
	glTexCoord2d(x,y);
}

void GLRender::TexCoord2f(float x, float y)
{
	glTexCoord2f(x,y);
}

void GLRender::Color4f(float r,float g, float b, float alpha)
{
	glColor4f(r,g,b,alpha);
}

void GLRender::Color3f(float r,float g, float b)
{
	glColor3f(r,g,b);
}

void GLRender::ClearColor(float r, float g, float b, float alpha)
{
	glClearColor(r,g,b,alpha);
}

void GLRender::ClearDepth(double value)
{
	glClearDepth(value);
}

void GLRender::EnableDepthMask()
{
	glDepthMask(true);
}

void GLRender::DisableDepthMask()
{
	glDepthMask(false);
}

void GLRender::DeleteBuffers(int size,unsigned int* id)
{
	glDeleteBuffers(size, id);
}

void GLRender::EnableClientState(int mode)
{
	glEnableClientState( mode );
}

void GLRender::DisableClientState(int mode)
{
	glDisableClientState( mode );
}

void GLRender::BindBuffer(int mode,unsigned int id)
{
	glBindBuffer(mode, id);
}

void GLRender::DrawArray(int mode, int first, int size)
{
	glDrawArrays(mode, first, size);
}

void GLRender::GenerateBuffer(int size, unsigned int* id)
{
	glGenBuffers(size,id);
}

void GLRender::PolygonMode(int mode, int mode1)
{
	glPolygonMode(mode, mode1);
}

void GLRender::BlendFunc(int mode , int mode1)
{
	glBlendFunc(mode , mode1);
}

void GLRender::Clear(int mode)
{
	glClear( mode );
}

void GLRender::PointSize(float value)
{
	glPointSize(value);
}

void GLRender::VertexPointer(int size, int mode, int stripe,const GLvoid* ptr)
{
	glVertexPointer(size, mode, stripe, ptr);
}

void GLRender::ColorPointer(int size, int mode, int stripe,const GLvoid* ptr)
{
	glColorPointer(size, mode, stripe, ptr);
}

void GLRender::TexCoordPointer(int size, int mode, int stripe,const GLvoid* ptr)
{
	glTexCoordPointer(size, mode, stripe, ptr);
}

void GLRender::FrontFace(int mode)
{
	glFrontFace(mode);
}

void GLRender::BufferData(int mode, GLsizeiptr size, const GLvoid* ptr, int mode1)
{
	glBufferData( mode, size, ptr, mode1 );
}

void GLRender::LineWidth(float size)
{
	glLineWidth(size);
}

void GLRender::DisableShaderProgram()
{
	m_shaderProgram->UseShaderProgram();
}

void GLRender::UseShaderProgram()
{
	m_shaderProgram->UseShaderProgram();
}

void GLRender::DisableDefaultShaderProgram()
{
	m_defaultShaderProgram->UseShaderProgram();
}

void GLRender::UseDefaultShaderProgram()
{
	m_defaultShaderProgram->DisableShaderProgram();
}

void GLRender::RenderText(const Vector2& position,const RGBColor& color, BitmapFont* font, OpenGLShaderProgram* shader, const float& cellHeight, const std::string& myStr)
{
	if(font == nullptr)
		font = m_defaultFont;

	if(shader == nullptr)
		shader = m_defaultShaderProgram;

	CreateVBOAndDrawFont(position, color, font, shader, cellHeight, myStr);
}

float GLRender::CalcTextWidth(  BitmapFont* font, const float& cellHeight, const std::string& myStr )
{
	if(font == nullptr)
		font = m_defaultFont;

	int asciiValue = 0;
	int index = 0;
	float ttfAWidth = 0.f;
	float ttfBWidth = 0.f;
	float ttfCWidth = 0.f;
	float textWidth = 0.f;

	for( unsigned int i = 0; i < myStr.size();i++ )
	{
		asciiValue = myStr[i];
		index = asciiValue - 32;

		ttfAWidth = font->m_bitmapFontGlypdDataList[index].ttfA * cellHeight;
		ttfBWidth = font->m_bitmapFontGlypdDataList[index].ttfB * cellHeight;
		ttfCWidth = font->m_bitmapFontGlypdDataList[index].ttfC * cellHeight;

		textWidth += ( ttfAWidth + ttfBWidth + ttfCWidth );
	}
	return textWidth;
}

void GLRender::CreateVBOAndDrawFont(const Vector2& position,const RGBColor& color, BitmapFont* font, OpenGLShaderProgram* shader, const float& cellHeight, const std::string& myStr)
{
	vertex3d tempVertex;
	Vector2 texCoordMin;
	Vector2 texCoordMax;
	int asciiValue = 0;
	int index = 0;
	float ttfAWidth = 0.f;
	float ttfBWidth = 0.f;
	float ttfCWidth = 0.f;
	Vector3 positionTodraw = Vector3(position.x,position.y,0.f);

	std::vector<vertex3d> vertexList;
	for(unsigned int i = 0; i < myStr.size();i++)
	{
		asciiValue = myStr[i];
		index = asciiValue - 32;

		texCoordMin = font->m_bitmapFontGlypdDataList[index].texCoordMins;
		texCoordMax = font->m_bitmapFontGlypdDataList[index].texCoordMax;
		ttfAWidth = font->m_bitmapFontGlypdDataList[index].ttfA * cellHeight;
		ttfBWidth = font->m_bitmapFontGlypdDataList[index].ttfB * cellHeight;
		ttfCWidth = font->m_bitmapFontGlypdDataList[index].ttfC * cellHeight;

		positionTodraw.x += ttfAWidth;

		tempVertex.m_color = color;

		//bottom left corner
		tempVertex.m_position = positionTodraw;
		//tempVertex.m_texCoords = texCoordMin;
		tempVertex.m_texCoords = Vector2(texCoordMin.x, texCoordMax.y);
		vertexList.push_back(tempVertex);

		//bottom right corner
		tempVertex.m_position = Vector3(positionTodraw.x + ttfBWidth, positionTodraw.y, 0.f);
		tempVertex.m_texCoords = texCoordMax;
		//tempVertex.m_texCoords = Vector2(texCoordMax.x,texCoordMin.y);
		vertexList.push_back(tempVertex);

		//top right corner
		tempVertex.m_position = Vector3(positionTodraw.x + ttfBWidth, positionTodraw.y + cellHeight, 0.f);
		//tempVertex.m_texCoords = texCoordMax;
		tempVertex.m_texCoords = Vector2(texCoordMax.x,texCoordMin.y);
		vertexList.push_back(tempVertex);

		//top left corner
		tempVertex.m_position = Vector3(positionTodraw.x, positionTodraw.y + cellHeight, 0.f);
		//tempVertex.m_texCoords = Vector2(texCoordMin.x, texCoordMax.y);
		tempVertex.m_texCoords = texCoordMin;
		vertexList.push_back(tempVertex);

		positionTodraw.x += ( ttfBWidth + ttfCWidth );
	}

	unsigned int vboID = 0;
	glGenBuffers( 1, &vboID  );

	glBindBuffer( GL_ARRAY_BUFFER, m_vboID );
	glBufferData( GL_ARRAY_BUFFER, sizeof( vertex3d ) * vertexList.size(), vertexList.data(), GL_STATIC_DRAW );

	m_vertexListSize = vertexList.size();

	int textureSamplerUniformLocation = glGetUniformLocation(shader->m_shaderProgramID,"u_myTexture");
	glActiveTexture(GL_TEXTURE0);
	glUniform1i( textureSamplerUniformLocation, 0);
	glBindTexture(GL_TEXTURE_2D, font->m_fontTextureSheet->m_openglTextureID);

	shader->UseShaderProgram();
	glEnable( GL_TEXTURE_2D );
	glDisable( GL_DEPTH_TEST );
	//glEnable(GL_BLEND);
	//glBlendFunc( GL_SRC_COLOR, GL_ONE );
	//glEnable(GL_DEPTH_TEST);

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );

	glVertexPointer(3, GL_FLOAT, sizeof( vertex3d ), (const GLvoid*) offsetof(vertex3d, m_position) );
	glTexCoordPointer( 2, GL_FLOAT, sizeof( vertex3d ), (const GLvoid*) offsetof(vertex3d, m_texCoords) );
	glColorPointer( 4, GL_FLOAT, sizeof( vertex3d ), (const GLvoid*) offsetof(vertex3d, m_color) );

	glDrawArrays( GL_QUADS, 0, m_vertexListSize );

	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );

	glDisable(GL_TEXTURE);
	//glDisable(GL_BLEND);
	//glDisable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_TEST);
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glDeleteBuffers( 1, &vboID );
	shader->DisableShaderProgram();
}

void GLRender::ComputeSurfaceTangentsAtVertex( Vector3& surfaceTangentAtVertex_out,
											   Vector3& surfaceBitangentAtVertex_out,
											   const Vector3& /*normalAtThisVertex*/,
											   const Vector3& positionOfThisVertex,
											   const Vector2& texCoordsOfThisVertex,
											   const Vector3& positionOfPreviousAdjacentVertex,
											   const Vector2& texCoordsOfPreviousAdjacentVertex,
											   const Vector3& positionOfNextAdjacentVertex,
											   const Vector2& texCoordsOfNextAdjacentVertex )
{
	Vector3 vecToPrevious	= positionOfPreviousAdjacentVertex - positionOfThisVertex;
	Vector3 vecToNext		= positionOfNextAdjacentVertex - positionOfThisVertex;

	Vector2 texToPrevious	= texCoordsOfPreviousAdjacentVertex - texCoordsOfThisVertex;
	Vector2 texToNext		= texCoordsOfNextAdjacentVertex - texCoordsOfThisVertex;

	float determinant = ((texToPrevious.x * texToNext.y) - (texToNext.x * texToPrevious.y));

	Vector3 uDirectionInWorldSpace(	
		(texToNext.y * vecToPrevious.x - texToPrevious.y * vecToNext.x),
		(texToNext.y * vecToPrevious.y - texToPrevious.y * vecToNext.y),
		(texToNext.y * vecToPrevious.z - texToPrevious.y * vecToNext.z)
		);

	Vector3 vDirectionInWorldSpace(
		(texToPrevious.x * vecToNext.x - texToNext.x * vecToPrevious.x),
		(texToPrevious.x * vecToNext.y - texToNext.x * vecToPrevious.y),
		(texToPrevious.x * vecToNext.z - texToNext.x * vecToPrevious.z)
		);

	float invDeterminant = 1.0f / determinant;
	uDirectionInWorldSpace *= invDeterminant;
	vDirectionInWorldSpace *= invDeterminant;

	surfaceTangentAtVertex_out = uDirectionInWorldSpace;
	surfaceBitangentAtVertex_out = vDirectionInWorldSpace; 
}

void GLRender::CreateInfinitePointLight( const Vector3& position, const Vector3& direction, const RGBColor& color )
{
	if( m_lightList.size() == MAX_LIGHT_NUM - 1 )
	{
		DebuggerPrintf("Exceed Maximum Number of Light.\n");
		return;
	}
	Light* temp = Light::CreateInfinitePointLight( position, direction, color );
	m_lightList.push_back(temp);
}

void GLRender::CreateAttenuatedPointLight( const Vector3& position, const Vector3& direction, const RGBColor& color, float innerRadius, float outerRadius )
{
	if( m_lightList.size() == MAX_LIGHT_NUM - 1 )
	{
		DebuggerPrintf("Exceed Maximum Number of Light.\n");
		return;
	}
	Light* temp = Light::CreateAttenuatedPointLight( position, direction, color, innerRadius, outerRadius );
	m_lightList.push_back(temp);
}

void GLRender::CreateAttenuatedSpotLight( const Vector3& position, const Vector3& direction, const RGBColor& color, float innerRadius, float outerRadius, float innerApertureDeg, float outerApertureDegs )
{
	if( m_lightList.size() == MAX_LIGHT_NUM - 1 )
	{
		DebuggerPrintf("Exceed Maximum Number of Light.\n");
		return;
	}
	Light* temp = Light::CreateAttenuatedSpotLight( position, direction, color, innerRadius, outerRadius, innerApertureDeg, outerApertureDegs );
	m_lightList.push_back(temp);
}

void GLRender::CreateInfiniteSpotLight( const Vector3& position, const Vector3& direction, const RGBColor& color, float innerApertureDeg, float outerApertureDeg )
{
	if( m_lightList.size() == MAX_LIGHT_NUM - 1 )
	{
		DebuggerPrintf("Exceed Maximum Number of Light.\n");
		return;
	}
	Light* temp = Light::CreateInfiniteSpotLight( position, direction, color, innerApertureDeg, outerApertureDeg );
	m_lightList.push_back(temp);
}

void GLRender::CreateLocalAmbientLight( const Vector3& position, const Vector3& direction, const RGBColor& color, float ambientNess )
{
	if( m_lightList.size() == MAX_LIGHT_NUM - 1)
	{
		DebuggerPrintf("Exceed Maximum Number of Light.\n");
		return;
	}
	Light* temp = Light::CreateLocalAmbientLight( position, direction, color, ambientNess );
	m_lightList.push_back(temp);
}

void GLRender::SetUniformValueOfLightToShader(int shaderProgramID)
{
	Vector3 lightPositions[MAX_LIGHT_NUM];
	RGBColor lightColorAndBrightness[MAX_LIGHT_NUM];
	float lightInnerApertureDot[MAX_LIGHT_NUM];
	float lightOuterApertureDot[MAX_LIGHT_NUM];
	float lightInnerRadius[MAX_LIGHT_NUM];
	float lightOuterRadius[MAX_LIGHT_NUM];
	float lightAbmientNess[MAX_LIGHT_NUM];
	Vector3 lightForwardDirection[MAX_LIGHT_NUM];
	int isDirectionalLight[MAX_LIGHT_NUM];

	int numActualLights = m_lightList.size();

	for( int i = 0; i < MAX_LIGHT_NUM; i++ )
	{
		if( i < numActualLights )
		{
			lightPositions[i] = m_lightList[i]->m_position;
			lightColorAndBrightness[i] = m_lightList[i]->m_colorAndBrightness;
			lightInnerApertureDot[i] = m_lightList[i]->m_innerApertureDot;
			lightOuterApertureDot[i] = m_lightList[i]->m_outerApertureDot;
			lightInnerRadius[i] = m_lightList[i]->m_innerRadius;
			lightOuterRadius[i] = m_lightList[i]->m_outerRadius;
			lightAbmientNess[i] = m_lightList[i]->m_ambientness;
			lightForwardDirection[i] = m_lightList[i]->m_forwardDirection;
			isDirectionalLight[i] = m_lightList[i]->m_isDirectionalLight;
		}
		else
		{
			lightPositions[i] = Vector3(0.f,0.f,0.f);
			lightColorAndBrightness[i] = RGBColor(0.f,0.f,0.f,0.f);
			lightInnerApertureDot[i] = -2.f;
			lightOuterApertureDot[i] = -3.f;
			lightInnerRadius[i] = VERY_FAR;
			lightOuterRadius[i] = VERY_FAR + 1;
			lightAbmientNess[i] = 0.f;
			lightForwardDirection[i] = Vector3(0.f,0.f,1.f);
			isDirectionalLight[i] = 0;
		}
	}
	glUseProgram(shaderProgramID);

	int lightPositionsUniformLoc		  = glGetUniformLocation(shaderProgramID, "u_lightPositions");
	int lightColorAndBrightnessUniformLoc = glGetUniformLocation(shaderProgramID, "u_lightColorAndBrightness");
	int lightInnerApertureDotUniformLoc	  = glGetUniformLocation(shaderProgramID, "u_lightInnerApertureDot");
	int lightOuterApertureDotUniforLoc	  = glGetUniformLocation(shaderProgramID, "u_lightOuterApertureDot");
	int lightInnerRadiusUniformLoc		  = glGetUniformLocation(shaderProgramID, "u_lightInnerRadius");
	int lightOuterRadiusUniformLoc		  = glGetUniformLocation(shaderProgramID, "u_lightOuterRadius");
	int lightAbmientNessUniformLoc		  = glGetUniformLocation(shaderProgramID, "u_lightAmbientness");
	int lightForwardDirectionUniformLoc	  = glGetUniformLocation(shaderProgramID, "u_lightForwardDirection");
	int isDirectionalLightUniformLoc	  = glGetUniformLocation(shaderProgramID, "u_isDirectionalLight");

	glUniform3fv( lightPositionsUniformLoc, MAX_LIGHT_NUM, &lightPositions[0].x);
	glUniform4fv( lightColorAndBrightnessUniformLoc, MAX_LIGHT_NUM, &lightColorAndBrightness[0].m_red);
	glUniform1fv( lightInnerApertureDotUniformLoc, MAX_LIGHT_NUM, &lightInnerApertureDot[0]);
	glUniform1fv( lightOuterApertureDotUniforLoc, MAX_LIGHT_NUM, &lightOuterApertureDot[0]);
	glUniform1fv( lightInnerRadiusUniformLoc, MAX_LIGHT_NUM, &lightInnerRadius[0]);
	glUniform1fv( lightOuterRadiusUniformLoc, MAX_LIGHT_NUM, &lightOuterRadius[0]);
	glUniform1fv( lightAbmientNessUniformLoc, MAX_LIGHT_NUM, &lightAbmientNess[0]);	
	glUniform3fv( lightForwardDirectionUniformLoc, MAX_LIGHT_NUM, &lightForwardDirection[0].x);	
	glUniform1iv( isDirectionalLightUniformLoc, MAX_LIGHT_NUM, &isDirectionalLight[0]);
}

void GLRender::InitializeAdvancedOpenGLFunctions()
{
	glGenBuffers					= (PFNGLGENBUFFERSPROC)					wglGetProcAddress( "glGenBuffers" );
	glBindBuffer					= (PFNGLBINDBUFFERPROC)					wglGetProcAddress( "glBindBuffer" );
	glBufferData					= (PFNGLBUFFERDATAPROC)					wglGetProcAddress( "glBufferData" );
	glGenerateMipmap				= (PFNGLGENERATEMIPMAPPROC)				wglGetProcAddress( "glGenerateMipmap" );
	glDeleteBuffers					= (PFNGLDELETEBUFFERSPROC)				wglGetProcAddress( "glDeleteBuffers" );
	glShaderSource					= (PFNGLSHADERSOURCEPROC)				wglGetProcAddress( "glShaderSource"	);	
	glCompileShader					= (PFNGLCOMPILESHADERPROC)				wglGetProcAddress( "glCompileShader" );		
	glGetShaderiv					= (PFNGLGETSHADERIVPROC)				wglGetProcAddress( "glGetShaderiv" );	
	glCreateProgram					= (PFNGLCREATEPROGRAMPROC)				wglGetProcAddress( "glCreateProgram" );		
	glCreateShader					= (PFNGLCREATESHADERPROC)				wglGetProcAddress( "glCreateShader"	);	
	glLinkProgram					= (PFNGLLINKPROGRAMPROC)				wglGetProcAddress( "glLinkProgram" );		
	glGetProgramiv					= (PFNGLGETPROGRAMIVPROC)				wglGetProcAddress( "glGetProgramiv" );		
	glUseProgram					= (PFNGLUSEPROGRAMPROC)					wglGetProcAddress( "glUseProgram" );		
	glUniform1f						= (PFNGLUNIFORM1FPROC)					wglGetProcAddress( "glUniform1f" );		
	glUniform1i						= (PFNGLUNIFORM1IPROC)					wglGetProcAddress( "glUniform1i" );			
	glGetUniformLocation			= (PFNGLGETUNIFORMLOCATIONPROC)			wglGetProcAddress( "glGetUniformLocation" );
	glDeleteShader					= (PFNGLDELETESHADERPROC)				wglGetProcAddress( "glDeleteShader" );	
	glAttachShader					= (PFNGLATTACHSHADERPROC)				wglGetProcAddress( "glAttachShader" );
	glGetShaderInfoLog				= (PFNGLGETSHADERINFOLOGPROC)			wglGetProcAddress( "glGetShaderInfoLog" );
	glDeleteProgram					= (PFNGLDELETEPROGRAMPROC)				wglGetProcAddress( "glDeleteProgram" );
	glActiveTexture					= (PFNGLACTIVETEXTUREPROC)				wglGetProcAddress( "glActiveTexture" );
	glBindAttribLocation			= (PFNGLBINDATTRIBLOCATIONPROC)			wglGetProcAddress( "glBindAttribLocation" );
	glEnableVertexAttribArray		= (PFNGLENABLEVERTEXATTRIBARRAYPROC)	wglGetProcAddress( "glEnableVertexAttribArray");
	glVertexAttribPointer			= (PFNGLVERTEXATTRIBPOINTERPROC)		wglGetProcAddress( "glVertexAttribPointer" );
	glGetAttribLocation				= (PFNGLGETATTRIBLOCATIONPROC)			wglGetProcAddress( "glGetAttribLocation");		
	glDisableVertexAttribArray		= (PFNGLDISABLEVERTEXATTRIBARRAYPROC)   wglGetProcAddress( "glDisableVertexAttribArray" );
	glUniformMatrix4fv				= (PFNGLUNIFORMMATRIX4FVPROC)			wglGetProcAddress( "glUniformMatrix4fv");
	glUniform3f						= (PFNGLUNIFORM3FPROC)					wglGetProcAddress( "glUniform3f" );
	glUniform2f						= (PFNGLUNIFORM2FPROC)					wglGetProcAddress( "glUniform2f" );
	glUniform4f						= (PFNGLUNIFORM4FPROC)					wglGetProcAddress( "glUniform4f" );
	glUniform4fv					= (PFNGLUNIFORM4FVPROC)					wglGetProcAddress( "glUniform4fv" );
	glUniform3fv					= (PFNGLUNIFORM3FVPROC)					wglGetProcAddress( "glUniform3fv" );
	glUniform2fV					= (PFNGLUNIFORM2FVPROC)					wglGetProcAddress( "glUniform2fV" );
	glUniform1fv					= (PFNGLUNIFORM1FVPROC)					wglGetProcAddress( "glUniform1fv" );
	glUniform1iv					= (PFNGLUNIFORM1IVPROC)					wglGetProcAddress( "glUniform1iv" );
	glBindFramebuffer				= (PFNGLBINDFRAMEBUFFERPROC)			wglGetProcAddress( "glBindFramebuffer" );
	glGenFramebuffers				= (PFNGLGENFRAMEBUFFERSPROC)			wglGetProcAddress( "glGenFramebuffers" );	
	glFramebufferTexture2D			= (PFNGLFRAMEBUFFERTEXTURE2DPROC)		wglGetProcAddress( "glFramebufferTexture2D" );
}

void GLRender::RenderLightSource()
{
	for(unsigned int i = 0; i < m_lightList.size(); i++)
	{
		DebugPoint temp;
		temp.m_drawMode = DepthTestOn;
		temp.m_position = m_lightList[i]->m_position;
		temp.m_color = m_lightList[i]->m_colorAndBrightness;
		temp.m_size = 0.1f;
		temp.Render();
	}
}

void GLRender::CreateFBO()
{
	FBO* temp = FBO::CreateFBO();
	m_fboList.push_back(temp);
}

FBO* GLRender::GetFBObyID(int id)
{
	for( unsigned int i = 0; i < m_fboList.size(); i++)
	{
		if( m_fboList[i]->m_fboID == static_cast<unsigned int> (id) )
		{
			return m_fboList[i];
		}
	}
	return nullptr;
}

void GLRender::DisableAllTexture()
{
	for( int index = 0; Texture::s_textureRegistry.size(); index++ )
	{
		glActiveTexture(GL_TEXTURE0 + index);
		glDisable(GL_TEXTURE_2D);
	}
}

void GLRender::Draw2DCircle( const Vector2& center, float radius, const RGBColor& color )
{
	const int iteration = 100;
	glPushMatrix();
	glTranslatef( center.x, center.y, 0.f );
	glBegin( GL_TRIANGLE_FAN );
	glColor3f( color.m_red, color.m_green, color.m_blue );
	glVertex2f( 0.f, 0.f );
	Vector2 rotateVector( 0.f , 0.f + radius );
	for( int i = 0; i < iteration; i++ )
	{
		rotateVector.RotateDegree( 360.f / iteration );
		glVertex2f( rotateVector.x, rotateVector.y );
		rotateVector.RotateDegree( 360.f / iteration );
		glVertex2f( rotateVector.x, rotateVector.y );
	}
	glEnd();
	glPopMatrix();
}

void GLRender::Draw2DRectangle(const Vector2& bottomLeft, float width, float height, const RGBColor& color)
{
	glBegin( GL_POLYGON );
	glColor3f( color.m_red, color.m_green, color.m_blue );
	glVertex2f( bottomLeft.x, bottomLeft.y );
	glVertex2f( bottomLeft.x + width, bottomLeft.y );
	glVertex2f( bottomLeft.x + width, bottomLeft.y + height );
	glVertex2f( bottomLeft.x, bottomLeft.y + height );
	glEnd();
}
