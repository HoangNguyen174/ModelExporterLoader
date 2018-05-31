#ifndef GAMECOMMON_HEADER
#define GAMECOMMON_HEADER

#include <windows.h>
#include <stdlib.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <stack>
#include <map>
#include <climits>
#include "strutil.h"
#include "Timer.h"
#include "Vector2.hpp"
#include "Vector2i.hpp"
#include "Vector3.h"
#include "Vector3i.hpp"
#include "../Render/glext.h"
#include "../Render/GLRender.hpp"
#include "Camera3D.hpp"
#include "RGBColor.hpp"
#include "Texture.hpp"
#include "../Debugger/DeveloperConsole.hpp"
#include "Time.hpp"

using namespace std;

struct Bone
{
	int			m_index;
	float		m_weight;

};

struct Vertex3D
{
	Vector3				m_position;
	RGBColor			m_color;
	Vector2				m_texCoords;
	Vector3				m_normal;
	Vector3				m_tangent;
	Vector3				m_bitangent;
	int					m_boneIndex[4];
	float				m_boneWeight[4];
	float				m_isStatic;
};

//-----------------------------------------------------------------------------------------------
//global variable
extern DeveloperConsole* g_theConsole;
extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;
//-----------------------------------------------------------------------------------------------

const std::string Stringf( const char* format, ... );
const std::string Stringf( const int maxLength, const char* format, ... );
void DebuggerPrintf( const char* messageFormat, ... );


#endif