#ifndef HANDMADE_WIN32_INPUTPROCESSOR
#define HANDMADE_WIN32_INPUTPROCESSOR

#include "../handmade_typedefs.h"
#include "../input/PhysicalInputs.h"
#include "../input/InputFrame.h"
#include <Strsafe.h>

/*
Needs to create InputFrames for the InputManager
Eventually will also handle keeping track of which devices are and aren't plugged in
*/
struct InputProcessor
{
	Input::InputFrame nextFrame;

	void RegisterRawInput()
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

	void HandleRawInputMessages(MSG message)
	{
		//TODO(Ian): Handle VK_Packet properly
		UINT dwSize;

		GetRawInputData((HRAWINPUT)message.lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
		LPBYTE lpb = new BYTE[dwSize];
		if (lpb == NULL)
		{
			return;
		}

		if (GetRawInputData((HRAWINPUT)message.lParam, RID_INPUT, lpb, &dwSize,	sizeof(RAWINPUTHEADER)) != dwSize)
			OutputDebugString(TEXT("GetRawInputData does not return correct size !\n"));

		RAWINPUT* raw = (RAWINPUT*)lpb;
		HRESULT hResult;
		wchar_t *charArray[100];
		STRSAFE_LPSTR szTempOutput = (STRSAFE_LPSTR)(void *)charArray;

		if (raw->header.dwType == RIM_TYPEKEYBOARD)
		{
			
			hResult = StringCchPrintf(szTempOutput, STRSAFE_MAX_CCH, TEXT(" Kbd: make=%04x Flags:%04x Reserved:%04x ExtraInformation:%08x, msg=%04x VK=%04x \n"),
				raw->data.keyboard.MakeCode,
				raw->data.keyboard.Flags,
				raw->data.keyboard.Reserved,
				raw->data.keyboard.ExtraInformation,
				raw->data.keyboard.Message,
				raw->data.keyboard.VKey);
			if (FAILED(hResult))
			{
				// TODO: write error handler
			}
			OutputDebugString(szTempOutput);
			nextFrame.SetKey((Input::PhysicalInputs)raw->data.keyboard.VKey, (raw->data.keyboard.Flags & RI_KEY_BREAK) == 0);
			if (nextFrame.GetKey(Input::PhysicalInputs::Right))
			{
				int b = 0;
				b++;
			}
		}
		else if (raw->header.dwType == RIM_TYPEMOUSE)
		{
			RAWMOUSE& mouse = raw->data.mouse;

			if ((mouse.usFlags & MOUSE_MOVE_ABSOLUTE) == true)
			{
				nextFrame.MousePos = { (real32)mouse.lLastX, (real32)mouse.lLastY };
			}
			else
			{
				nextFrame.MousePos += { (real32)mouse.lLastX, (real32)mouse.lLastY };
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
				//TODO(Ian):Fix this
				//nextFrame.scrollWheelChange = reinterpret_cast<int16>(mouse.usButtonData);
			}


			/*hResult = StringCchPrintf(szTempOutput, STRSAFE_MAX_CCH, TEXT("Mouse: usFlags=%04x ulButtons=%04x usButtonFlags=%04x usButtonData=%04x ulRawButtons=%04x lLastX=%04x lLastY=%04x ulExtraInformation=%04x\r\n"),
				raw->data.mouse.usFlags,
				raw->data.mouse.ulButtons,
				raw->data.mouse.usButtonFlags,
				raw->data.mouse.usButtonData,
				raw->data.mouse.ulRawButtons,
				raw->data.mouse.lLastX,
				raw->data.mouse.lLastY,
				raw->data.mouse.ulExtraInformation);

			if (FAILED(hResult))
			{
				// TODO: write error handler
			}
			OutputDebugString(szTempOutput);*/
		}

		delete[] lpb;
		//delete[] szTempOutput;
	}

	//TODO(Ian): Have this accept timing information in support of frametime-invariant updating
	Input::InputFrame GenerateFrame()
	{
		return nextFrame;
	}
};

#endif