#ifndef KDB_HEADER_RENDERER
#define KDB_HEADER_RENDERER

#include "kdb_common.h"
#include "kdb_config.h"

#include "kdb_scene.h"
#include "kdb_camera.h"
#include "kdb_asset.h"
#include "kdb_window.h"

#include "kdb_renderer_common.h"

#if KDBOpenGLEnabled
    #include "kdb_renderer_opengl.h"
#endif

namespace kadabra {
    
    enum renderer_type {
#if KDBOpenGLEnabled
        Renderer_OpenGL,
#endif
        Renderer_Count
    };
    
    class renderer {
        public:
#if KDBOpenGLEnabled
        renderer_opengl OpenGL;
#endif
        renderer_type Type;
        
        renderer_polygon_mode PolygonMode;
        
        b32 RenderScene(scene *Scene, 
                        asset_manager *AssetManager, window *Window);
        b32 SwapBuffers(window *Window);
        b32 Initialise(renderer_type RendererType, window *Window, 
                       renderer_polygon_mode PolygonMode);
        b32 OnWindowResize(window *Window);
        b32 Finalise();
    };
    
}

#endif