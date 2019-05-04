#include "kdb_util.h"
#include "kdb_dyn_array.h"
#include "kdb_math.h"
#include "kdb_string.h"
#include "kdb_platform.h"
#include "kdb_io.h"
#include "kdb_asset.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT
#include <stb_image_write.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

b32 
kadabra::asset_image::Load(char *Filename){
    
    // TODO(furkan): Texture manager to control image load/unload
    b32 Success = false;

    size_t FilenameLength = StringLength(Filename);
    if(platform::MemoryCopy(Path, StaticArraySize(Path),
                             Filename, FilenameLength)){
        PathLength = (u32) FilenameLength;
    } else {
        Warning("An error occured while copying filename");
    }

    stbi_set_flip_vertically_on_load(true);
    Pixels = stbi_load(Filename, &Width, &Height, &Channels, 0);
    // printf("%s, %u x %u, %u\n", Filename, Width, Height, Channels);
    ReverseComponents = false;
    if(Pixels){
        Success = true;
    }

    return Success;
}

b32 
kadabra::asset_image::Unload(){
    b32 Success = true;

    stbi_image_free(Pixels);
    Pixels = 0;

    return Success;
}

b32 kadabra::asset_mesh::ConstructBVH(){
    b32 Success = false;
    
    Assert(IndexCount > 0);
    
    printf("FaceCount: %4u, ", FaceCount);
    
    u32 *FacesSorted = 0;
    if(platform::MemoryAllocate((void **)&FacesSorted, 
                                sizeof(u32) * FaceCount)){
        
        for(u32 Idx=0; Idx<FaceCount; Idx++){
            FacesSorted[Idx] = Idx;
        }
        
        BVHRoot = 0;
        u32 BVHDepth = bvh::ConstructFromIndexedMesh(&BVHRoot, 
                                                     FacesSorted, FaceCount, 
                                                     Vertices, VertexCount, 
                                                     Indices, IndexCount);
                                                     
        printf("BVHDepth: %2u", BVHDepth);
                                                     
        if(BVHDepth){
            Success = true;
        } else {
            // TODO(furkan): Free allocated BVH nodes, if there are any
            BVHRoot = 0;
        }
        
        platform::MemoryFree((void **)&FacesSorted);
    }
    
    printf("\n");
    
    return Success;
}

b32
kadabra::asset_mesh::ComputeAABB(swizzle_order Swizzling){
    b32 Success = true;
    
    vec3 Min = Vec3(f32_Max, f32_Max, f32_Max);
    vec3 Max = Vec3(f32_Min, f32_Min, f32_Min);
    
    if(Swizzling != SwizzleOrder_XYZ){
        for(u32 VertexIndex = 0; 
                VertexIndex < VertexCount;
                VertexIndex++){
    
            vertex *V = Vertices + VertexIndex;
            vec3 Position = V->Position;
    
            switch(Swizzling){
                case SwizzleOrder_XYZ:{
                    Assert(!"Should be unreachable");
                } break;
                case SwizzleOrder_XZY:{
                    SwizzleCoords(Position.y, Position.z);
                } break;
                case SwizzleOrder_YXZ:{
                    SwizzleCoords(Position.y, Position.x);
                } break;
                case SwizzleOrder_YZX:{
                    SwizzleCoords(Position.y, Position.x);
                    SwizzleCoords(Position.y, Position.z);
                } break;
                case SwizzleOrder_ZXY:{
                    SwizzleCoords(Position.x, Position.z);
                    SwizzleCoords(Position.y, Position.z);
                } break;
                case SwizzleOrder_ZYX:{
                    SwizzleCoords(Position.x, Position.z);
                } break;
                UnexpectedDefaultCase;
            }
    
            V->Position = Vec3(Position.x, 
                               Position.y, 
                               Position.z);
    
            if(Min.x > Position.x){ Min.x = Position.x; }
            if(Min.y > Position.y){ Min.y = Position.y; }
            if(Min.z > Position.z){ Min.z = Position.z; }
            if(Max.x < Position.x){ Max.x = Position.x; }
            if(Max.y < Position.y){ Max.y = Position.y; }
            if(Max.z < Position.z){ Max.z = Position.z; }
        }
    } else {
        for(u32 VertexIndex = 0; 
                VertexIndex < VertexCount;
                VertexIndex++){
    
            vertex *V = Vertices + VertexIndex;
            vec3 Position = V->Position;
            
            if(Min.x > Position.x){ Min.x = Position.x; }
            if(Min.y > Position.y){ Min.y = Position.y; }
            if(Min.z > Position.z){ Min.z = Position.z; }
            if(Max.x < Position.x){ Max.x = Position.x; }
            if(Max.y < Position.y){ Max.y = Position.y; }
            if(Max.z < Position.z){ Max.z = Position.z; }
        }
    }
    
    AABB = { Min, Max };
    
    return Success;
}

b32 
kadabra::asset_mesh::TranslateAndScale(b32 MoveMidpointToOrigin, 
                                       b32 ScaleToTarget, 
                                       f32 TargetScale){
    b32 Success = true;
    
    if(MoveMidpointToOrigin || ScaleToTarget){
        vec3 Min = AABB.Min;
        vec3 Max = AABB.Max;
        
        // NOTE(furkan): Translate every point 
        // so that the center point will be on 
        // the origin (i.e. Vec3(0, 0, 0))
        vec3 Center = ((Min+Max)*0.5f);
        Min -= Center;
        Max -= Center;
    
        // NOTE(furkan): Scale every point so that 
        // the maximum size for a dimension (i.e. x, 
        // y or z) will be TargetScale
        f32 Scale = 1.0f;
        if(ScaleToTarget){
            vec3 Dim = Max-Min;
            Dim.x = Abs(Dim.x);
            Dim.y = Abs(Dim.y);
            Dim.z = Abs(Dim.z);
            
            f32 MaxDim = MaxOf(MaxOf(Dim.x, Dim.y), Dim.z);
            Scale = TargetScale/MaxDim;
            Min *= Scale;
            Max *= Scale;
            
            if(!MoveMidpointToOrigin){
                Min += Center;
                Max += Center;
            }
        }
        
        if(MoveMidpointToOrigin && ScaleToTarget){
            for(u32 VertexIndex = 0; 
                    VertexIndex < VertexCount;
                    VertexIndex++){
                vertex *V = Vertices + VertexIndex;
                V->Position -= Center;
                V->Position *= Scale;
            }
        } else if (ScaleToTarget){
            for(u32 VertexIndex = 0; 
                    VertexIndex < VertexCount;
                    VertexIndex++){
                vertex *V = Vertices + VertexIndex;
                V->Position -= Center; 
                V->Position *= Scale;
                V->Position += Center;
            }
        } else if (MoveMidpointToOrigin){
            for(u32 VertexIndex = 0; 
                    VertexIndex < VertexCount;
                    VertexIndex++){
                vertex *V = Vertices + VertexIndex;
                V->Position -= Center;
            }
        }
        
        AABB = { Min, Max };
    }
    
    return Success;
}

b32 
kadabra::asset_mesh::ComputePerFaceAttribute(b32 OverrideVertexNormals){
    b32 Success = true;

    if(FaceCount){
        if(platform::MemoryAllocate((void **)&FaceNormals, 
                                    sizeof(vec3) * FaceCount) && 
           platform::MemoryAllocate((void **)&FaceAreas, 
                                    sizeof(f32) * FaceCount)){
                                        
            if(OverrideVertexNormals && HasVertexNormals){
                for(u32 VertexIndex = 0; 
                        VertexIndex < VertexCount;
                        VertexIndex++){
                    vertex *V = Vertices + VertexIndex;
                    V->Normal = Vec3(0.0f, 0.0f, 0.0f);
                }
            }
            
            TotalArea = 0.0f;
            for(u32 FaceIndex = 0; 
                    FaceIndex < FaceCount;
                    FaceIndex++){
                u32 I0 = Indices[FaceIndex*3 + 0];
                u32 I1 = Indices[FaceIndex*3 + 1];
                u32 I2 = Indices[FaceIndex*3 + 2];

                vertex *V0 = Vertices + I0;
                vertex *V1 = Vertices + I1;
                vertex *V2 = Vertices + I2;

                vec3 FaceNormal = Cross(V1->Position - V0->Position,
                                        V2->Position - V0->Position);

                if(OverrideVertexNormals){
                    V0->Normal += FaceNormal;
                    V1->Normal += FaceNormal;
                    V2->Normal += FaceNormal;
                }
                
                f32 FaceArea = Length(FaceNormal)*0.5f;
                vec3 FaceNormalized = Normalize(FaceNormal);
                FaceNormals[FaceIndex] = FaceNormalized;
                FaceAreas[FaceIndex] = FaceArea;
                TotalArea += FaceArea;
            }
            
            if(OverrideVertexNormals){
                for(u32 VertexIndex = 0; 
                        VertexIndex < VertexCount;
                        VertexIndex++){
                    vertex *V = Vertices + VertexIndex;
                    V->Normal = Normalize(V->Normal);
                }
                
                HasVertexNormals = true;
            }
        } else {
            Success = false;
        }
    } else {
        // TODO(furkan): These properties should be computed in cases 
        // where triangles are not indexed, as well.
        Assert(!"Not implemented");
    }

    return Success;
}

namespace kadabra {
    s32 
    CompareU32Triplet(const u32_triplet *A, const u32_triplet *B){
        s32 Result = 0;
        
        for(u32 Idx=0; Idx<3; Idx++){
            if(A->Val[Idx] == B->Val[Idx]){
                
            } else if(A->Val[Idx] > B->Val[Idx]){
                Result = 1;
                break;
            } else {
                Result = -1;
                break;
            }
        }
        
        return Result;
    }
    
    s32 
    CompareU32(const u32 *A, const u32 *B){
        return *A - *B;
    }
    
    s32 
    CompareVec3(const vec3 *A, const vec3 *B){
        s32 Result = 0;
        
        if(A->x == B->x){
            if(A->y == B->y){
                if(A->z == B->z){
                    
                } else if(A->z > B->z){
                    Result = 1;
                } else {
                    Result = -1;
                }
            } else if(A->y > B->y){
                Result = 1;
            } else {
                Result = -1;
            }
        } else if(A->x > B->x){
            Result = 1;
        } else {
            Result = -1;
        }
        
        return Result;
    }
    
    s32 
    CompareVec2(const vec2 *A, const vec2 *B){
        s32 Result = 0;
        
        if(A->x == B->x){
            if(A->y == B->y){
                
            } else if(A->y > B->y){
                Result = 1;
            } else {
                Result = -1;
            }
        } else if(A->x > B->x){
            Result = 1;
        } else {
            Result = -1;
        }
        
        return Result;
    }
}

b32
kadabra::asset_mesh::LoadOBJ(char *Filename){
    b32 Success = false;
    
    ZeroStruct(this);

    DrawPrimitive = DrawPrimitive_Triangle;
    PointSize = 1.0f;

    char *FileContents = 0;
    u64 FileSize = 0;
    if(ReadEntireFile(Filename, (void **)&FileContents, &FileSize)){
        char *Cursor = FileContents;
        
        Assert(FileSize < 0x7FFFFFFF);
        u32 TotalLength = (u32)FileSize;
        
        dyn_array<u32_triplet> Triplets(CompareU32Triplet);
        dyn_array<u32>  BufIndices(CompareU32);
        dyn_array<vec3> BufPositions(CompareVec3);
        dyn_array<vec3> BufNormals(CompareVec3);
        dyn_array<vec2> BufUVs(CompareVec2);
        
        u32 ParsedLength = 0;
        while(ParsedLength < TotalLength){
            u32 LineLength = 0;
            char *Line = Cursor;
            while(ParsedLength < TotalLength){
                LineLength++;
                ParsedLength++;
                Cursor++;
                
                char Current = *Cursor;
                if(Current == '\n'){
                    LineLength++;
                    ParsedLength++;
                    Cursor++;
                    break;
                }
            }
            
            if(LineLength){
                switch(Line[0]){
                    case '#': {
                        // NOTE(furkan): Comment line. Do nothing
                    } break;
                    case 'f': {
                        Assert(LineLength >= 19);
                        Line++;
                        
                        Assert(Line[0] == ' ');
                        Line++;
                        
                        u32 IdxV[3];
                        u32 IdxVT[3];
                        u32 IdxVN[3];
                        for(u32 Vert=0; Vert<3; Vert++){
                            IdxV[Vert]  = StringToU32(Line, &Line);
                            Line++;
                            
                            IdxVT[Vert] = StringToU32(Line, &Line);
                            Line++;
                            
                            IdxVN[Vert] = StringToU32(Line, &Line);
                            Line++;
                        }
                        
                        for(u32 Idx=0; Idx<3; Idx++){
                            u32_triplet T = { IdxV[Idx], IdxVN[Idx], IdxVT[Idx] };
                            u32 TIdx;
                            if(Triplets.Find(&T, &TIdx)){
                                // NOTE(furkan): At this point, TIdx contains the index 
                                // of T in Triplets
                            } else {
                                if(Triplets.Insert(T)){
                                    TIdx = Triplets.Size()-1;
                                } else {
                                    Error("Insert failed on Triplets");
                                }
                            }
                            
                            if(!BufIndices.Insert(TIdx)){
                                Error("Insert failed on BufIndices");
                            }
                        }
                        
                        
                    } break;
                    case 'v': {
                        Assert(LineLength >= 20);
                        Line++;
                        
                        switch(Line[0]){
                            case ' ': {
                                Line++;
                                
                                // NOTE(furkan): Position
                                f32 P0 = StringToF32(Line, &Line);
                                f32 P1 = StringToF32(Line, &Line);
                                f32 P2 = StringToF32(Line, &Line);
                                
                                if(!BufPositions.Insert(Vec3(P0, P1, P2))){
                                    Error("Insert failed on BufPositions");
                                }
                            } break;
                            case 't': {
                                Line++;
                                
                                // NOTE(furkan): UV
                                f32 U = StringToF32(Line, &Line);
                                f32 V = StringToF32(Line, &Line);
                                
                                if(!BufUVs.Insert(Vec2(U, V))){
                                    Error("Insert failed on BufUVs");
                                }
                            } break;
                            case 'n': {
                                Line++;
                                
                                // NOTE(furkan): Normal
                                f32 N0 = StringToF32(Line, &Line);
                                f32 N1 = StringToF32(Line, &Line);
                                f32 N2 = StringToF32(Line, &Line);
                                
                                if(!BufNormals.Insert(Vec3(N0, N1, N2))){
                                    Error("Insert failed on BufNormals");
                                }
                            } break;
                            default: {
                                Warning("obj contains unsupported feature");
                            } break;
                        }
                    } break;
                    default: {
                        Warning("obj contains unsupported feature");
                    } break;
                }
            }
        }
        
        Assert(BufIndices.Size() % 3 == 0);
        
        VertexCount = Triplets.Size();
        IndexCount  = BufIndices.Size();
        FaceCount   = IndexCount / 3;
        
        Log("VertexCount: %u, FaceCount: %u", VertexCount, FaceCount);
        
        if(platform::MemoryAllocate((void **)&Vertices, 
                                    sizeof(vertex) * VertexCount) && 
           platform::MemoryAllocate((void **)&Indices, 
                                    sizeof(u32) * IndexCount)){
            
            for(u32 VertIdx=0; VertIdx<VertexCount; VertIdx++){
                u32_triplet *Trip = Triplets.Array + VertIdx;
                vertex *V = Vertices + VertIdx;
                V->Position  = BufPositions[Trip->Val[0]-1];
                V->Color     = Vec3(1.0f, 1.0f, 1.0f); 
                V->Normal    =   BufNormals[Trip->Val[1]-1];
                V->TexCoord0 =       BufUVs[Trip->Val[2]-1];
            }
            
            size_t CopySize = sizeof(u32)*IndexCount;
            if(platform::MemoryCopy(Indices, CopySize,
                                    BufIndices.Array, CopySize)){
                Success = true;
                HasVertexNormals = true;
            } else {
                Error("MemoryCopy failed");
            }
        }

        platform::MemoryFree((void **)&FileContents);
    } else {
        Error("An error occured while loading file: %s", Filename);
    }

    return Success;
}

b32
kadabra::asset_mesh::LoadOFF(char *Filename){
    b32 Success = false;
    
    ZeroStruct(this);

    DrawPrimitive = DrawPrimitive_Triangle;
    PointSize = 1.0f;

    char *FileContents = 0;
    u64 FileSize = 0;
    if(ReadEntireFile(Filename, (void **)&FileContents, &FileSize)){
        char *Cursor = FileContents;

        char *FileSignature = "OFF";
        size_t FileSignatureLength = StringLength(FileSignature);
        if(!StringCompareN((char *)Cursor, FileSize, 
                           FileSignature, FileSignatureLength)){
            Cursor += FileSignatureLength;

            VertexCount   = StringToU32(Cursor, &Cursor);
            FaceCount     = StringToU32(Cursor, &Cursor);
            u32 EdgeCount = StringToU32(Cursor, &Cursor);
            IndexCount  = FaceCount * 3;
            Assert(IndexCount > FaceCount);

            Log("VertexCount: %u, FaceCount: %u, EdgeCount: %u", VertexCount,
                                                                 FaceCount,
                                                                 EdgeCount);

            if(platform::MemoryAllocate((void **)&Vertices, 
                                      sizeof(vertex) * VertexCount) && 
               platform::MemoryAllocate((void **)&Indices, 
                                      sizeof(u32) * IndexCount)){

                for(u32 VertexIndex = 0; 
                        VertexIndex < VertexCount;
                        VertexIndex++){
                    vec3 Position = StringToVec3(Cursor, &Cursor);

                    vertex *V = Vertices + VertexIndex;
                    V->Position = Vec3(Position.x, 
                                       Position.y, 
                                       Position.z);
                    V->Color     = Vec3(1.0f, 1.0f, 1.0f);
                    V->Normal    = Vec3(0.0f, 0.0f, 0.0f);
                    V->TexCoord0 = Vec2(0.0f, 0.0f);
                }

                for(u32 FaceIndex = 0; 
                        FaceIndex < FaceCount;
                        FaceIndex++){
                    u32 FaceVertexCount = StringToU32(Cursor, &Cursor);

                    // TODO(furkan): For now, OFF importer only supports 
                    // triangular faces. 
                    Assert(FaceVertexCount == 3);

                    // NOTE(furkan): Indices are 0-based
                    u32 I0 = StringToU32(Cursor, &Cursor);
                    u32 I1 = StringToU32(Cursor, &Cursor);
                    u32 I2 = StringToU32(Cursor, &Cursor);

                    Indices[FaceIndex*3 + 0] = I0;
                    Indices[FaceIndex*3 + 1] = I1;
                    Indices[FaceIndex*3 + 2] = I2;
                }

                while((size_t)(Cursor-FileContents) < FileSize && 
                      IsWhitespace(Cursor[0])){
                    Cursor++;
                }

                Assert((size_t)(Cursor-FileContents) == FileSize);
                
                Log("Loaded successfully");
                Success = true;
            }
        } else {
            Error("Header could not be recognized");
        }

        platform::MemoryFree((void **)&FileContents);
    } else {
        Error("An error occured while loading file: %s", Filename);
    }

    return Success;
}

b32 
kadabra::asset_mesh::Load(char *Filename){
    b32 Success = false;

    size_t FilenameLen = StringLength(Filename);
    u32 FileExtIdx = FileExtensionIndex(Filename);

    Log("Loading %s", Filename);
    if(!StringCompare(Filename+FileExtIdx, FilenameLen-FileExtIdx,
                      "off", 3) ||
       !StringCompare(Filename+FileExtIdx, FilenameLen-FileExtIdx,
                      "OFF", 3)){
        Success = LoadOFF(Filename);
    } else if(!StringCompare(Filename+FileExtIdx, FilenameLen-FileExtIdx,
                      "obj", 3) ||
       !StringCompare(Filename+FileExtIdx, FilenameLen-FileExtIdx,
                      "OBJ", 3)){
        Success = LoadOBJ(Filename);
    } else {
        Error("Mesh file type is not supported! (%s)", Filename+FileExtIdx);
    }

    return Success;
}

b32
kadabra::asset_manager::LoadGizmo(){
    b32 Success = false;
    
    ZeroStruct(&Gizmo);
    
    char *Filename = "..\\..\\assets\\meshes\\gizmo";
    char *FileContents = 0;
    u64 FileSize = 0;
    if(ReadEntireFile(Filename, (void **)&FileContents, &FileSize)){
        char *Cursor = FileContents;

        u32 VertexCount = *((u32 *)Cursor); Cursor += sizeof(u32);
        u32 FaceCount   = *((u32 *)Cursor); Cursor += sizeof(u32);
        u32 IndexCount  = FaceCount * 3;

        if(platform::MemoryAllocate((void **)&Gizmo.Vertices, 
                                    sizeof(vertex) * VertexCount) && 
           platform::MemoryAllocate((void **)&Gizmo.Indices, 
                                    sizeof(u32) * IndexCount)){

            for(u32 VertexIndex = 0; 
                    VertexIndex < VertexCount;
                    VertexIndex++){
                vec3 Position;
                Position.x = *((f32 *)Cursor); Cursor += sizeof(f32);
                Position.y = *((f32 *)Cursor); Cursor += sizeof(f32);
                Position.z = *((f32 *)Cursor); Cursor += sizeof(f32);
                
                vec3 Color;
                Color.x = *((f32 *)Cursor); Cursor += sizeof(f32);
                Color.y = *((f32 *)Cursor); Cursor += sizeof(f32);
                Color.z = *((f32 *)Cursor); Cursor += sizeof(f32);

                vertex *V = Gizmo.Vertices + VertexIndex;
                V->Position  = Position;
                V->Color     = Color;
                V->Normal    = Vec3(0.0f, 0.0f, 0.0f);
                V->TexCoord0 = Vec2(0.0f, 0.0f);
            }

            for(u32 FaceIndex = 0; 
                    FaceIndex < FaceCount;
                    FaceIndex++){
                u32 I0 = *((u32 *)Cursor); Cursor += sizeof(u32);
                u32 I1 = *((u32 *)Cursor); Cursor += sizeof(u32);
                u32 I2 = *((u32 *)Cursor); Cursor += sizeof(u32);

                Gizmo.Indices[FaceIndex*3 + 0] = I0;
                Gizmo.Indices[FaceIndex*3 + 1] = I1;
                Gizmo.Indices[FaceIndex*3 + 2] = I2;
            }
            
            Gizmo.VertexCount = VertexCount;
            Gizmo.FaceCount = FaceCount;
            Gizmo.IndexCount = IndexCount;
            
            Gizmo.ID            = 0xFFFFFF00;
            Gizmo.DrawPrimitive = DrawPrimitive_Triangle;
            Gizmo.PointSize     = 1.0f;

            Assert((size_t)(Cursor-FileContents) == FileSize);
            
            Log("Loaded successfully");
            Success = true;
            
            // Gizmo.TranslateAndScale(true, true, 100.0f);
            Gizmo.ComputePerFaceAttribute(true);
        }

        platform::MemoryFree((void **)&FileContents);
    } else {
        Error("An error occured while loading file: %s", Filename);
    }

    return Success;
}

b32
kadabra::asset_manager::Initialise(u32 MeshFileCount, char **MeshFilePaths, 
                                   swizzle_order Swizzling){
    b32 Success = false;

    ZeroStruct(this);
    
    if(NullTexture.Load("..\\..\\assets\\textures\\null.jpg")){
        Success = true;
    } else {
        Error("Null texture could not be loaded");
    }

    MaterialOpaque.Shader = ShaderType_Standard;
    MaterialOpaque.Texture = &NullTexture;
    MaterialOpaque.AmbientLight = Vec4(0.85f, 0.85f, 0.85f, 1.0f);

    MaterialTranslucent.Shader = ShaderType_Standard;
    MaterialTranslucent.Texture = &NullTexture;
    MaterialTranslucent.AmbientLight = Vec4(0.15f, 0.15f, 0.15f, 0.15f);
    
    MaterialHidden.Shader = ShaderType_Standard;
    MaterialHidden.Texture = &NullTexture;
    MaterialHidden.AmbientLight = Vec4(1.0f, 1.0f, 1.0f, 0.0f);
    
    LoadGizmo();
    
    ///
    
    CeilingIdx     = 0;
    CubeInwardIdx  = 1;
    CubeOutwardIdx = 2;
    CylinderIdx    = 3;
    FliTriIdx      = 4;
    FloorIdx       = 5;
    IcosphereIdx   = 6;
    PlaneIdx       = 7;
    PlatformIdx    = 8;
    PyramidIdx     = 9;
    SphereIdx      = 10;
    
    Assert(MeshFileCount == LoPoTextureCount);
    
    char *LoPoTexturePaths[] = {
        "..\\..\\assets\\textures\\ceiling.png ",
        "..\\..\\assets\\textures\\cube_inward.png ",
        "..\\..\\assets\\textures\\cube_outward.png",
        "..\\..\\assets\\textures\\cylinder.png ",
        "..\\..\\assets\\textures\\flitri.png ",
        "..\\..\\assets\\textures\\floor.png ",
        "..\\..\\assets\\textures\\icosphere.png ",
        "..\\..\\assets\\textures\\plane.png ",
        "..\\..\\assets\\textures\\platform.png ",
        "..\\..\\assets\\textures\\pyramid.png ",
        "..\\..\\assets\\textures\\sphere.png",
    };
    
    for(u32 Idx=0; Idx<LoPoTextureCount; Idx++){
        if(LoPoTextures[Idx].Load(LoPoTexturePaths[Idx])){
            LoPoMaterials[Idx].Shader = ShaderType_Standard;
            LoPoMaterials[Idx].Texture = &LoPoTextures[Idx];
            LoPoMaterials[Idx].AmbientLight = Vec4(0.3f, 0.3f, 0.3f, 1.0f);
        } else {
            Error("LoPoTextures[%u] could not be loaded", Idx);
        }
    }
    
    ///

    for(u32 M=0; M<MeshFileCount; M++){
        asset_mesh *Mesh = Meshes + MeshCount;

        if(Mesh->Load(MeshFilePaths[M])){
            Mesh->ComputeAABB(Swizzling);
            // Mesh->TranslateAndScale(true, true, 1.0f);
            Mesh->ComputePerFaceAttribute(false);
            if(!Mesh->ConstructBVH()){
                Error("ConstructBVH failed");
            }
            
            MeshCount++;
            Mesh->ID = MeshCount;
            Log("V: %u, T: %u", Mesh->VertexCount, Mesh->FaceCount);
        } else {
            Success = false;
            break;
        }
    }
    
    return Success;
}

b32
kadabra::asset_manager::Finalise(){
    b32 Success = true;

    if(!NullTexture.Unload()){
        Warning("An error occured while unloading null texture");
    }

    return Success;
}
