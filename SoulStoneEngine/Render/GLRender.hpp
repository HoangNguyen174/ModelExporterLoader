#ifndef GL_MYRENDER
#define GL_MYRENDER
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "../Utilities/Vector2.hpp"
#include "../Debugger/DebugPoint.hpp"
#include "../Render/OpenGLShaderProgram.hpp"
#include "../Utilities/GameCommon.hpp"
#include "BitmapFont.hpp"
#include "../Utilities/RGBColor.hpp"
#include "../Utilities/MatrixStack.hpp"
#include "Light.hpp"
#include "FBO.hpp"
#pragma comment( lib, "opengl32" )
#pragma comment( lib, "glu32")

const int MAX_LIGHT_NUM = 16;

struct vertex3d
{
	Vector3	m_position;
	RGBColor m_color;
	Vector2 m_texCoords;
	Vector3 m_normal;
	Vector3 m_tangent;
	Vector3 m_bitangent;
};

enum VERTEX_ATTRIB   { VERTEX_ATTRIB_POSITIONS = 0,
					   VERTEX_ATTRIB_COLORS,
					   VERTEX_ATTRIB_TEXCOORDS,
					   VERTEX_ATTRIB_NORMALS, 
					   VERTEX_ATTRIB_TANGENT,
					   VERTEX_ATTRIB_BITANGENT,
					   VERTEX_ATTRIB_BONE_INDEX,
					   VERTEX_ATTRIB_BONE_WEIGHT,
					   VERTEX_ATTRIB_IS_STATIC };
class GLRender
{
	public:
		static set<Material*>				s_materialRegisteredList;
		static set<OpenGLShaderProgram*>	s_shaderProgramRegisteredList;
		std::vector<DebugObject*>			m_debugPrimitiveToDraw;
		OpenGLShaderProgram*				m_shaderProgram;
		BitmapFont*							m_defaultFont;
		OpenGLShaderProgram*				m_defaultShaderProgram;
		std::vector<Light*>					m_lightList;
		std::vector<FBO*>					m_fboList;

	private:
		std::vector<vertex3d>				m_vertexList;
		unsigned int						m_vboID;
		int									m_vertexListSize;
		MatrixStack							m_matrixStack;

	public:
		GLRender();
		~GLRender();
		void BeginDraw(int value);
		void Clear(int mode);
		void EndDraw();
		void Enable(int mode);
		void Disable(int mode);
		void BlendFunc(int mode, int mode1);
		void PushMatrix();
		void PopMatrix();
		void LoadIdentityMatrix();
		void Translatef(float x, float y, float z);
		void Translated(double x, double y, double z);
		void Rotatef(float angle, float x, float y, float z);
		void Rotated(double angle, double x, double y, double z);
		void Scalef(float x, float y, float z);
		void Scaled(double x, double y, double z);
		void BindTexture(int mode, int id);
		void Vertex3f(float x,float y, float z);
		void Vertex3d(double x, double y, double z);
		void Vertex3i(int x,int y,int z);
		void TexCoord2d(double x, double y);
		void TexCoord2f(float x, float y);
		void Color4f(float r, float g, float b, float alpha);
		void Color3f(float r, float g, float b);
		void ClearColor(float r, float g, float b, float alpha);
		void ClearDepth(double value);
		void EnableDepthMask();
		void DisableDepthMask();
		void DeleteBuffers(int size, unsigned int* id);
		void EnableClientState(int mode);
		void DisableClientState(int mode);
		void BindBuffer(int mode, unsigned int id);
		void DrawArray(int mode,int firstIndex, int size);
		void GenerateBuffer(int size, unsigned int* id);
		void PolygonMode(int mode, int mode1);
		void PointSize(float size);
		void VertexPointer(int, int, int,const GLvoid*);
		void ColorPointer(int, int, int,const GLvoid*);
		void TexCoordPointer(int, int, int, const GLvoid*);
		void BufferData(int mode, GLsizeiptr size, const GLvoid*, int mode1 );
		void FrontFace(int mode);
		void LineWidth(float size);
		void UseShaderProgram();
		void DisableShaderProgram();
		void UseDefaultShaderProgram();
		void DisableDefaultShaderProgram();
		void RenderText(const Vector2& position,const RGBColor& color, BitmapFont* font, OpenGLShaderProgram* shader, const float& cellHeight, const std::string& myStr);
		void DisableAllTexture();

		float CalcTextWidth(BitmapFont* font, const float& cellHeight, const std::string& myStr);
		void ComputeSurfaceTangentsAtVertex( Vector3& surfaceTangentAtVertex_out,
											  Vector3& surfaceBitangentAtVertex_out,
											  const Vector3& normalAtThisVertex,
											  const Vector3& positionOfThisVertex,
											  const Vector2& texCoordsOfThisVertex,
											  const Vector3& positionOfPreviousAdjacentVertex,
											  const Vector2& texCoordsOfPreviousAdjacentVertex,
											  const Vector3& positionOfNextAdjacentVertex,
											  const Vector2& texCoordsOfNextAdjacentVertex );
		void RenderLightSource();
		void CreateInfinitePointLight( const Vector3& position, const Vector3& direction, const RGBColor& color );
		void CreateAttenuatedPointLight( const Vector3& position, const Vector3& direction, const RGBColor& color, float innerRadius, float outerRadius );
		void CreateAttenuatedSpotLight( const Vector3& position, const Vector3& direction, const RGBColor& color, float innerRadius, float outerRadius, float m_innerApertureDeg, float m_outerApertureDegs );
		void CreateInfiniteSpotLight( const Vector3& position, const Vector3& direction, const RGBColor& color, float m_innerApertureDeg, float m_outerApertureDeg );
		void CreateLocalAmbientLight( const Vector3& position, const Vector3& direction, const RGBColor& color, float ambientNess ); 
		void SetUniformValueOfLightToShader(int shaderProgramID);
		void CreateFBO();
		FBO* GetFBObyID(int id);
		void Draw2DCircle( const Vector2& center, float radius, const RGBColor& color );
		void Draw2DRectangle( const Vector2& bottomLeft, float width, float height, const RGBColor& color );

	private:
		void CreateVBOAndDrawFont(const Vector2& position,const RGBColor& color, BitmapFont* font, OpenGLShaderProgram* shader, const float& cellHeight, const std::string& myStr);
		void InitializeAdvancedOpenGLFunctions();
};

extern GLRender* g_glRender;

extern PFNGLGENBUFFERSPROC					glGenBuffers;
extern PFNGLBINDBUFFERPROC					glBindBuffer;
extern PFNGLBUFFERDATAPROC					glBufferData;
extern PFNGLGENERATEMIPMAPPROC				glGenerateMipmap;
extern PFNGLDELETEBUFFERSPROC				glDeleteBuffers;
extern PFNGLSHADERSOURCEPROC				glShaderSource;
extern PFNGLCOMPILESHADERPROC				glCompileShader;
extern PFNGLGETSHADERIVPROC					glGetShaderiv;
extern PFNGLCREATEPROGRAMPROC				glCreateProgram;
extern PFNGLCREATESHADERPROC				glCreateShader;
extern PFNGLLINKPROGRAMPROC					glLinkProgram;
extern PFNGLGETPROGRAMIVPROC				glGetProgramiv;
extern PFNGLUSEPROGRAMPROC					glUseProgram;
extern PFNGLUNIFORM1FPROC					glUniform1f;
extern PFNGLUNIFORM1IPROC					glUniform1i;
extern PFNGLGETUNIFORMLOCATIONPROC			glGetUniformLocation;
extern PFNGLDELETESHADERPROC				glDeleteShader;
extern PFNGLATTACHSHADERPROC				glAttachShader;
extern PFNGLGETSHADERINFOLOGPROC			glGetShaderInfoLog;
extern PFNGLDELETEPROGRAMPROC				glDeleteProgram;
extern PFNGLACTIVETEXTUREPROC				glActiveTexture;
extern PFNGLBINDATTRIBLOCATIONPROC			glBindAttribLocation;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC		glEnableVertexAttribArray;
extern PFNGLVERTEXATTRIBPOINTERPROC			glVertexAttribPointer;
extern PFNGLGETATTRIBLOCATIONPROC			glGetAttribLocation;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC	glDisableVertexAttribArray;
extern PFNGLUNIFORMMATRIX4FVPROC			glUniformMatrix4fv;
extern PFNGLUNIFORM3FPROC					glUniform3f;
extern PFNGLUNIFORM4FPROC					glUniform4f;
extern PFNGLUNIFORM2FPROC					glUniform2f;
extern PFNGLUNIFORM4FVPROC					glUniform4fv;
extern PFNGLUNIFORM3FVPROC					glUniform3fv;
extern PFNGLUNIFORM2FVPROC					glUniform2fV;
extern PFNGLUNIFORM1FVPROC					glUniform1fv;
extern PFNGLUNIFORM1IVPROC					glUniform1iv;
extern PFNGLBINDFRAMEBUFFERPROC				glBindFramebuffer;
extern PFNGLGENFRAMEBUFFERSPROC				glGenFramebuffers;
extern PFNGLFRAMEBUFFERTEXTURE2DPROC		glFramebufferTexture2D;

#endif