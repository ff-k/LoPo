#ifndef KDB_HEADER_RENDERER_COMMON
#define KDB_HEADER_RENDERER_COMMON

namespace kadabra {
    enum renderer_polygon_mode {
        RendererPolygonMode_Point,
        RendererPolygonMode_Wireframe,
        RendererPolygonMode_Fill
    };
    
    enum texture_filter {
        TextureFilter_Nearest,
        TextureFilter_Linear,
        TextureFilter_NearestMipmapNearest,
        TextureFilter_LinearMipmapNearest,
        TextureFilter_NearestMipmapLinear,
        TextureFilter_LinearMipmapLinear
    };
    
    enum texture_wrap {
        TextureWrap_ClampToEdge, 
        TextureWrap_ClampToBorder, 
        TextureWrap_MirroredRepeat, 
        TextureWrap_Repeat
    };
}

#endif