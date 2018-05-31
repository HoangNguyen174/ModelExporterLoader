#ifndef MAIN
#define MAIN
#define WIN32_LEAN_AND_MEAN


#include <math.h>
#include <stdlib.h>
#include <cassert>
#include <crtdbg.h>
#include "../Render/GLRender.hpp"
#include "GameCommon.hpp"
#include "World.hpp"

#pragma comment( lib, "opengl32" ) // Link in the OpenGL32.lib static library
#pragma comment( lib, "glu32")
//-----------------------------------------------------------------------------------------------
#define UNUSED(x) (void)(x);

//-----------------------------------------------------------------------------------------------
bool g_isQuitting = false;							
HWND g_hWnd = nullptr;
HDC g_displayDeviceContext = nullptr;
HGLRC g_openGLRenderingContext = nullptr;
const char* APP_NAME = "SoulStoneEngine";
const int OFFSET_FROM_WINDOW_DESKTOP = 50;
const int WINDOW_PHYSICAL_WIDTH = 1600;
const int WINDOW_PHYSICAL_HEIGHT = 900;
const double VIEW_LEFT = 0.0;
const double VIEW_RIGHT = 1600.0;
const double VIEW_BOTTOM = 0.0;
const double VIEW_TOP = VIEW_RIGHT * static_cast<double>(WINDOW_PHYSICAL_HEIGHT) / static_cast<double>(WINDOW_PHYSICAL_WIDTH);
float lastTimeElapsed = 0.f;
float currTimeElapsed = 0.f;
float elapsedTime = 0.f;

//-----------------------------------------------------------------------------------------------
LRESULT CALLBACK WindowsMessageHandlingProcedure( HWND windowHandle, UINT wmMessageCode, WPARAM wParam, LPARAM lParam )
{
	unsigned char asKey = (unsigned char) wParam;
	switch( wmMessageCode )
	{
		case WM_CLOSE:
		case WM_DESTROY:
		case WM_QUIT:
			g_isQuitting = true;
			return 0;
		case WM_KEYDOWN:
			if( asKey == VK_ESCAPE && !g_theConsole->m_isOpen )
			{
				g_isQuitting = true;
				return 0;
			}
			if(theWorld->ProcessKeyDownEvent(windowHandle,wmMessageCode,wParam,lParam) && !g_theConsole->m_isOpen)
			{
				return 0;
			}
			if(g_theConsole->m_isOpen)
			{
				g_theConsole->ProcessSpecialKeyboard(asKey);
				return 0;
			}

		case WM_KEYUP:
			if(theWorld->ProcessKeyDownEvent(windowHandle,wmMessageCode,wParam,lParam))
			{
				return 0;
			}
			break;
		case WM_CHAR:
			if(g_theConsole->m_isOpen)
			{
				g_theConsole->ProcessKeyboard(asKey);
				return 0;
			}
			break;
		case WM_LBUTTONDOWN:
			break;
		case WM_RBUTTONDOWN:
			break;
	}

	return DefWindowProc( windowHandle, wmMessageCode, wParam, lParam );
}


//-----------------------------------------------------------------------------------------------
void CreateOpenGLWindow( HINSTANCE applicationInstanceHandle )
{
	// Define a window class
	WNDCLASSEX windowClassDescription;
	memset( &windowClassDescription, 0, sizeof( windowClassDescription ) );
	windowClassDescription.cbSize = sizeof( windowClassDescription );
	windowClassDescription.style = CS_OWNDC; // Redraw on move, request own Display Context
	windowClassDescription.lpfnWndProc = static_cast< WNDPROC >( WindowsMessageHandlingProcedure ); // Assign a win32 message-handling function
	windowClassDescription.hInstance = GetModuleHandle( NULL );
	windowClassDescription.hIcon = NULL;
	windowClassDescription.hCursor = NULL;
	windowClassDescription.lpszClassName = TEXT( "Simple Window Class" );
	RegisterClassEx( &windowClassDescription );

	const DWORD windowStyleFlags = WS_CAPTION | WS_BORDER | WS_THICKFRAME | WS_SYSMENU | WS_OVERLAPPED;
	const DWORD windowStyleExFlags = WS_EX_APPWINDOW;

	RECT desktopRect;
	HWND desktopWindowHandle = GetDesktopWindow();
	GetClientRect( desktopWindowHandle, &desktopRect );

	RECT windowRect = { OFFSET_FROM_WINDOW_DESKTOP + 0, OFFSET_FROM_WINDOW_DESKTOP + 0, OFFSET_FROM_WINDOW_DESKTOP + WINDOW_PHYSICAL_WIDTH, 50 + WINDOW_PHYSICAL_HEIGHT };
	AdjustWindowRectEx( &windowRect, windowStyleFlags, FALSE, windowStyleExFlags );

	WCHAR windowTitle[ 1024 ];
	MultiByteToWideChar( GetACP(), 0, APP_NAME, -1, windowTitle, sizeof(windowTitle)/sizeof(windowTitle[0]) );
	g_hWnd = CreateWindowEx(
		windowStyleExFlags,
		windowClassDescription.lpszClassName,
		windowTitle,
		windowStyleFlags,
		windowRect.left,
		windowRect.top,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		applicationInstanceHandle,
		NULL );

	ShowWindow( g_hWnd, SW_SHOW );
	SetForegroundWindow( g_hWnd );
	SetFocus( g_hWnd );
	
	g_displayDeviceContext = GetDC( g_hWnd );
	HCURSOR cursor = LoadCursor( NULL, NULL );
	SetCursor( cursor );

	PIXELFORMATDESCRIPTOR pixelFormatDescriptor;
	memset( &pixelFormatDescriptor, 0, sizeof( pixelFormatDescriptor ) );
	pixelFormatDescriptor.nSize			= sizeof( pixelFormatDescriptor );
	pixelFormatDescriptor.nVersion		= 1;
	pixelFormatDescriptor.dwFlags		= PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pixelFormatDescriptor.iPixelType	= PFD_TYPE_RGBA;
	pixelFormatDescriptor.cColorBits	= 24;
	pixelFormatDescriptor.cDepthBits	= 24;
	pixelFormatDescriptor.cAccumBits	= 0;
	pixelFormatDescriptor.cStencilBits	= 8;

	int pixelFormatCode = ChoosePixelFormat( g_displayDeviceContext, &pixelFormatDescriptor );
	SetPixelFormat( g_displayDeviceContext, pixelFormatCode, &pixelFormatDescriptor );
	g_openGLRenderingContext = wglCreateContext( g_displayDeviceContext );
	wglMakeCurrent( g_displayDeviceContext, g_openGLRenderingContext );
	g_glRender->Enable(GL_LINE_SMOOTH);
	g_glRender->Enable(GL_BLEND);
	g_glRender->BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

//-----------------------------------------------------------------------------------------------
void RunMessagePump()
{
	MSG queuedMessage;
	for( ;; )
	{
		const BOOL wasMessagePresent = PeekMessage( &queuedMessage, NULL, 0, 0, PM_REMOVE );
		if( !wasMessagePresent )
		{
			break;
		}

		TranslateMessage( &queuedMessage );
		DispatchMessage( &queuedMessage );
	}
}

//-----------------------------------------------------------------------------------------------
void Update(float elapsedTime)
{
	theWorld->Update(elapsedTime);
}


//-----------------------------------------------------------------------------------------------
void Render()
{
	g_glRender->ClearColor( 0.f, 0.f, 0.f, 1.f );
	g_glRender->Clear( GL_COLOR_BUFFER_BIT );
	theWorld->Render();
	SwapBuffers( g_displayDeviceContext );
}


//-----------------------------------------------------------------------------------------------
void RunFrame(float elapsedTime)
{
	RunMessagePump();
	Update(elapsedTime);
	Render();
	theWorld->ClearWorld();
}

void Initialize(HINSTANCE applicationInstanceHandle)
{
	CreateOpenGLWindow(applicationInstanceHandle);
	g_glRender = new GLRender();
	g_theConsole = new DeveloperConsole();
	theWorld = new World();
}

void Shutdown()
{
	delete g_glRender;
	delete g_theConsole;
	delete theWorld;
}

//-----------------------------------------------------------------------------------------------
int WINAPI WinMain( HINSTANCE applicationInstanceHandle, HINSTANCE, LPSTR commandLineString, int )
{
	UNUSED( commandLineString );
	Initialize(applicationInstanceHandle);
	Timer time;
	while( !g_isQuitting )	
	{
		currTimeElapsed = static_cast<float>(time.GetTimeSeconds());
		elapsedTime = currTimeElapsed - lastTimeElapsed;
		lastTimeElapsed = currTimeElapsed;
		RunFrame(elapsedTime);
	}

	// #if defined( _WIN32 ) && defined( _DEBUG )
	// 	assert( _CrtCheckMemory() );
	// 	_CrtDumpMemoryLeaks();
	// #endif

	Shutdown();
	return 0;
}
#endif