#ifndef KDB_HEADER_WINDOW
#define KDB_HEADER_WINDOW

#include "kdb_common.h"
#include "kdb_math.h"
#include "kdb_platform.h"

namespace kadabra {

    struct window_event_tracker {
        vec2i NewSize;
    
        b32 IsSizingOrMoving;
        b32 Resized;
        b32 ApplicationHasFocus;
    };
    
    class window {
        public:
        window_event_tracker EventTracker;
    
        window_handle *Handle;
        vec2i Size;
        vec2i ClientSize;
        
        bool QuitRequested;
        
        b32 Initialise(char *WindowTitle, platform_window_callback *Callback, 
                       cursor_handle CursorHandle, icon_handle IconHandle, 
                       s32 AtX, s32 AtY, s32 Width, s32 Height, 
                       b32 ShowImmediately);
        b32 OnResize(vec2i NewSize);
        b32 SetTitle(char *Title);
    };
    
}

#endif