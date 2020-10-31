#pragma once
#include <Common/Util/Error.h>
#include <deque>

namespace Backend
{
	namespace Event
	{
		//Event config
		struct Config
		{
			
		};
		
		//Input enums
		enum InputCode
		{
			InputCode_Unknown,
			
			//Letters
			InputCode_A,
			InputCode_B,
			InputCode_C,
			InputCode_D,
			InputCode_E,
			InputCode_F,
			InputCode_G,
			InputCode_H,
			InputCode_I,
			InputCode_J,
			InputCode_K,
			InputCode_L,
			InputCode_M,
			InputCode_N,
			InputCode_O,
			InputCode_P,
			InputCode_Q,
			InputCode_R,
			InputCode_S,
			InputCode_T,
			InputCode_U,
			InputCode_V,
			InputCode_W,
			InputCode_X,
			InputCode_Y,
			InputCode_Z,
			
			//Numbers
			InputCode_1,
			InputCode_2,
			InputCode_3,
			InputCode_4,
			InputCode_5,
			InputCode_6,
			InputCode_7,
			InputCode_8,
			InputCode_9,
			InputCode_0,
			
			//Whitespace and escape
			InputCode_Return,
			InputCode_Escape,
			InputCode_Backspace,
			InputCode_Tab,
			InputCode_Space,
			
			//Symbols
			InputCode_Minus,
			InputCode_Equals,
			InputCode_LeftBracket,
			InputCode_RightBracket,
			InputCode_Backslash,
			InputCode_NonUSHash,
			InputCode_SemiColon,
			InputCode_Apostrophe,
			InputCode_Grave,
			InputCode_Comma,
			InputCode_Period,
			InputCode_Slash,
			
			//Caps lock
			InputCode_CapsLock,
			
			//Function keys
			InputCode_F1,
			InputCode_F2,
			InputCode_F3,
			InputCode_F4,
			InputCode_F5,
			InputCode_F6,
			InputCode_F7,
			InputCode_F8,
			InputCode_F9,
			InputCode_F10,
			InputCode_F11,
			InputCode_F12,
			
			//Control keys
			InputCode_PrintScreen,
			InputCode_ScrollLock,
			InputCode_Pause,
			InputCode_Insert,
			InputCode_Home,
			InputCode_PageUp,
			InputCode_Delete,
			InputCode_End,
			InputCode_PageDown,
			
			//Arrow keys
			InputCode_Right,
			InputCode_Left,
			InputCode_Down,
			InputCode_Up,
			
			//Keypad
			InputCode_NumLockClear,
			InputCode_KP_Divide,
			InputCode_KP_Multiply,
			InputCode_KP_Minus,
			InputCode_KP_Plus,
			InputCode_KP_Enter,
			InputCode_KP_1,
			InputCode_KP_2,
			InputCode_KP_3,
			InputCode_KP_4,
			InputCode_KP_5,
			InputCode_KP_6,
			InputCode_KP_7,
			InputCode_KP_8,
			InputCode_KP_9,
			InputCode_KP_0,
			InputCode_KP_Period,
			
			//Some USB standard stuff
			InputCode_NonUSBackslash,
			InputCode_Application,
			InputCode_Power,
			InputCode_KP_Equals,
			InputCode_F13,
			InputCode_F14,
			InputCode_F15,
			InputCode_F16,
			InputCode_F17,
			InputCode_F18,
			InputCode_F19,
			InputCode_F20,
			InputCode_F21,
			InputCode_F22,
			InputCode_F23,
			InputCode_F24,
			InputCode_Execute,
			InputCode_Help,
			InputCode_Menu,
			InputCode_Select,
			InputCode_Stop,
			InputCode_Again,
			InputCode_Undo,
			InputCode_Cut,
			InputCode_Copy,
			InputCode_Paste,
			InputCode_Find,
			InputCode_Mute,
			InputCode_VolumeUp,
			InputCode_VolumeDown,
			InputCode_KP_Comma,
			InputCode_KP_EqualsAS400,
			
			InputCode_International1,
			InputCode_International2,
			InputCode_International3,
			InputCode_International4,
			InputCode_International5,
			InputCode_International6,
			InputCode_International7,
			InputCode_International8,
			InputCode_International9,
			InputCode_Lang1,
			InputCode_Lang2,
			InputCode_Lang3,
			InputCode_Lang4,
			InputCode_Lang5,
			InputCode_Lang6,
			InputCode_Lang7,
			InputCode_Lang8,
			InputCode_Lang9,
			
			InputCode_AltErase,
			InputCode_SysReq,
			InputCode_Cancel,
			InputCode_Clear,
			InputCode_Prior,
			InputCode_Return2,
			InputCode_Separator,
			InputCode_Out,
			InputCode_Oper,
			InputCode_ClearAgain,
			InputCode_CRSel,
			InputCode_EXSel,
			
			InputCode_KP_00,
			InputCode_KP_000,
			InputCode_ThousandsSeparator,
			InputCode_DecimalSeparator,
			InputCode_CurrencyUnit,
			InputCode_CurrencySubUnit,
			InputCode_KP_LeftParen,
			InputCode_KP_RightParen,
			InputCode_KP_LeftBrace,
			InputCode_KP_RightBrace,
			InputCode_KP_Tab,
			InputCode_KP_Backspace,
			InputCode_KP_A,
			InputCode_KP_B,
			InputCode_KP_C,
			InputCode_KP_D,
			InputCode_KP_E,
			InputCode_KP_F,
			InputCode_KP_XOR,
			InputCode_KP_Power,
			InputCode_KP_Percent,
			InputCode_KP_Less,
			InputCode_KP_Greater,
			InputCode_KP_Ampersand,
			InputCode_KP_DBLAmpersand,
			InputCode_KP_VerticalBar,
			InputCode_KP_DBLVerticalBar,
			InputCode_KP_Colon,
			InputCode_KP_Hash,
			InputCode_KP_Space,
			InputCode_KP_At,
			InputCode_KP_Exclam,
			InputCode_KP_MemStore,
			InputCode_KP_MemRecall,
			InputCode_KP_MemClear,
			InputCode_KP_MemAdd,
			InputCode_KP_MemSubtract,
			InputCode_KP_MemMultiply,
			InputCode_KP_MemDivide,
			InputCode_KP_PlusMinus,
			InputCode_KP_Clear,
			InputCode_KP_ClearEntry,
			InputCode_KP_Binary,
			InputCode_KP_Octal,
			InputCode_KP_Decimal,
			InputCode_KP_Hexadecimal,
			
			InputCode_LCtrl,
			InputCode_LShift,
			InputCode_LAlt,
			InputCode_LGui,
			InputCode_RCtrl,
			InputCode_RShift,
			InputCode_RAlt,
			InputCode_RGui,
			
			InputCode_Mode,
			
			InputCode_AudioNext,
			InputCode_AudioPrev,
			InputCode_AudioStop,
			InputCode_AudioPlay,
			InputCode_AudioMute,
			InputCode_MediaSelect,
			InputCode_WWW,
			InputCode_Mail,
			InputCode_Calculator,
			InputCode_Computer,
			InputCode_AC_Search,
			InputCode_AC_Home,
			InputCode_AC_Back,
			InputCode_AC_Forward,
			InputCode_AC_Stop,
			InputCode_AC_Refresh,
			InputCode_AC_Bookmarks,
			
			InputCode_BrightnessDown,
			InputCode_BrightnessUp,
			InputCode_DisplaySwitch,
			
			InputCode_KBDIllumToggle,
			InputCode_KBDIllumDown,
			InputCode_KBDIllumUp,
			InputCode_Eject,
			InputCode_Sleep,
			
			InputCode_App1,
			InputCode_App2,
			InputCode_AudioRewind,
			InputCode_AudioFastForward,
			
			//Face buttons
			InputCode_Gamepad_A,
			InputCode_Gamepad_B,
			InputCode_Gamepad_X,
			InputCode_Gamepad_Y,
			
			//Middle buttons
			InputCode_Gamepad_Back,
			InputCode_Gamepad_Guide,
			InputCode_Gamepad_Start,
			
			//Left and right analogue sticks
			InputCode_Gamepad_LeftStick,
			InputCode_Gamepad_RightStick,
			
			//Shoulder buttons
			InputCode_Gamepad_LeftShoulder,
			InputCode_Gamepad_RightShoulder,
			
			//DPad
			InputCode_Gamepad_Up,
			InputCode_Gamepad_Down,
			InputCode_Gamepad_Left,
			InputCode_Gamepad_Right,
			
			//Analogue sticks
			InputCode_Gamepad_LeftStick_X,
			InputCode_Gamepad_LeftStick_Y,
			InputCode_Gamepad_RightStick_X,
			InputCode_Gamepad_RightStick_Y,
			
			//Analogue triggers
			InputCode_Gamepad_LeftTrigger,
			InputCode_Gamepad_RightTrigger,
			
			//Mouse
			InputCode_Mouse_X,
			InputCode_Mouse_Y,
			InputCode_Mouse_Left,
			InputCode_Mouse_Middle,
			InputCode_Mouse_Right,
			InputCode_Mouse_X1,
			InputCode_Mouse_X2,
			InputCode_Mouse_ScrollX,
			InputCode_Mouse_ScrollY,
			
			InputCode_Num,
		};
		
		enum InputDevice
		{
			InputDevice_Keyboard,
			InputDevice_Mouse,
			InputDevice_Gamepad1,
			InputDevice_Gamepad2,
			InputDevice_Gamepad3,
			InputDevice_Gamepad4,
			InputDevice_Gamepad5,
			InputDevice_Gamepad6,
			InputDevice_Gamepad7,
			InputDevice_Gamepad8,
		};
		
		//Event types
		enum EventType
		{
			EventType_Unknown,
			EventType_Quit,
			EventType_InputBool,
			EventType_InputFloat,
		};
		
		struct EventData_InputBool
		{
			InputDevice device;
			InputCode code;
			bool value;
		};
		
		struct EventData_InputFloat
		{
			InputDevice device;
			InputCode code;
			float value, rel_value;
		};
		
		struct EventData
		{
			//Event type
			EventType type;
			
			//Event specific information
			union
			{
				EventData_InputBool input_bool;
				EventData_InputFloat input_float;
			};
		};
		
		//Event base class
		class Event
		{
			protected:
				//Error
				Error error;
				
				//Event queue
				std::deque<EventData> event_queue;
				
			public:
				//Virtual destructor
				virtual ~Event() {}
				
				//Event interface
				virtual bool SetConfig(const Config config) = 0;
				
				virtual bool PollEvent(EventData &event_data) = 0;
				
				//Get error
				const Error &GetError() const { return error; }
		};
	}
}
