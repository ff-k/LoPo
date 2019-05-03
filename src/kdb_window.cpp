#include "kdb_util.h"
#include "kdb_window.h"

b32 
kadabra::window::Initialise(char *WindowTitle, 
                           platform_window_callback *Callback, 
                           cursor_handle CursorHandle, icon_handle IconHandle, 
                           s32 AtX, s32 AtY, s32 Width, s32 Height, 
                           b32 ShowImmediately){
    b32 Success = false;
    
    ZeroStruct(this);
    
    if(kadabra::platform::MemoryAllocate((void **)&Handle, 
                              sizeof(window_handle)) && 
       kadabra::platform::CreateWindow(Handle, WindowTitle, 
                               "MainWnd",
                               Callback, CursorHandle, IconHandle, 
                               AtX, AtY, Width, Height)){
        Size = Vec2i(Width, Height);
        
        if(!kadabra::platform::InitialiseWindowEventTracker(this)){
            Warning("WindowEventTracker could not be initialised");
        }
        
        if(!kadabra::platform::GetWindowClientSize(Handle, &ClientSize)){
            Warning("GetWindowClientSize failed");
        }
        
        if(ShowImmediately){
            Success = kadabra::platform::ShowWindow(Handle);
        } else {
            Success = true;
        }
    }
    
    return Success;
}

b32 
kadabra::window::OnResize(vec2i NewSize){
    b32 Success = true;
    
    Size = NewSize;
        
    if(!kadabra::platform::GetWindowClientSize(Handle, &ClientSize)){
        Warning("GetWindowClientSize failed");
        Success = false;
    }

    return Success;
}

b32 
kadabra::window::SetTitle(char *Title){
    b32 Success = true;
    
    if(!kadabra::platform::SetWindowTitle(Handle, Title)){
        Warning("SetWindowTitle failed");
        Success = false;
    }

    return Success;
}