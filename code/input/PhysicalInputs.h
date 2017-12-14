#ifndef HANDMADE_PHYSICALINPUTS_H
#define HANDMADE_PHYSICALINPUTS_H

#include "../handmade_typedefs.h"

namespace Input
{
	//NOTE(Ian): Matches Windows VK codes for convenience, but is *not* platform-specific code and may include values that Windows does not
	enum PhysicalInputs
	{
		LMB = 1,
		RMB,
		CANCEL,
		MMB,
		XMB1,
		XMB2,
		//Reserved on Windows
		BACKSPACE = 8,
		TAB,
		//Reserved on Windows
		CLEAR = 10,
		ENTER,
		//Reserved on Windows
		SHIFT = 12,
		CONTROL,
		ALT,
		PAUSE,
		CAPSLOCK,
		IMEKANA,
		//Reserved on Windows
		IMEJUNJA = 18,
		IMEFINAL,
		IMEKANJI,
		//Reserved on Windows
		ESCAPE = 21,


		PHYSICALINPUTS_LAST
	};
}

#endif