#ifndef KDB_HEADER_INPUT
#define KDB_HEADER_INPUT

#include "kdb_common.h"
#include "kdb_math.h"

namespace kadabra {
    enum input_key_type {
        InputKey_Backspace,
        InputKey_Tab,
        InputKey_Enter,
        InputKey_Shift,
        InputKey_Ctrl,
        InputKey_Alt,
        InputKey_Pause,
        InputKey_CapsLock,
        InputKey_ESC,
        InputKey_Space,
        InputKey_PageUp,
        InputKey_PageDown,
        InputKey_End,
        InputKey_Home,
        InputKey_ArrowLeft,
        InputKey_ArrowUp,
        InputKey_ArrowRight,
        InputKey_ArrowDown,
        InputKey_PrintScreen,
        InputKey_Insert,
        InputKey_Delete,
        InputKey_Help,
        InputKey_0,
        InputKey_1,
        InputKey_2,
        InputKey_3,
        InputKey_4,
        InputKey_5,
        InputKey_6,
        InputKey_7,
        InputKey_8,
        InputKey_9,
        InputKey_A,
        InputKey_B,
        InputKey_C,
        InputKey_D,
        InputKey_E,
        InputKey_F,
        InputKey_G,
        InputKey_H,
        InputKey_I,
        InputKey_J,
        InputKey_K,
        InputKey_L,
        InputKey_M,
        InputKey_N,
        InputKey_O,
        InputKey_P,
        InputKey_Q,
        InputKey_R,
        InputKey_S,
        InputKey_T,
        InputKey_U,
        InputKey_V,
        InputKey_W,
        InputKey_X,
        InputKey_Y,
        InputKey_Z,
        InputKey_Sleep,
        InputKey_Numpad0,
        InputKey_Numpad1,
        InputKey_Numpad2,
        InputKey_Numpad3,
        InputKey_Numpad4,
        InputKey_Numpad5,
        InputKey_Numpad6,
        InputKey_Numpad7,
        InputKey_Numpad8,
        InputKey_Numpad9,
        InputKey_NumpadStar,
        InputKey_NumpadPlus,
        InputKey_NumpadMinus,
        InputKey_NumpadDot,
        InputKey_NumpadBackslash,
        InputKey_F1,
        InputKey_F2,
        InputKey_F3,
        InputKey_F4,
        InputKey_F5,
        InputKey_F6,
        InputKey_F7,
        InputKey_F8,
        InputKey_F9,
        InputKey_F10,
        InputKey_F11,
        InputKey_F12,
        InputKey_NumLock,
        InputKey_ScrollLock,
        InputKey_LeftShift,
        InputKey_RightShift,
        InputKey_LeftCtrl,
        InputKey_RightCtrl,
        InputKey_LeftAlt,
        InputKey_RightAlt,
        InputKey_Mute,
        InputKey_VolumeDown,
        InputKey_VolumeUp,
        
        InputKey_Count
    };
    
    enum input_mouse_button_type {
        InputMouseButton_Left,
        InputMouseButton_Right,
        InputMouseButton_Middle,
        InputMouseButton_Extra1,
        InputMouseButton_Extra2,
        
        InputMouseButton_Count
    };
    
    struct input_key {
        b32 IsDown;
    };
    
    struct input_buffer {
        input_key Keys[InputKey_Count];
        
        input_key MouseButtons[InputMouseButton_Count];
        vec2 MousePositionScreen;
        vec2 MousePositionClient;
        vec2 NormalisedMousePositionScreen;
        vec2 NormalisedMousePositionClient;
        f32 MouseWheel;
    };
    
    class input {
        public:
        
        input_buffer Buffers[2];
        f32 DeltaTime;
        u32 FrontBufferIndex;
        
        input_buffer *InputFrontBuffer();
        input_buffer *InputBackBuffer();
        void InputBufferPair(input_buffer **FrontBuffer, input_buffer **BackBuffer);
        
        b32 IsKeyWentDown(input_key_type KeyCode);
        b32 IsKeyDown(input_key_type KeyCode);
        b32 IsKeyReleased(input_key_type KeyCode);
        
        b32 IsMouseButtonWentDown(input_mouse_button_type Button);
        b32 IsMouseButtonDown(input_mouse_button_type Button);
        b32 IsMouseButtonReleased(input_mouse_button_type Button);
        
        f32 GetMouseWheel();
        f32 GetMouseWheelDelta();
        
        vec2 GetMousePosition();
        vec2 GetNormalisedMousePosition();
        vec2 GetMousePositionScreen();
        vec2 GetNormalisedMousePositionScreen();
        vec2 GetMousePositionDelta();
        vec2 GetNormalisedMousePositionDelta();
        vec2 GetMousePositionScreenDelta();
        vec2 GetNormalisedMousePositionScreenDelta();

    };
    
}

#endif