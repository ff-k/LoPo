#ifndef KDB_HEADER_RENDERER_OPENGL_WIN32
#define KDB_HEADER_RENDERER_OPENGL_WIN32

#define OEMRESOURCE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <GL/GL.h>

#define WGL_DRAW_TO_WINDOW_ARB                    0x2001
#define WGL_ACCELERATION_ARB                      0x2003
#define WGL_SUPPORT_OPENGL_ARB                    0x2010
#define WGL_DOUBLE_BUFFER_ARB                     0x2011
#define WGL_PIXEL_TYPE_ARB                        0x2013
#define WGL_COLOR_BITS_ARB                        0x2014
#define WGL_DEPTH_BITS_ARB                        0x2022
#define WGL_STENCIL_BITS_ARB                      0x2023
#define WGL_NO_ACCELERATION_ARB                   0x2025
#define WGL_GENERIC_ACCELERATION_ARB              0x2026
#define WGL_FULL_ACCELERATION_ARB                 0x2027
#define WGL_TYPE_RGBA_ARB                         0x202B
#define WGL_TYPE_COLORINDEX_ARB                   0x202C
#define WGL_SAMPLE_BUFFERS_ARB                    0x2041
#define WGL_SAMPLES_ARB                           0x2042
#define WGL_CONTEXT_MAJOR_VERSION_ARB             0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB             0x2092
#define WGL_CONTEXT_LAYER_PLANE_ARB               0x2093
#define WGL_CONTEXT_FLAGS_ARB                     0x2094
#define WGL_CONTEXT_PROFILE_MASK_ARB              0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB          0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002

#define Win32OpenGLFuncDeclare(ReturnType, FuncName, ...)            \
    typedef ReturnType WINAPI OpenGLFunc_##FuncName(__VA_ARGS__);    \
    static OpenGLFunc_##FuncName *FuncName;
    
#define Win32OpenGLFuncLoad(FuncName)                                    \
    FuncName = (OpenGLFunc_##FuncName *)wglGetProcAddress(#FuncName);    \
    if(!FuncName){                                                       \
        Error("OpenGL failed to load "#FuncName" function");            \
    }

namespace kadabra {
    
    struct renderer_opengl_platform {
        HGLRC RenderingContext;
    };
    
}
    
#endif