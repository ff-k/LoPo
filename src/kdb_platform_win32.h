#ifndef KDB_HEADER_PLATFORM_WIN32
#define KDB_HEADER_PLATFORM_WIN32

#define OEMRESOURCE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef CreateWindow

#include "kdb_common.h"

namespace kadabra {

    typedef struct file_handle {
        HANDLE Ptr;
    } file_handle;
    
    typedef struct thread_handle {
        HANDLE Ptr;
    } thread_handle;
    
    typedef struct window_handle {
        HWND Ptr;
    } window_handle;
    
    typedef struct cursor_handle {
        HCURSOR Ptr;
    } cursor_handle;
    
    typedef struct icon_handle {
        HICON Ptr;
    } icon_handle;
    
    typedef struct timer {
        u64 Counter;
    } timer;
    
    typedef struct timer_freq {
        u64 Value;
    } timer_freq;
    
    #define PlatformWindowCallback(CallbackName) \
        LRESULT (CALLBACK CallbackName)(HWND, UINT, WPARAM, LPARAM)
    typedef PlatformWindowCallback(platform_window_callback);
    
    #define PlatformThreadProc(ProcName) \
        DWORD __cdecl ProcName(LPVOID Parameters)
    typedef PlatformThreadProc(platform_thread_proc);
    
}

#endif