#ifndef HANDMADE_WIN32_INPUTPROCESSOR_CPP
#define HANDMADE_WIN32_INPUTPROCESSOR_CPP

#include "InputProcessor.h"
#include "../handmade_typedefs.h"
#include "../input/PhysicalInputs.h"
#include "../input/InputFrame.h"
#include <Strsafe.h>

void InputProcessor::RegisterRawInput()
{
	RAWINPUTDEVICE Rid[2];

	//HID Mouse
	Rid[0].usUsagePage = 0x01;
	Rid[0].usUsage = 0x02;
	Rid[0].dwFlags = 0;
	Rid[0].hwndTarget = 0;

	//HID Keyboard
	Rid[1].usUsagePage = 0x01;
	Rid[1].usUsage = 0x06;
	Rid[1].dwFlags = 0;
	Rid[1].hwndTarget = 0;

	if (RegisterRawInputDevices(Rid, 2, sizeof(Rid[0])) == FALSE) {
		//registration failed. Call GetLastError for the cause of the error
	}
}

void InputProcessor::HandleRawInputMessages(MSG message)
{
	//TODO: Handle VK_Packet properly
	UINT dwSize;

	GetRawInputData((HRAWINPUT)message.lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
	LPBYTE lpb = new BYTE[dwSize];
	if (lpb == NULL)
	{
		return;
	}

	if (GetRawInputData((HRAWINPUT)message.lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
		OutputDebugString(TEXT("GetRawInputData does not return correct size !\n"));

	RAWINPUT* raw = (RAWINPUT*)lpb;

	if (raw->header.dwType == RIM_TYPEKEYBOARD)
	{
		nextFrame.SetKey((Input::PhysicalInputs)raw->data.keyboard.VKey, (raw->data.keyboard.Flags & RI_KEY_BREAK) == 0);
	}
	else if (raw->header.dwType == RIM_TYPEMOUSE)
	{
		RAWMOUSE& mouse = raw->data.mouse;

		if ((mouse.usFlags & MOUSE_MOVE_ABSOLUTE) == true) //TODO: Appears to never be true?  Test with tablet input
		{
			nextFrame.MousePos = { (real32)mouse.lLastX, (real32)mouse.lLastY };
		}
		else if (mouse.lLastX != 0 && mouse.lLastY != 0)
		{
			nextFrame.TrueMouseMovement = { (real32)mouse.lLastX, (real32)mouse.lLastY };
		}

		if ((mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN) != 0)
		{
			nextFrame.SetKey(Input::PhysicalInputs::LMB, true);
		}
		if ((mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP) != 0)
		{
			nextFrame.SetKey(Input::PhysicalInputs::LMB, false);
		}

		if ((mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN) != 0)
		{
			nextFrame.SetKey(Input::PhysicalInputs::MMB, true);
		}
		if ((mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_UP) != 0)
		{
			nextFrame.SetKey(Input::PhysicalInputs::MMB, false);
		}

		if ((mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN) != 0)
		{
			nextFrame.SetKey(Input::PhysicalInputs::RMB, true);
		}
		if ((mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP) != 0)
		{
			nextFrame.SetKey(Input::PhysicalInputs::RMB, false);
		}

		if ((mouse.usButtonFlags & RI_MOUSE_BUTTON_4_DOWN) != 0)
		{
			nextFrame.SetKey(Input::PhysicalInputs::XMB1, true);
		}
		if ((mouse.usButtonFlags & RI_MOUSE_BUTTON_4_UP) != 0)
		{
			nextFrame.SetKey(Input::PhysicalInputs::XMB1, false);
		}

		if ((mouse.usButtonFlags & RI_MOUSE_BUTTON_5_DOWN) != 0)
		{
			nextFrame.SetKey(Input::PhysicalInputs::XMB2, true);
		}
		if ((mouse.usButtonFlags & RI_MOUSE_BUTTON_5_DOWN) != 0)
		{
			nextFrame.SetKey(Input::PhysicalInputs::XMB2, false);
		}


		if ((mouse.usButtonFlags & RI_MOUSE_WHEEL) != 0)
		{
			int16 correctedScroll = mouse.usButtonData & MaxInt16;
			if (mouse.usButtonData > MaxInt16)
			{
				correctedScroll = -correctedScroll;
			}
			nextFrame.scrollWheelChange = correctedScroll;
		}
	}

	delete[] lpb;
}

#endif //HANDMADE_WIN32_INPUTPROCESSOR_CPP