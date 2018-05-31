#ifndef XBOXCONTROLLER_HEADER
#define XBOXCONTROLLER_HEADER
#include <stdio.h>
#include <Windows.h>
#include <Xinput.h>
#include <math.h>
#include "Vector2.hpp"
#pragma comment( lib, "xinput" )

class XBoxController
{
	private:
		XINPUT_STATE XboxControllerState;
		int controllerNumber;

	public:
		float DEAD_ZONE;
		float JOYSTICKMAXVAL;
		bool IsButtonADownCurrentFrame;
		bool IsButtonADownLastFrame;
		bool IsButtonAPressed;
		bool IsButtonDownLastFrame;
		bool IsButtonDownCurrentFrame ;
		bool IsButtonPressed ;

	public:
		XBoxController(int playerNumber);
		~XBoxController();
		bool IsConnected();
		XINPUT_STATE GetState();
		void Vibrate(float left,float right);
		float GetJoyStickMaxLength();
		Vector2 GetLeftJoyStickCoordinate();
		Vector2 GetRightJoyStickCoordinate();
		bool IsRightTriggerPressed();
		bool IsRightTriggerDown();
		bool IsButtonPressedCurrentFrame(char button);
		bool IsButtonDown(DWORD button);
};
#endif;