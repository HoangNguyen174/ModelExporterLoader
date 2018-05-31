#include "OpenGLShaderProgram.hpp"
#include "GLRender.hpp"

OpenGLShaderProgram::OpenGLShaderProgram(const std::string& vertexShader, const std::string& fragmentShader)
{
	m_shaderProgramID = glCreateProgram();
	m_numberOfShader = 0;

	OpenGLShader vertShader(VERTEX_SHADER);
	vertShader.LoadAndCompileShader(vertexShader);

	OpenGLShader fragShader(FRAGMENT_SHADER);
	fragShader.LoadAndCompileShader(fragmentShader);

	AttachShader( vertShader );
	AttachShader( fragShader );

	glBindAttribLocation( m_shaderProgramID, VERTEX_ATTRIB_POSITIONS, "a_vertexPosition");
	glBindAttribLocation( m_shaderProgramID, VERTEX_ATTRIB_COLORS, "a_vertexColor");
	glBindAttribLocation( m_shaderProgramID, VERTEX_ATTRIB_TEXCOORDS, "a_vertexTexCoords");
	glBindAttribLocation( m_shaderProgramID, VERTEX_ATTRIB_NORMALS, "a_vertexNormal");
	glBindAttribLocation( m_shaderProgramID, VERTEX_ATTRIB_TANGENT, "a_vertexTangent");
	glBindAttribLocation( m_shaderProgramID, VERTEX_ATTRIB_BITANGENT, "a_vertexBitangent");
	glBindAttribLocation( m_shaderProgramID, VERTEX_ATTRIB_BONE_INDEX, "a_vertexBoneIndex");
	glBindAttribLocation( m_shaderProgramID, VERTEX_ATTRIB_BONE_WEIGHT, "a_vertexBoneWeight");
	glBindAttribLocation( m_shaderProgramID, VERTEX_ATTRIB_IS_STATIC, "a_isStatic");

	LinkProgram();

	GetUniformLocation();
}

OpenGLShaderProgram::~OpenGLShaderProgram()
{
	glDeleteProgram(m_shaderProgramID);
}

void OpenGLShaderProgram::AttachShader(const OpenGLShader& shader)
{
	glAttachShader( m_shaderProgramID, shader.m_shaderID );
	m_numberOfShader++;
	if(shader.m_shaderType == FRAGMENT_SHADER)
	{
		const int LONG_PATH = 1024;
		char currentDirectory[ LONG_PATH ];
		m_fragmentShaderFullPath = currentDirectory +  ( "\\" + shader.m_shaderName ) ;
	}
}

void OpenGLShaderProgram::LinkProgram()
{
	char windowMessage[2048];
	const GLubyte* openglVersion = glGetString( GL_VERSION );
	const GLubyte* shadingLanguageVersion = glGetString( GL_SHADING_LANGUAGE_VERSION );

	GetUniformLocation();

	if(m_numberOfShader < 2)
	{
		DebuggerPrintf( "Shader Program Linking failed.\n");
		DebuggerPrintf( "Error : Need at least 2 shaders for program to link.\n");

		strcpy_s(windowMessage, "OpenGL Version: ");
		strcat_s(windowMessage, (const char*)openglVersion );
		strcat_s(windowMessage, "\n" );
		strcat_s(windowMessage, "Shading Language Version: ");
		strcat_s(windowMessage, (const char*)shadingLanguageVersion );
		strcat_s(windowMessage, "\n\n" );
		strcat_s(windowMessage, "Shader Program Linking failed.\n" );
		strcat_s(windowMessage, "Error : Need at least 2 shaders for program to link.\n");
		strcat_s(windowMessage, "\n\n" );

		int msgBoxID1 = MessageBoxA( NULL,
									windowMessage,
									"Linking Error!" ,
									MB_ICONSTOP | MB_OK | MB_DEFBUTTON1);

		if( msgBoxID1 == IDOK )
		{
			exit(-1);
		}
	}

	glLinkProgram(m_shaderProgramID);


	int linkStatus;
	glGetProgramiv( m_shaderProgramID, GL_LINK_STATUS, &linkStatus );
	if ( linkStatus == GL_FALSE)
	{

		DebuggerPrintf( "Shader Program Linking failed.\n");
		DebuggerPrintf( "%s\n", m_fragmentShaderFullPath.c_str() );

		strcpy_s(windowMessage, "OpenGL Version: ");
		strcat_s(windowMessage, (const char*)openglVersion );
		strcat_s(windowMessage, "\n" );
		strcat_s(windowMessage, "Shading Language Version: ");
		strcat_s(windowMessage, (const char*)shadingLanguageVersion );
		strcat_s(windowMessage, "\n\n" );
		strcat_s(windowMessage, "Error : Shader Program Linking failed.\n\n" );
		strcat_s(windowMessage, m_fragmentShaderFullPath.c_str() );

		int msgBoxID = MessageBoxA( NULL,
									windowMessage,
									"Linking Error!" ,
									MB_ICONSTOP | MB_OK | MB_DEFBUTTON1);

		if( msgBoxID == IDOK )
		{
			exit(-1);
		}
	}
	else
	{
		DebuggerPrintf( "Shader Program Linking Successes.\n");
	}
}

void OpenGLShaderProgram::UseShaderProgram()
{
	glUseProgram(m_shaderProgramID);
}

void OpenGLShaderProgram::DisableShaderProgram()
{
	glUseProgram(0);
}

void OpenGLShaderProgram::GetUniformLocation()
{
	m_diffuseTextureUniformLoc = glGetUniformLocation(m_shaderProgramID,"u_diffuseTexture");;
	m_normalTextureUniformLoc = glGetUniformLocation(m_shaderProgramID,"u_normalTexture");
	m_specularTextureUniformLoc = glGetUniformLocation(m_shaderProgramID,"u_specularTexture");
	m_emissiveTextureUniformLoc = glGetUniformLocation(m_shaderProgramID,"u_emissiveTexture");
	m_depthTextureUniformLoc = glGetUniformLocation(m_shaderProgramID,"u_depthTexture");
	m_worldToScreenMatrixLoc = glGetUniformLocation(m_shaderProgramID,"u_WorldToScreenMatrix");;
	m_lightWorldPosLoc = glGetUniformLocation(m_shaderProgramID,"u_lightPosition");
	m_timeUniformLoc = glGetUniformLocation(m_shaderProgramID,"u_time");
	m_cameraWorldPositionUniformLoc = glGetUniformLocation(m_shaderProgramID,"u_cameraWorldPosition");
	m_fogColorUniformLoc = glGetUniformLocation(m_shaderProgramID,"u_fogColor");
	m_fogEndDistanceUniformLoc = glGetUniformLocation(m_shaderProgramID,"u_fogEndDistance");
	m_fogStartDistanceUniformLoc = glGetUniformLocation(m_shaderProgramID,"u_fogStartDistance");
	m_scaleBiasUniformLoc = glGetUniformLocation(m_shaderProgramID,"u_scaleBias");
	m_deltaTimeUniformLoc = glGetUniformLocation(m_shaderProgramID,"u_deltaTime");
	m_localToWorldMatrixLoc = glGetUniformLocation(m_shaderProgramID,"u_LocalToWorldMatrix");
	m_boneTMLoc = glGetUniformLocation(m_shaderProgramID, "u_boneTM");
}