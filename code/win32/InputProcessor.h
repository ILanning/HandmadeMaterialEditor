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

		Rid[0].usUsagePage = 0x01;
		Rid[0].usUsage = 0x02;
		Rid[0].dwFlags = 0;// RIDEV_NOLEGACY;   // adds HID mouse and also ignores legacy mouse messages
		Rid[0].hwndTarget = 0;

		Rid[1].usUsagePage = 0x01;
		Rid[1].usUsage = 0x06;
		Rid[1].dwFlags = 0;// RIDEV_NOLEGACY;   // adds HID keyboard and also ignores legacy keyboard messages
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
			nextFrame.SetKey((Input::PhysicalInputs)raw->data.keyboard.VKey, raw->data.keyboard.Flags == RI_KEY_MAKE);
		}
		else if (raw->header.dwType == RIM_TYPEMOUSE)
		{
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
		Input::InputFrame temp = nextFrame;
		nextFrame = {};
		return temp;
	}
};

#endif