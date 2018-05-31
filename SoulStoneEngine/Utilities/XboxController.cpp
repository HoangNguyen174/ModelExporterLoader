#include "XboxController.hpp"

XBoxController::XBoxController(int playerNumber)
{
	DEAD_ZONE = 15000.f;
	JOYSTICKMAXVAL = 32767.f;
	controllerNumber = playerNumber;//support only one player
	IsButtonADownLastFrame = false;
	IsButtonADownCurrentFrame = false;
	IsButtonAPressed = false;
	IsButtonDownLastFrame = false;
	IsButtonDownCurrentFrame = false;
	IsButtonPressed = false;
}

XBoxController::~XBoxController()
{

}

bool XBoxController::IsConnected()
{
	memset(&XboxControllerState,0,sizeof(XboxControllerState));
	DWORD errorStatus = XInputGetState(controllerNumber,&XboxControllerState);
	if(errorStatus == ERROR_SUCCESS)
	{
		return true;
	}
	else 
		return false;
}

void XBoxController::Vibrate(float left,float right)
{
	XINPUT_VIBRATION vibration;

	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));

	vibration.wLeftMotorSpeed = (WORD)left;
	vibration.wRightMotorSpeed = (WORD)right;

	XInputSetState(controllerNumber,&vibration);

}

float XBoxController::GetJoyStickMaxLength()
{
	return sqrt(JOYSTICKMAXVAL * JOYSTICKMAXVAL);
}

XINPUT_STATE XBoxController::GetState()
{
	ZeroMemory(&XboxControllerState,sizeof(XINPUT_STATE));

	XInputGetState(controllerNumber, &XboxControllerState);

	return XboxControllerState;
}

bool XBoxController::IsRightTriggerDown()
{
	if(XboxControllerState.Gamepad.bRightTrigger > 0)
	{
		IsButtonDownCurrentFrame = true;
	}
	else
	{
		IsButtonDownCurrentFrame = false;
	}
	return IsButtonDownCurrentFrame;
}

bool XBoxController::IsButtonDown(DWORD button)
{
	if(XboxControllerState.Gamepad.wButtons & button)
	{
		IsButtonDownCurrentFrame = true;
	}
	else
		IsButtonDownCurrentFrame = false;

	return IsButtonDownCurrentFrame;
}

bool XBoxController::IsButtonPressedCurrentFrame(char button)
{
	
	if(IsButtonDown(button))
	{
		if(IsButtonDownCurrentFrame == IsButtonDownLastFrame)
		{
			IsButtonPressed = false;
		}
		else
		{
			IsButtonPressed = true;
		}
	}
	IsButtonDownLastFrame = IsButtonDownCurrentFrame;
	return IsButtonPressed;
}

bool XBoxController::IsRightTriggerPressed()
{
	if(IsRightTriggerDown())
	{
		if(IsButtonDownCurrentFrame == IsButtonDownLastFrame)
		{
			IsButtonPressed = false;
		}
		else
		{
			IsButtonPressed = true;
		}
	}
	IsButtonDownLastFrame = IsButtonDownCurrentFrame;
	return IsButtonPressed;
}

// Vector2  XBoxController::GetLeftJoyStickCoordinate()
// {
// 	return Vector2(XboxControllerState.Gamepad.bLeftTrigger.sThumbLX,XboxControllerState.Gamepad.bLeftTrigger.sThumbLY);
// }
// 
// Vector2 XBoxController::GetRightJoyStickCoordinate()
// {
// 	return Vector2(XboxControllerState.Gamepad.bRightTrigger.X,XboxControllerState.Gamepad.bRightTrigger.Y);
// }
