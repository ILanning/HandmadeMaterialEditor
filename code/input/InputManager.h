#ifndef HANDMADE_INPUTMANAGER_H
#define HANDMADE_INPUTMANAGER_H

#include "../handmade_typedefs.h"
#include "PhysicalInputs.h"
#include "InputFrame.h"
#include "../math/Vector2.h"

namespace Input
{
	struct InputManager
	{
		//TODO(Ian): Keep a set of frames per registered controller
		InputFrame[2] frames;
		InputFrame& OldFrame = &frames[1];
		InputFrame& NewFrame = &frames[0];

		void HandleInput(InputFrame *nextFrame)
		{

		}

		bool IsReleased(PhysicalInputs input)
		{

		}
		/*                  PLATFORM LAYER
		case WM_MOUSEMOVE:                              //With pointer ballistics, write a custom function for this later to make it platform independent
    {
        int xPosAbsolute = GET_X_PARAM(lParam); 
        int yPosAbsolute = GET_Y_PARAM(lParam);
        // ...
        break;
    }
		
		
		#ifndef HID_USAGE_PAGE_GENERIC                  //Without, not really needed for this project
    #define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
    #endif
    #ifndef HID_USAGE_GENERIC_MOUSE
    #define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
    #endif

    RAWINPUTDEVICE Rid[1];
    Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC; 
    Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE; 
    Rid[0].dwFlags = RIDEV_INPUTSINK;   
    Rid[0].hwndTarget = hWnd;
    RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]);

	...

	case WM_INPUT:
	{
	UINT dwSize = 40;
	static BYTE lpb[40];

	GetRawInputData((HRAWINPUT)lParam, RID_INPUT,
	lpb, &dwSize, sizeof(RAWINPUTHEADER));

	RAWINPUT* raw = (RAWINPUT*)lpb;

	if (raw->header.dwType == RIM_TYPEMOUSE)
	{
	int xPosRelative = raw->data.mouse.lLastX;
	int yPosRelative = raw->data.mouse.lLastY;
	}
	break;
	}
	//https://msdn.microsoft.com/en-us/library/windows/desktop/ms645536(v=vs.85).aspx
	//https://msdn.microsoft.com/en-us/library/windows/desktop/ms645546(v=vs.85).aspx

		*/



	};
}

#endif