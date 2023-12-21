#pragma once
namespace MarkTech
{
#define MTVK_MAX_KEYS 120


#define MTIK_ESCAPE          0x01
#define MTIK_1               0x02
#define MTIK_2               0x03
#define MTIK_3               0x04
#define MTIK_4               0x05
#define MTIK_5               0x06
#define MTIK_6               0x07
#define MTIK_7               0x08
#define MTIK_8               0x09
#define MTIK_9               0x0A
#define MTIK_0               0x0B
#define MTIK_MINUS           0x0C    /* - on main keyboard */
#define MTIK_EQUALS          0x0D
#define MTIK_BACK            0x0E    /* backspace */
#define MTIK_TAB             0x0F
#define MTIK_Q               0x10
#define MTIK_W               0x11
#define MTIK_E               0x12
#define MTIK_R               0x13
#define MTIK_T               0x14
#define MTIK_Y               0x15
#define MTIK_U               0x16
#define MTIK_I               0x17
#define MTIK_O               0x18
#define MTIK_P               0x19
#define MTIK_LBRACKET        0x1A
#define MTIK_RBRACKET        0x1B
#define MTIK_RETURN          0x1C    /* Enter on main keyboard */
#define MTIK_LCONTROL        0x1D
#define MTIK_A               0x1E
#define MTIK_S               0x1F
#define MTIK_D               0x20
#define MTIK_F               0x21
#define MTIK_G               0x22
#define MTIK_H               0x23
#define MTIK_J               0x24
#define MTIK_K               0x25
#define MTIK_L               0x26
#define MTIK_SEMICOLON       0x27
#define MTIK_APOSTROPHE      0x28
#define MTIK_GRAVE           0x29    /* accent grave */
#define MTIK_LSHIFT          0x2A
#define MTIK_BACKSLASH       0x2B
#define MTIK_Z               0x2C
#define MTIK_X               0x2D
#define MTIK_C               0x2E
#define MTIK_V               0x2F
#define MTIK_B               0x30
#define MTIK_N               0x31
#define MTIK_M               0x32
#define MTIK_COMMA           0x33
#define MTIK_PERIOD          0x34    /* . on main keyboard */
#define MTIK_SLASH           0x35    /* / on main keyboard */
#define MTIK_RSHIFT          0x36
#define MTIK_MULTIPLY        0x37    /* * on numeric keypad */
#define MTIK_LMENU           0x38    /* left Alt */
#define MTIK_SPACE           0x39
#define MTIK_CAPITAL         0x3A
#define MTIK_F1              0x3B
#define MTIK_F2              0x3C
#define MTIK_F3              0x3D
#define MTIK_F4              0x3E
#define MTIK_F5              0x3F
#define MTIK_F6              0x40
#define MTIK_F7              0x41
#define MTIK_F8              0x42
#define MTIK_F9              0x43
#define MTIK_F10             0x44
#define MTIK_NUMLOCK         0x45
#define MTIK_SCROLL          0x46    /* Scroll Lock */
#define MTIK_NUMPAD7         0x47
#define MTIK_NUMPAD8         0x48
#define MTIK_NUMPAD9         0x49
#define MTIK_SUBTRACT        0x4A    /* - on numeric keypad */
#define MTIK_NUMPAD4         0x4B
#define MTIK_NUMPAD5         0x4C
#define MTIK_NUMPAD6         0x4D
#define MTIK_ADD             0x4E    /* + on numeric keypad */
#define MTIK_NUMPAD1         0x4F
#define MTIK_NUMPAD2         0x50
#define MTIK_NUMPAD3         0x51
#define MTIK_NUMPAD0         0x52
#define MTIK_DECIMAL         0x53    /* . on numeric keypad */
#define MTIK_OEM_102         0x56    /* <> or \| on RT 102-key keyboard (Non-U.S.) */
#define MTIK_F11             0x57
#define MTIK_F12             0x58
#define MTIK_F13             0x64    /*                     (NEC PC98) */
#define MTIK_F14             0x65    /*                     (NEC PC98) */
#define MTIK_F15             0x66    /*                     (NEC PC98) */
#define MTIK_KANA            0x70    /* (Japanese keyboard)            */
#define MTIK_ABNT_C1         0x73    /* /? on Brazilian keyboard */
#define MTIK_CONVERT         0x79    /* (Japanese keyboard)            */
#define MTIK_NOCONVERT       0x7B    /* (Japanese keyboard)            */
#define MTIK_YEN             0x7D    /* (Japanese keyboard)            */
#define MTIK_ABNT_C2         0x7E    /* Numpad . on Brazilian keyboard */
#define MTIK_NUMPADEQUALS    0x8D    /* = on numeric keypad (NEC PC98) */
#define MTIK_PREVTRACK       0x90    /* Previous Track (MTIK_CIRCUMFLEX on Japanese keyboard) */
#define MTIK_AT              0x91    /*                     (NEC PC98) */
#define MTIK_COLON           0x92    /*                     (NEC PC98) */
#define MTIK_UNDERLINE       0x93    /*                     (NEC PC98) */
#define MTIK_KANJI           0x94    /* (Japanese keyboard)            */
#define MTIK_STOP            0x95    /*                     (NEC PC98) */
#define MTIK_AX              0x96    /*                     (Japan AX) */
#define MTIK_UNLABELED       0x97    /*                        (J3100) */
#define MTIK_NEXTTRACK       0x99    /* Next Track */
#define MTIK_NUMPADENTER     0x9C    /* Enter on numeric keypad */
#define MTIK_RCONTROL        0x9D
#define MTIK_MUTE            0xA0    /* Mute */
#define MTIK_CALCULATOR      0xA1    /* Calculator */
#define MTIK_PLAYPAUSE       0xA2    /* Play / Pause */
#define MTIK_MEDIASTOP       0xA4    /* Media Stop */
#define MTIK_VOLUMEDOWN      0xAE    /* Volume - */
#define MTIK_VOLUMEUP        0xB0    /* Volume + */
#define MTIK_WEBHOME         0xB2    /* Web home */
#define MTIK_NUMPADCOMMA     0xB3    /* , on numeric keypad (NEC PC98) */
#define MTIK_DIVIDE          0xB5    /* / on numeric keypad */
#define MTIK_SYSRQ           0xB7
#define MTIK_RMENU           0xB8    /* right Alt */
#define MTIK_PAUSE           0xC5    /* Pause */
#define MTIK_HOME            0xC7    /* Home on arrow keypad */
#define MTIK_UP              0xC8    /* UpArrow on arrow keypad */
#define MTIK_PRIOR           0xC9    /* PgUp on arrow keypad */
#define MTIK_LEFT            0xCB    /* LeftArrow on arrow keypad */
#define MTIK_RIGHT           0xCD    /* RightArrow on arrow keypad */
#define MTIK_END             0xCF    /* End on arrow keypad */
#define MTIK_DOWN            0xD0    /* DownArrow on arrow keypad */
#define MTIK_NEXT            0xD1    /* PgDn on arrow keypad */
#define MTIK_INSERT          0xD2    /* Insert on arrow keypad */
#define MTIK_DELETE          0xD3    /* Delete on arrow keypad */
#define MTIK_LWIN            0xDB    /* Left Windows key */
#define MTIK_RWIN            0xDC    /* Right Windows key */
#define MTIK_APPS            0xDD    /* AppMenu key */
#define MTIK_POWER           0xDE    /* System Power */
#define MTIK_SLEEP           0xDF    /* System Sleep */
#define MTIK_WAKE            0xE3    /* System Wake */
#define MTIK_WEBSEARCH       0xE5    /* Web Search */
#define MTIK_WEBFAVORITES    0xE6    /* Web Favorites */
#define MTIK_WEBREFRESH      0xE7    /* Web Refresh */
#define MTIK_WEBSTOP         0xE8    /* Web Stop */
#define MTIK_WEBFORWARD      0xE9    /* Web Forward */
#define MTIK_WEBBACK         0xEA    /* Web Back */
#define MTIK_MYCOMPUTER      0xEB    /* My Computer */
#define MTIK_MAIL            0xEC    /* Mail */
#define MTIK_MEDIASELECT     0xED    /* Media Select */

#define MTVM_MAX_BUTTONS 5

#define MTVM_Mouse1		0
#define MTVM_Mouse2		1
#define MTVM_Mouse3		2
#define MTVM_MouseX1	3
#define MTVM_MouseX2	4

}