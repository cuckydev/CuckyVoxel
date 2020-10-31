#include <SDL_events.h>
#include "Event.h"
#include <unordered_map>

namespace Backend
{
	namespace Event
	{
		//Input code mappings
		static std::unordered_map<SDL_Scancode, InputCode> map_scancode_inputcode = {
			{SDL_SCANCODE_UNKNOWN, InputCode_Unknown},
			
			//Letters
			{SDL_SCANCODE_A, InputCode_A},
			{SDL_SCANCODE_B, InputCode_B},
			{SDL_SCANCODE_C, InputCode_C},
			{SDL_SCANCODE_D, InputCode_D},
			{SDL_SCANCODE_E, InputCode_E},
			{SDL_SCANCODE_F, InputCode_F},
			{SDL_SCANCODE_G, InputCode_G},
			{SDL_SCANCODE_H, InputCode_H},
			{SDL_SCANCODE_I, InputCode_I},
			{SDL_SCANCODE_J, InputCode_J},
			{SDL_SCANCODE_K, InputCode_K},
			{SDL_SCANCODE_L, InputCode_L},
			{SDL_SCANCODE_M, InputCode_M},
			{SDL_SCANCODE_N, InputCode_N},
			{SDL_SCANCODE_O, InputCode_O},
			{SDL_SCANCODE_P, InputCode_P},
			{SDL_SCANCODE_Q, InputCode_Q},
			{SDL_SCANCODE_R, InputCode_R},
			{SDL_SCANCODE_S, InputCode_S},
			{SDL_SCANCODE_T, InputCode_T},
			{SDL_SCANCODE_U, InputCode_U},
			{SDL_SCANCODE_V, InputCode_V},
			{SDL_SCANCODE_W, InputCode_W},
			{SDL_SCANCODE_X, InputCode_X},
			{SDL_SCANCODE_Y, InputCode_Y},
			{SDL_SCANCODE_Z, InputCode_Z},
			
			//Numbers
			{SDL_SCANCODE_1, InputCode_1},
			{SDL_SCANCODE_2, InputCode_2},
			{SDL_SCANCODE_3, InputCode_3},
			{SDL_SCANCODE_4, InputCode_4},
			{SDL_SCANCODE_5, InputCode_5},
			{SDL_SCANCODE_6, InputCode_6},
			{SDL_SCANCODE_7, InputCode_7},
			{SDL_SCANCODE_8, InputCode_8},
			{SDL_SCANCODE_9, InputCode_9},
			{SDL_SCANCODE_0, InputCode_0},
			
			//Whitespace and escape
			{SDL_SCANCODE_RETURN, InputCode_Return},
			{SDL_SCANCODE_ESCAPE, InputCode_Escape},
			{SDL_SCANCODE_BACKSPACE, InputCode_Backspace},
			{SDL_SCANCODE_TAB, InputCode_Tab},
			{SDL_SCANCODE_SPACE, InputCode_Space},
			
			//Symbols
			{SDL_SCANCODE_MINUS, InputCode_Minus},
			{SDL_SCANCODE_EQUALS, InputCode_Equals},
			{SDL_SCANCODE_LEFTBRACKET, InputCode_LeftBracket},
			{SDL_SCANCODE_RIGHTBRACKET, InputCode_RightBracket},
			{SDL_SCANCODE_BACKSLASH, InputCode_Backslash},
			{SDL_SCANCODE_NONUSHASH, InputCode_NonUSHash},
			{SDL_SCANCODE_SEMICOLON, InputCode_SemiColon},
			{SDL_SCANCODE_APOSTROPHE, InputCode_Apostrophe},
			{SDL_SCANCODE_GRAVE, InputCode_Grave},
			{SDL_SCANCODE_COMMA, InputCode_Comma},
			{SDL_SCANCODE_PERIOD, InputCode_Period},
			{SDL_SCANCODE_SLASH, InputCode_Slash},
			
			//Caps lock
			{SDL_SCANCODE_CAPSLOCK, InputCode_CapsLock},
			
			//Function keys
			{SDL_SCANCODE_F1, InputCode_F1},
			{SDL_SCANCODE_F2, InputCode_F2},
			{SDL_SCANCODE_F3, InputCode_F3},
			{SDL_SCANCODE_F4, InputCode_F4},
			{SDL_SCANCODE_F5, InputCode_F5},
			{SDL_SCANCODE_F6, InputCode_F6},
			{SDL_SCANCODE_F7, InputCode_F7},
			{SDL_SCANCODE_F8, InputCode_F8},
			{SDL_SCANCODE_F9, InputCode_F9},
			{SDL_SCANCODE_F10, InputCode_F10},
			{SDL_SCANCODE_F11, InputCode_F11},
			{SDL_SCANCODE_F12, InputCode_F12},
			
			//Control keys
			{SDL_SCANCODE_PRINTSCREEN, InputCode_PrintScreen},
			{SDL_SCANCODE_SCROLLLOCK, InputCode_ScrollLock},
			{SDL_SCANCODE_PAUSE, InputCode_Pause},
			{SDL_SCANCODE_INSERT, InputCode_Insert},
			{SDL_SCANCODE_HOME, InputCode_Home},
			{SDL_SCANCODE_PAGEUP, InputCode_PageUp},
			{SDL_SCANCODE_DELETE, InputCode_Delete},
			{SDL_SCANCODE_END, InputCode_End},
			{SDL_SCANCODE_PAGEDOWN, InputCode_PageDown},
			
			//Arrow keys
			{SDL_SCANCODE_RIGHT, InputCode_Right},
			{SDL_SCANCODE_LEFT, InputCode_Left},
			{SDL_SCANCODE_DOWN, InputCode_Down},
			{SDL_SCANCODE_UP, InputCode_Up},
			
			//Keypad
			{SDL_SCANCODE_NUMLOCKCLEAR, InputCode_NumLockClear},
			{SDL_SCANCODE_KP_DIVIDE, InputCode_KP_Divide},
			{SDL_SCANCODE_KP_MULTIPLY, InputCode_KP_Multiply},
			{SDL_SCANCODE_KP_MINUS, InputCode_KP_Minus},
			{SDL_SCANCODE_KP_PLUS, InputCode_KP_Plus},
			{SDL_SCANCODE_KP_ENTER, InputCode_KP_Enter},
			{SDL_SCANCODE_KP_1, InputCode_KP_1},
			{SDL_SCANCODE_KP_2, InputCode_KP_2},
			{SDL_SCANCODE_KP_3, InputCode_KP_3},
			{SDL_SCANCODE_KP_4, InputCode_KP_4},
			{SDL_SCANCODE_KP_5, InputCode_KP_5},
			{SDL_SCANCODE_KP_6, InputCode_KP_6},
			{SDL_SCANCODE_KP_7, InputCode_KP_7},
			{SDL_SCANCODE_KP_8, InputCode_KP_8},
			{SDL_SCANCODE_KP_9, InputCode_KP_9},
			{SDL_SCANCODE_KP_0, InputCode_KP_0},
			{SDL_SCANCODE_KP_PERIOD, InputCode_KP_Period},
			
			//Some USB standard stuff
			{SDL_SCANCODE_NONUSBACKSLASH, InputCode_NonUSBackslash},
			{SDL_SCANCODE_APPLICATION, InputCode_Application},
			{SDL_SCANCODE_POWER, InputCode_Power},
			{SDL_SCANCODE_KP_EQUALS, InputCode_KP_Equals},
			{SDL_SCANCODE_F13, InputCode_F13},
			{SDL_SCANCODE_F14, InputCode_F14},
			{SDL_SCANCODE_F15, InputCode_F15},
			{SDL_SCANCODE_F16, InputCode_F16},
			{SDL_SCANCODE_F17, InputCode_F17},
			{SDL_SCANCODE_F18, InputCode_F18},
			{SDL_SCANCODE_F19, InputCode_F19},
			{SDL_SCANCODE_F20, InputCode_F20},
			{SDL_SCANCODE_F21, InputCode_F21},
			{SDL_SCANCODE_F22, InputCode_F22},
			{SDL_SCANCODE_F23, InputCode_F23},
			{SDL_SCANCODE_F24, InputCode_F24},
			{SDL_SCANCODE_EXECUTE, InputCode_Execute},
			{SDL_SCANCODE_HELP, InputCode_Help},
			{SDL_SCANCODE_MENU, InputCode_Menu},
			{SDL_SCANCODE_SELECT, InputCode_Select},
			{SDL_SCANCODE_STOP, InputCode_Stop},
			{SDL_SCANCODE_AGAIN, InputCode_Again},
			{SDL_SCANCODE_UNDO, InputCode_Undo},
			{SDL_SCANCODE_CUT, InputCode_Cut},
			{SDL_SCANCODE_COPY, InputCode_Copy},
			{SDL_SCANCODE_PASTE, InputCode_Paste},
			{SDL_SCANCODE_FIND, InputCode_Find},
			{SDL_SCANCODE_MUTE, InputCode_Mute},
			{SDL_SCANCODE_VOLUMEUP, InputCode_VolumeUp},
			{SDL_SCANCODE_VOLUMEDOWN, InputCode_VolumeDown},
			{SDL_SCANCODE_KP_COMMA, InputCode_KP_Comma},
			{SDL_SCANCODE_KP_EQUALSAS400, InputCode_KP_EqualsAS400},
			
			{SDL_SCANCODE_INTERNATIONAL1, InputCode_International1},
			{SDL_SCANCODE_INTERNATIONAL2, InputCode_International2},
			{SDL_SCANCODE_INTERNATIONAL3, InputCode_International3},
			{SDL_SCANCODE_INTERNATIONAL4, InputCode_International4},
			{SDL_SCANCODE_INTERNATIONAL5, InputCode_International5},
			{SDL_SCANCODE_INTERNATIONAL6, InputCode_International6},
			{SDL_SCANCODE_INTERNATIONAL7, InputCode_International7},
			{SDL_SCANCODE_INTERNATIONAL8, InputCode_International8},
			{SDL_SCANCODE_INTERNATIONAL9, InputCode_International9},
			{SDL_SCANCODE_LANG1, InputCode_Lang1},
			{SDL_SCANCODE_LANG2, InputCode_Lang2},
			{SDL_SCANCODE_LANG3, InputCode_Lang3},
			{SDL_SCANCODE_LANG4, InputCode_Lang4},
			{SDL_SCANCODE_LANG5, InputCode_Lang5},
			{SDL_SCANCODE_LANG6, InputCode_Lang6},
			{SDL_SCANCODE_LANG7, InputCode_Lang7},
			{SDL_SCANCODE_LANG8, InputCode_Lang8},
			{SDL_SCANCODE_LANG9, InputCode_Lang9},
			
			{SDL_SCANCODE_ALTERASE, InputCode_AltErase},
			{SDL_SCANCODE_SYSREQ, InputCode_SysReq},
			{SDL_SCANCODE_CANCEL, InputCode_Cancel},
			{SDL_SCANCODE_CLEAR, InputCode_Clear},
			{SDL_SCANCODE_PRIOR, InputCode_Prior},
			{SDL_SCANCODE_RETURN2, InputCode_Return2},
			{SDL_SCANCODE_SEPARATOR, InputCode_Separator},
			{SDL_SCANCODE_OUT, InputCode_Out},
			{SDL_SCANCODE_OPER, InputCode_Oper},
			{SDL_SCANCODE_CLEARAGAIN, InputCode_ClearAgain},
			{SDL_SCANCODE_CRSEL, InputCode_CRSel},
			{SDL_SCANCODE_EXSEL, InputCode_EXSel},
			
			{SDL_SCANCODE_KP_00, InputCode_KP_00},
			{SDL_SCANCODE_KP_000, InputCode_KP_000},
			{SDL_SCANCODE_THOUSANDSSEPARATOR, InputCode_ThousandsSeparator},
			{SDL_SCANCODE_DECIMALSEPARATOR, InputCode_DecimalSeparator},
			{SDL_SCANCODE_CURRENCYUNIT, InputCode_CurrencyUnit},
			{SDL_SCANCODE_CURRENCYSUBUNIT, InputCode_CurrencySubUnit},
			{SDL_SCANCODE_KP_LEFTPAREN, InputCode_KP_LeftParen},
			{SDL_SCANCODE_KP_RIGHTPAREN, InputCode_KP_RightParen},
			{SDL_SCANCODE_KP_LEFTBRACE, InputCode_KP_LeftBrace},
			{SDL_SCANCODE_KP_RIGHTBRACE, InputCode_KP_RightBrace},
			{SDL_SCANCODE_KP_TAB, InputCode_KP_Tab},
			{SDL_SCANCODE_KP_BACKSPACE, InputCode_KP_Backspace},
			{SDL_SCANCODE_KP_A, InputCode_KP_A},
			{SDL_SCANCODE_KP_B, InputCode_KP_B},
			{SDL_SCANCODE_KP_C, InputCode_KP_C},
			{SDL_SCANCODE_KP_D, InputCode_KP_D},
			{SDL_SCANCODE_KP_E, InputCode_KP_E},
			{SDL_SCANCODE_KP_F, InputCode_KP_F},
			{SDL_SCANCODE_KP_XOR, InputCode_KP_XOR},
			{SDL_SCANCODE_KP_POWER, InputCode_KP_Power},
			{SDL_SCANCODE_KP_PERCENT, InputCode_KP_Percent},
			{SDL_SCANCODE_KP_LESS, InputCode_KP_Less},
			{SDL_SCANCODE_KP_GREATER, InputCode_KP_Greater},
			{SDL_SCANCODE_KP_AMPERSAND, InputCode_KP_Ampersand},
			{SDL_SCANCODE_KP_DBLAMPERSAND, InputCode_KP_DBLAmpersand},
			{SDL_SCANCODE_KP_VERTICALBAR, InputCode_KP_VerticalBar},
			{SDL_SCANCODE_KP_DBLVERTICALBAR, InputCode_KP_DBLVerticalBar},
			{SDL_SCANCODE_KP_COLON, InputCode_KP_Colon},
			{SDL_SCANCODE_KP_HASH, InputCode_KP_Hash},
			{SDL_SCANCODE_KP_SPACE, InputCode_KP_Space},
			{SDL_SCANCODE_KP_AT, InputCode_KP_At},
			{SDL_SCANCODE_KP_EXCLAM, InputCode_KP_Exclam},
			{SDL_SCANCODE_KP_MEMSTORE, InputCode_KP_MemStore},
			{SDL_SCANCODE_KP_MEMRECALL, InputCode_KP_MemRecall},
			{SDL_SCANCODE_KP_MEMCLEAR, InputCode_KP_MemClear},
			{SDL_SCANCODE_KP_MEMADD, InputCode_KP_MemAdd},
			{SDL_SCANCODE_KP_MEMSUBTRACT, InputCode_KP_MemSubtract},
			{SDL_SCANCODE_KP_MEMMULTIPLY, InputCode_KP_MemMultiply},
			{SDL_SCANCODE_KP_MEMDIVIDE, InputCode_KP_MemDivide},
			{SDL_SCANCODE_KP_PLUSMINUS, InputCode_KP_PlusMinus},
			{SDL_SCANCODE_KP_CLEAR, InputCode_KP_Clear},
			{SDL_SCANCODE_KP_CLEARENTRY, InputCode_KP_ClearEntry},
			{SDL_SCANCODE_KP_BINARY, InputCode_KP_Binary},
			{SDL_SCANCODE_KP_OCTAL, InputCode_KP_Octal},
			{SDL_SCANCODE_KP_DECIMAL, InputCode_KP_Decimal},
			{SDL_SCANCODE_KP_HEXADECIMAL, InputCode_KP_Hexadecimal},
			
			{SDL_SCANCODE_LCTRL, InputCode_LCtrl},
			{SDL_SCANCODE_LSHIFT, InputCode_LShift},
			{SDL_SCANCODE_LALT, InputCode_LAlt},
			{SDL_SCANCODE_LGUI, InputCode_LGui},
			{SDL_SCANCODE_RCTRL, InputCode_RCtrl},
			{SDL_SCANCODE_RSHIFT, InputCode_RShift},
			{SDL_SCANCODE_RALT, InputCode_RAlt},
			{SDL_SCANCODE_RGUI, InputCode_RGui},
			
			{SDL_SCANCODE_MODE, InputCode_Mode},
			
			{SDL_SCANCODE_AUDIONEXT, InputCode_AudioNext},
			{SDL_SCANCODE_AUDIOPREV, InputCode_AudioPrev},
			{SDL_SCANCODE_AUDIOSTOP, InputCode_AudioStop},
			{SDL_SCANCODE_AUDIOPLAY, InputCode_AudioPlay},
			{SDL_SCANCODE_AUDIOMUTE, InputCode_AudioMute},
			{SDL_SCANCODE_MEDIASELECT, InputCode_MediaSelect},
			{SDL_SCANCODE_WWW, InputCode_WWW},
			{SDL_SCANCODE_MAIL, InputCode_Mail},
			{SDL_SCANCODE_CALCULATOR, InputCode_Calculator},
			{SDL_SCANCODE_COMPUTER, InputCode_Computer},
			{SDL_SCANCODE_AC_SEARCH, InputCode_AC_Search},
			{SDL_SCANCODE_AC_HOME, InputCode_AC_Home},
			{SDL_SCANCODE_AC_BACK, InputCode_AC_Back},
			{SDL_SCANCODE_AC_FORWARD, InputCode_AC_Forward},
			{SDL_SCANCODE_AC_STOP, InputCode_AC_Stop},
			{SDL_SCANCODE_AC_REFRESH, InputCode_AC_Refresh},
			{SDL_SCANCODE_AC_BOOKMARKS, InputCode_AC_Bookmarks},
			
			{SDL_SCANCODE_BRIGHTNESSDOWN, InputCode_BrightnessDown},
			{SDL_SCANCODE_BRIGHTNESSUP, InputCode_BrightnessUp},
			{SDL_SCANCODE_DISPLAYSWITCH, InputCode_DisplaySwitch},
			
			{SDL_SCANCODE_KBDILLUMTOGGLE, InputCode_KBDIllumToggle},
			{SDL_SCANCODE_KBDILLUMDOWN, InputCode_KBDIllumDown},
			{SDL_SCANCODE_KBDILLUMUP, InputCode_KBDIllumUp},
			{SDL_SCANCODE_EJECT, InputCode_Eject},
			{SDL_SCANCODE_SLEEP, InputCode_Sleep},
			
			{SDL_SCANCODE_APP1, InputCode_App1},
			{SDL_SCANCODE_APP2, InputCode_App2},
			{SDL_SCANCODE_AUDIOREWIND, InputCode_AudioRewind},
			{SDL_SCANCODE_AUDIOFASTFORWARD, InputCode_AudioFastForward},
		};
		
		static std::unordered_map<int, InputCode> map_mousebutton_inputcode = {
			{SDL_BUTTON_LEFT, InputCode_Mouse_Left},
			{SDL_BUTTON_MIDDLE, InputCode_Mouse_Middle},
			{SDL_BUTTON_RIGHT, InputCode_Mouse_Right},
			{SDL_BUTTON_X1, InputCode_Mouse_X1},
			{SDL_BUTTON_X2, InputCode_Mouse_X2},
		};
		
		//Constructors and destructor
		Event_SDL2::Event_SDL2()
		{
			return;
		}
		
		Event_SDL2::Event_SDL2(const Config config)
		{
			//Use given config
			SetConfig(config);
		}
		
		Event_SDL2::~Event_SDL2()
		{
			
		}
		
		//Event interface
		bool Event_SDL2::SetConfig(const Config config)
		{
			return false;
		}
		
		bool Event_SDL2::PollEvent(EventData &event_data)
		{
			//Process SDL events
			SDL_Event event;
			EventData new_data;
			
			while (SDL_PollEvent(&event))
			{
				//Process SDL event
				switch (event.type)
				{
					case SDL_QUIT:
						//Push quit event to queue
						new_data.type = EventType_Quit;
						event_queue.push_back(new_data);
						break;
					case SDL_KEYDOWN:
					case SDL_KEYUP:
						//Push keyboard event to queue
						new_data.type = EventType_InputBool;
						new_data.input_bool.device = InputDevice_Keyboard;
						new_data.input_bool.code = map_scancode_inputcode[event.key.keysym.scancode];
						new_data.input_bool.value = (event.type == SDL_KEYDOWN);
						event_queue.push_back(new_data);
						break;
					case SDL_MOUSEBUTTONDOWN:
					case SDL_MOUSEBUTTONUP:
						//Push mouse button event to queue
						new_data.type = EventType_InputBool;
						new_data.input_bool.device = InputDevice_Mouse;
						new_data.input_bool.code = map_mousebutton_inputcode[event.button.button];
						new_data.input_bool.value = (event.type == SDL_MOUSEBUTTONDOWN);
						event_queue.push_back(new_data);
						break;
					case SDL_MOUSEMOTION:
						new_data.type = EventType_InputFloat;
						new_data.input_float.device = InputDevice_Mouse;
						
						//Push mouse X event to queue
						new_data.input_float.code = InputCode_Mouse_X;
						new_data.input_float.value = (float)event.motion.x;
						new_data.input_float.rel_value = (float)event.motion.xrel;
						event_queue.push_back(new_data);
						
						//Push mouse Y event to queue
						new_data.input_float.code = InputCode_Mouse_Y;
						new_data.input_float.value = (float)event.motion.y;
						new_data.input_float.rel_value = (float)event.motion.yrel;
						event_queue.push_back(new_data);
						break;
					default:
						break;
				}
			}
			
			//Pop first queued event
			if (event_queue.empty())
				return false;
			event_data = event_queue.front();
			event_queue.pop_front();
			return true;
		}
	}
}
