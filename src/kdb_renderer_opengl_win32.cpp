#include "kdb_common.h"
#include "kdb_util.h"
#include "kdb_window.h"

#include "kdb_renderer_opengl_win32.h"

namespace kadabra {
    
    Win32OpenGLFuncDeclare(BOOL, wglChoosePixelFormatARB, 
                           HDC DeviceContext, const int *AttribIntList, 
                           const FLOAT *AttribFloatList, UINT MaxFormats, 
                           int *Formats, UINT *NumberOfFormats);
    Win32OpenGLFuncDeclare(HGLRC, wglCreateContextAttribsARB, 
                           HDC DeviceContext, HGLRC ShareContext, 
                           const int *AttribList);
    Win32OpenGLFuncDeclare(const char *, wglGetExtensionsStringARB, 
                           HDC DeviceContext);
    Win32OpenGLFuncDeclare(const char *, wglGetExtensionsStringEXT, void);
    Win32OpenGLFuncDeclare(BOOL, wglSwapIntervalEXT, int Interval);
    
    static b32 
    Win32LoadWGLExtensions(window *Window) {
        b32 Success = false;
    
        window FalseContextWindow;
        ZeroStruct(&FalseContextWindow);
    
        if (platform::MemoryAllocate((void **)&FalseContextWindow.Handle, 
                                     sizeof(window_handle)) &&
            Win32CreateWindow(FalseContextWindow.Handle, 
                                "FalseContext", 
                                "FalseWnd",
                                0, {0}, {0}, 0, 0, 
                                Window->Size.x, Window->Size.y)) {
            HDC DeviceContext = GetDC(FalseContextWindow.Handle->Ptr);
    
            PIXELFORMATDESCRIPTOR PixelFormatDesc =
            {
                sizeof(PIXELFORMATDESCRIPTOR),
                1,
                PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
                PFD_TYPE_RGBA,
                32,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                24,
                8,
                0,
                PFD_MAIN_PLANE,
                0, 0, 0, 0 };
    
            s32 PixelFormat = ChoosePixelFormat(DeviceContext, &PixelFormatDesc);
            SetPixelFormat(DeviceContext, PixelFormat, &PixelFormatDesc);
    
            HGLRC OpenGLRC = wglCreateContext(DeviceContext);
            if (OpenGLRC) {
                if (wglMakeCurrent(DeviceContext, OpenGLRC)) {
                    // TODO(furkan) : Do not forget to handle failures
                    Win32OpenGLFuncLoad(wglChoosePixelFormatARB);
                    Win32OpenGLFuncLoad(wglCreateContextAttribsARB);
                    Win32OpenGLFuncLoad(wglGetExtensionsStringARB);
                    Win32OpenGLFuncLoad(wglGetExtensionsStringEXT);
                    Win32OpenGLFuncLoad(wglSwapIntervalEXT);
    
                    Success = true;
                    wglMakeCurrent(0, 0);
                }
    
                wglDeleteContext(OpenGLRC);
            }
    
            ReleaseDC(FalseContextWindow.Handle->Ptr, DeviceContext);
            DestroyWindow(FalseContextWindow.Handle->Ptr);
        }
    
        if(!platform::MemoryFree((void **)&FalseContextWindow.Handle)){
            Warning("PlatformMemoryFree failed on FalseContextWindow");
        }
        
        return Success;
    }
    
    static b32
    Win32LoadOpenGLExtensions(char *Extensions){
        b32 Success = true;
    
        // TODO(furkan) : Load extensions here, if needed
        Log("OpenGLExtensions : %s", Extensions);
    
        return Success;
    }
    
    b32
    Win32SwapBuffers(window *Window){
        b32 Success = true;
        
        HDC DeviceContext = GetDC(Window->Handle->Ptr);
        if(DeviceContext){
            SwapBuffers(DeviceContext);
            ReleaseDC(Window->Handle->Ptr, DeviceContext);
        } else {
            Success = false;
        }
    
        return Success;
    }
    
    b32
    Win32Initialise(renderer_opengl_platform *OpenGL, window *Window){
        b32 Success = false;
        
        HWND WindowHandle = Window->Handle->Ptr;
        
        HDC DeviceContext = GetDC(WindowHandle);
        if(DeviceContext){
            s32 PixelFormat = 0;
            u32 PixelFormatCount = 0;
            
            if(!Win32LoadWGLExtensions(Window)){
                Warning("Win32LoadWGLExtensions failed!");
            }
            
            if(wglChoosePixelFormatARB){
                int IntAttribList[] =
                {
                    WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
                    WGL_ACCELERATION_ARB  , WGL_FULL_ACCELERATION_ARB,
                    WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
                    WGL_DOUBLE_BUFFER_ARB , GL_TRUE,
                    WGL_PIXEL_TYPE_ARB    , WGL_TYPE_RGBA_ARB,
                    WGL_COLOR_BITS_ARB    , 32,
                    WGL_DEPTH_BITS_ARB    , 24,
                    WGL_STENCIL_BITS_ARB  , 8,
                    WGL_SAMPLE_BUFFERS_ARB, 1,
                    WGL_SAMPLES_ARB       , 16, // NOTE(furkan): 16x Multisampling
                    0,
                };
            
                if(wglChoosePixelFormatARB(DeviceContext, IntAttribList, 0, 1, 
                                           &PixelFormat, &PixelFormatCount)){
                    // NOTE(furkan): Success
                } else {
                    PixelFormatCount = 0;
                    Warning("wglChoosePixelFormatARB failed");
                }
            }
        
            if (!PixelFormatCount) {
                PIXELFORMATDESCRIPTOR PixelFormatDesc =
                {
                    sizeof(PIXELFORMATDESCRIPTOR),
                    1,
                    PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
                    PFD_TYPE_RGBA,
                    32,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    24,
                    8,
                    0,
                    PFD_MAIN_PLANE,
                    0, 0, 0, 0 };
            
                PixelFormat = ChoosePixelFormat(DeviceContext, 
                                                &PixelFormatDesc);
            }
            
            PIXELFORMATDESCRIPTOR PixelFormatDesc;
            if(DescribePixelFormat(DeviceContext, PixelFormat, sizeof(PixelFormatDesc), 
                                &PixelFormatDesc)){
                if(SetPixelFormat(DeviceContext, PixelFormat, &PixelFormatDesc)){
                    
                    if (wglCreateContextAttribsARB) {
                        int ContextAttribs[] =
                        {
                            WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
                            WGL_CONTEXT_MINOR_VERSION_ARB, 2,
                            WGL_CONTEXT_FLAGS_ARB, 0,
                            WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                            0,
                        };
                    
                        OpenGL->RenderingContext = 
                                            wglCreateContextAttribsARB(DeviceContext, 
                                                                    0, 
                                                                    ContextAttribs);
                    }
                    
                    if (!OpenGL->RenderingContext) {
                        OpenGL->RenderingContext = wglCreateContext(DeviceContext);
                    }
                    
                    if (OpenGL->RenderingContext) {
                        if (wglMakeCurrent(DeviceContext, OpenGL->RenderingContext)) {
                            Success = true;
                    
                            char *OpenGLExtensions = 0;
                            if(wglGetExtensionsStringARB){
                                OpenGLExtensions = (char *)wglGetExtensionsStringARB(DeviceContext);
                            }
    
                            if(!OpenGLExtensions && wglGetExtensionsStringEXT){
                                OpenGLExtensions = (char *)wglGetExtensionsStringEXT();
                            }
    
                            if(OpenGLExtensions){
                                if(!Win32LoadOpenGLExtensions(OpenGLExtensions)){
                                    Warning("LoadOpenGLExtensions failed");
                                }
                            } else {
                                Warning("Could not detected any OpenGLExtension");
                            }
    
                            if (wglSwapIntervalEXT) {
                                wglSwapIntervalEXT(1);
                            }
                        } else {
                            wglDeleteContext(OpenGL->RenderingContext);
                        }
                    }
                }
            }
            
            ReleaseDC(WindowHandle, DeviceContext);        
        }
        
        return Success;
    }
    
}
