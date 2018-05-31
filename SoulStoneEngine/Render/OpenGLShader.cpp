#include "OpenGLShader.hpp"
#include "GLRender.hpp"
#include "../Utilities/GameCommon.hpp"

OpenGLShader::OpenGLShader(const ShaderType& type)
{
	if(type == VERTEX_SHADER)
		m_shaderID = glCreateShader(GL_VERTEX_SHADER);
	else if(type == FRAGMENT_SHADER)
		m_shaderID = glCreateShader(GL_FRAGMENT_SHADER);
	m_shaderType = type;
}

char* OpenGLShader::LoadShaderSourceFromFile(const string& fileName)
{
	m_shaderName = fileName;
	FILE* fp;
	char* buffer;
	int fileSize;
	size_t result;

	fopen_s(&fp, fileName.c_str(), "rb");

	if(fp == nullptr)
	{
		DebuggerPrintf( "Failed to open file: %s\n", fileName);
		exit(-1);
	}
	else
	{
		fseek(fp, 0, SEEK_END);
		fileSize = ftell(fp);
		rewind(fp);

		buffer = new char[fileSize + 1];
		result = fread(buffer, 1, fileSize, fp);
		fclose(fp);
	}
	buffer[fileSize] = '\0';
	
	return buffer;	
}

void OpenGLShader::LoadAndCompileShader(const string& fileName)
{
	char* shaderSource = LoadShaderSourceFromFile(fileName);
	glShaderSource(m_shaderID, 1, &shaderSource, nullptr);
	glCompileShader(m_shaderID);

	if(!IsShaderCompilationSuccess(fileName))
	{
		exit(-1);
	}
	else
	{
		DebuggerPrintf( "%s : ", m_shaderName.c_str() );
		DebuggerPrintf( "Shader Compilation Successes.\n");
	}
	delete[] shaderSource;
	shaderSource = nullptr;
}

bool OpenGLShader::IsShaderCompilationSuccess(const std::string& fileName)
{
	int shaderStatus;
	const int LONG_PATH = 1024;
	char currentDirectory[ LONG_PATH ];
//	DWORD currentDirectoryLength = GetCurrentDirectoryA( LONG_PATH, currentDirectory );
	glGetShaderiv(m_shaderID, GL_COMPILE_STATUS, &shaderStatus);
	std::string shaderFullPath = currentDirectory +  ( "\\" + fileName ) ;
	std::string tempString;
	std::string firstErrorMess;

	if(shaderStatus == GL_FALSE)
	{
		int compileErrorMessLength;

		glGetShaderiv(m_shaderID, GL_INFO_LOG_LENGTH, &compileErrorMessLength);

		char* compileErrorMess = new char[compileErrorMessLength + 1];
		glGetShaderInfoLog(m_shaderID, compileErrorMessLength, nullptr, compileErrorMess);
		compileErrorMess[compileErrorMessLength] = '\0';
		int numError = 0;

		const GLubyte* openglVersion = glGetString( GL_VERSION );
		const GLubyte* shadingLanguageVersion = glGetString( GL_SHADING_LANGUAGE_VERSION );

		DebuggerPrintf( "OpenGL Version: %s\n", openglVersion );
		DebuggerPrintf( "Shading Language Version: %s\n", shadingLanguageVersion );
		DebuggerPrintf( "%s : Shader Compilation Failed\n\n", shaderFullPath.c_str());
		
		int newLine = 0;

		tempString = compileErrorMess;
		newLine = tempString.find("\n");
		firstErrorMess = tempString.substr(1,newLine);

		for(int i = 0 ; i < compileErrorMessLength; i++)
		{
			if( compileErrorMess[i] == '0' && compileErrorMess[i+1] == '(' )
			{
				compileErrorMess[i] = ' ';
				numError++;
				tempString = compileErrorMess;
				newLine = tempString.find("\n");
				std::string stringToPrint;
				//tempString = tempString.substr(i+1,newLine);
				stringToPrint = tempString.substr(i+1,newLine);
				tempString.erase(newLine);
 				DebuggerPrintf( "%s" ,shaderFullPath.c_str() );
 				DebuggerPrintf( "%s\n", stringToPrint.c_str() );//testing only
				tempString.clear();
			}
		}
		

		char caption[256];
		char windowMessage[2048];

		strcpy_s(caption, fileName.c_str());
		strcat_s(caption, " Compilation Fails!");
		strcpy_s(windowMessage, fileName.c_str());
		strcat_s(windowMessage, " Compilation Fails!\n\n");
		strcat_s(windowMessage, "OpenGL Version: ");
		strcat_s(windowMessage, (const char*)openglVersion );
		strcat_s(windowMessage, "\n" );
		strcat_s(windowMessage, "Shading Language Version: ");
		strcat_s(windowMessage, (const char*)shadingLanguageVersion );
		strcat_s(windowMessage, "\n\n" );
		strcat_s(windowMessage, "ERROR:\n");
		strcat_s(windowMessage, "Line ");
		strcat_s(windowMessage, firstErrorMess.c_str());
		strcat_s(windowMessage, "\n" );
		strcat_s(windowMessage, "File Location:\n");
		strcat_s(windowMessage, shaderFullPath.c_str() );
		strcat_s(windowMessage, "\n\n" );
		strcat_s(windowMessage, "RAW ERROR LOG:\n");
		strcat_s(windowMessage, compileErrorMess );
		strcat_s(windowMessage, "\0" );
		int msgBoxID = MessageBoxA( NULL,
								    windowMessage,
								    caption ,
								    MB_ICONSTOP | MB_OK | MB_DEFBUTTON1);

		delete[] compileErrorMess;
		if( msgBoxID == IDOK )
		{
			return false;
		}

		return false;
	}
	else
	{
		return true;
	}
}