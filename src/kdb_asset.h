#ifndef KDB_HEADER_ASSET
#define KDB_HEADER_ASSET

#include "kdb_common.h"
#include "kdb_math.h"
#include "kdb_bvh.h"

#define SwizzleCoords(CoordA, CoordB)                             \
    *((u32 *)&CoordA) = *((u32 *)&CoordA) ^ *((u32 *)&CoordB);    \
    *((u32 *)&CoordB) = *((u32 *)&CoordB) ^ *((u32 *)&CoordA);    \
    *((u32 *)&CoordA) = *((u32 *)&CoordA) ^ *((u32 *)&CoordB);

namespace kadabra {
    
    struct bvh_node;
    
    enum swizzle_order {
        SwizzleOrder_XYZ,
        SwizzleOrder_XZY,
        SwizzleOrder_YXZ,
        SwizzleOrder_YZX,
        SwizzleOrder_ZXY,
        SwizzleOrder_ZYX
    };
    
    enum draw_primitive {
        DrawPrimitive_Point,
        DrawPrimitive_Line,
        DrawPrimitive_Triangle
    };
    
    enum shader_type {
        ShaderType_Standard,
        // ShaderType_UserDefined, TODO(furkan)
    };
    
    #pragma pack(push, 1)
    struct vertex {
        vec3 Position;
        vec3 Color;
        vec3 Normal;
        vec2 TexCoord0;
    };
    #pragma pack(pop)
    
    class asset_image {
        public:
        char Path[256];
        u8 *Pixels;
    
        u32 PathLength;
    
        s32 Width;
        s32 Height;
        s32 Channels;
    
        b32 ReverseComponents;
        
        b32 Load(char *Filename);
        b32 Unload();
    };
    
    struct asset_material {
        // TODO(furkan): Parse shaders and retrieve the list of properties in the 
        // compilation phase
        asset_image *Texture;
        vec4 AmbientLight;
    
        shader_type Shader;
    };
    
    class asset_mesh {
        public:
        aabb    AABB;
        
        bvh_node *BVHRoot;
        
        vertex *Vertices;
        u32    *Indices;
        vec3   *FaceNormals;
        f32    *FaceAreas;
    
        u32 VertexCount;
        u32 FaceCount;
        u32 IndexCount;
    
        u32 CollidingFaceIdx;
    
        f32 TotalArea;
        
        b32 HasVertexNormals;
    
        draw_primitive DrawPrimitive;
        union{
            f32 PointSize;
            f32 LineWidth;
        };
    
        u32 ID;
        
        b32 ConstructBVH();
        b32 ComputeAABB(swizzle_order Swizzling);
        b32 TranslateAndScale(b32 MoveMidpointToOrigin, b32 ScaleToTarget, f32 TargetScale);
        b32 ComputePerFaceAttribute(b32 OverrideVertexNormals);
        b32 LoadOBJ(char *Filename);
        b32 LoadOFF(char *Filename);
        b32 Load(char *Filename);
    };
    
    class asset_manager {
        public:
        
        u32 CeilingIdx;
        u32 CubeInwardIdx;
        u32 CubeOutwardIdx;
        u32 CylinderIdx;
        u32 FliTriIdx;
        u32 FloorIdx;
        u32 IcosphereIdx;
        u32 PlaneIdx;
        u32 PlatformIdx;
        u32 PyramidIdx;
        u32 SphereIdx;
        
#define LoPoTextureCount 11
        asset_image    LoPoTextures[LoPoTextureCount];
        asset_material LoPoMaterials[LoPoTextureCount];
        
        //
        
        asset_image NullTexture;
        asset_material MaterialOpaque;
        asset_material MaterialTranslucent;
        asset_material MaterialHidden;
    
        asset_mesh Gizmo;
        asset_mesh Meshes[MeshAssetCapacity];
        u32 MeshCount;
        
        b32 LoadGizmo();
        b32 Initialise(u32 MeshFileCount, char **MeshFilePaths, swizzle_order Swizzling);
        b32 Finalise();
    };

}

#endif