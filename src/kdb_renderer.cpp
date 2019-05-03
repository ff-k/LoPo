#include "kdb_renderer.h"
#include "kdb_util.h"

using namespace kadabra;

b32
kadabra::renderer::RenderScene(scene *Scene, 
                               asset_manager *AssetManager, window *Window){
    b32 Success = false;

    switch(Type){
#if KDBOpenGLEnabled
        case Renderer_OpenGL:{
            Assert(Scene->ActiveCameraIndex < Scene->CameraCount);
            camera *Camera = Scene->Cameras + Scene->ActiveCameraIndex;
            Success = OpenGL.RenderScene(AssetManager, Scene, Camera, Window, 
                                         PolygonMode);
        } break;
#endif
        UnexpectedDefaultCase
    }

    return Success;
}

b32
kadabra::renderer::SwapBuffers(window *Window){
    b32 Success = false;

    switch(Type){
#if KDBOpenGLEnabled
        case Renderer_OpenGL:{
            Success = opengl_platform::SwapBuffers(Window);
        } break;
#endif
        UnexpectedDefaultCase
    }

    return Success;
}

b32
kadabra::renderer::Initialise(renderer_type RendererType, window *Window, 
                              renderer_polygon_mode PolygonMode){
    b32 Success = false;
    
    switch(RendererType){
#if KDBOpenGLEnabled
        case Renderer_OpenGL:{
            
            ZeroStruct(&OpenGL);
            
            if(OpenGL.Initialise(Window)){
                if(opengl_platform::Initialise(&OpenGL.Platform, Window)){
                    OpenGL.LoadFunctions();
                    if(OpenGL.OnInitialise(Window)){
                        Success = true;
                    } else {
                        Warning("OpenGLOnInitialise failed");
                    }
                }
            }
        } break;
#endif
        default:{
            Error("Unexpected renderer type");
        } break;
    }
    
    this->PolygonMode = PolygonMode;
    
    if(Success){
        Type = RendererType;
    }

    return Success;
}

b32
kadabra::renderer::OnWindowResize(window *Window){
    b32 Success = false;

    switch(Type){
#if KDBOpenGLEnabled
        case Renderer_OpenGL:{
            Success = OpenGL.OnWindowResize(Window);
        } break;
#endif
        UnexpectedDefaultCase
    }

    return Success;
}

b32 kadabra::renderer::Finalise(){
    b32 Success = false;

    switch(Type){
#if KDBOpenGLEnabled
        case Renderer_OpenGL:{
            Success = OpenGL.Finalise();
        } break;
#endif
        UnexpectedDefaultCase
    }

    return Success;

}