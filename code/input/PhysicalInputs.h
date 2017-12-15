#ifndef HANDMADE_PHYSICALINPUTS_H
#define HANDMADE_PHYSICALINPUTS_H

#include "../handmade_typedefs.h"

namespace Input
{
	//NOTE(Ian): Matches Windows VK codes for convenience, but is *not* platform-specific code and may include values that Windows does not
	enum PhysicalInputs
	{
		LMB = 0x01,
		RMB,
		Cancel,
		MMB,
		XMB1,
		XMB2,
		//Reserved on Windows
		Backspace = 0x08,
		Tab,
		//Reserved on Windows
		Clear = 0x0C,
		Enter,
		//Reserved on Windows
		Shift = 0x10,
		Control,
		Alt,
		Pause,
		CapsLock,
		IME_Kana,
		//Reserved on Windows
		IME_Junja = 0x17,
		IME_Final,
		IME_Kanji,
		//Reserved on Windows
		Escape = 0x1B,
		IME_Convert,
		IME_NonConvert,
		IME_Accept,
		IME_ModeChange,
		Space,
		PageUp,
		PageDown,
		End,
		Home,
		Left,
		Up,
		Right,
		Down,
		Select,
		Print,
		Execute,
		PrintScreen,
		Insert,
		Delete,
		Help,
		Zero,
		One,
		Two,
		Three,
		Four,
		Five,
		Six,
		Seven,
		Eight,
		Nine,
		//0x3A - 40 Reserved on Windows
		A = 0x41,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,
		LeftWindows, //TODO(Ian): Find out what these are on other OSs and rename them to match
		RightWindows,
		Apps,
		//Reserved on Windows
		Sleep = 0x5F,
		Numpad0,
		Numpad1,
		Numpad2,
		Numpad3,
		Numpad4,
		Numpad5,
		Numpad6,
		Numpad7,
		Numpad8,
		Numpad9,
		Multiply,
		Add,
		Seperator,
		Subtract,
		Divide,
		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		F13,
		F14,
		F15,
		F16,
		F17,
		F18,
		F19,
		F20,
		F21,
		F22,
		F23,
		F24,
		//0x88 - 8F Reserved on Windows
		NumLock = 0x90,
		ScrollLock,
		//0x92 - 96 OEM Specific
		//0x97 - 9F Reserved on Windows
		LeftShift = 0xA0,
		RightShift,
		LeftControl,
		RightControl,
		LeftMenu, //TODO(Ian): Is this the Alt key?
		RightMenu,
		Browser_Back,
		Browser_Forward,
		Browser_Refresh,
		Browser_Stop,
		Browser_Search,
		Browser_Favorites,
		Browser_Home,
		Volume_Mute,
		Volume_Up,
		Volume_Down,
		Media_NextTrack,
		Media_PreviousTrack,
		Media_Stop,
		Media_PlayPause,
		Launch_Mail,
		Launch_MediaSelect,
		Launch_App1,
		Launch_App2,
		//0xB8 - B9 Reserved on Windows
		OEM_1 = 0xBA, //Varies, ; and : on US standard keyboard
		OEM_Plus,
		OEM_Comma,
		OEM_Minus,
		OEM_Period,
		OEM_2, // / and ? on US standard keyboard
		OEM_3, // ` and ~ on US standard keyboard
		//0xC1 - DA Reserved on Windows
		OEM_4 = 0xDB, // [ and { on US standard keyboard
		OEM_5, // \ and | on US standard keyboard
		OEM_6, // ] and } on US standard keyboard
		OEM_7, // ' and " on US standard keyboard
		OEM_8, // Varies by US standard keyboard
		//0xE0 Reserved on Windows
		//0xE1 OEM specific, no VK code specified
		OEM_10_2 = 0xE2,
		//0xE3 - E4 OEM specific, no VK code specified
		IME_Process = 0xE5,
		//0xE6 OEM specific, no VK code specified
		Packet,
		//0xE8 Reserved on Windows
		//0xE9 - F5 OEM specific, no VK code specified
		Attention = 0xF6,
		CrSel,
		ExSel,
		EraseEOF,
		Play,
		Zoom,
		NoName,
		PA1,
		OEM_Clear,

		PHYSICALINPUTS_LAST
	};
}

#endif