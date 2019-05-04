#include "kdb_bvh.h"
#include "kdb_platform.h"

u32 
kadabra::bvh::ConstructFromIndexedMesh(bvh_node **Node, 
                                       u32 *Faces, u32 FaceCount, 
                                       vertex *Vertices, u32 VertexCount, 
                                       u32 *Indices, u32 IndexCount, 
                                       u32 Depth){
    u32 MaxLevel = 0;
    
    Assert(Node);
    Assert((*Node) == 0);
    
    Assert(FaceCount);
    
    // static u32 AllocCount;
    // 
    // if(Depth == 0){
    //     AllocCount = 0;
    // }
    
    if(platform::MemoryAllocate((void **)Node, sizeof(bvh_node))){
        // AllocCount++;
        
        vec3 Min = Vec3(f32_Max, f32_Max, f32_Max);
        vec3 Max = Vec3(f32_Min, f32_Min, f32_Min);
        for(u32 FaceIdx=0; FaceIdx<FaceCount; FaceIdx++){
            
            u32 TripleFaceIdx = 3*Faces[FaceIdx];
            
            for(u32 Idx=0; Idx<3; Idx++){
                u32 I  = Indices[TripleFaceIdx + Idx];
                vec3 P = Vertices[I].Position;
                
                if(Min.x > P.x){ Min.x = P.x; }
                if(Min.y > P.y){ Min.y = P.y; }
                if(Min.z > P.z){ Min.z = P.z; }
                if(Max.x < P.x){ Max.x = P.x; }
                if(Max.y < P.y){ Max.y = P.y; }
                if(Max.z < P.z){ Max.z = P.z; }
            }
        }
        (*Node)->AABB = { Min, Max };
        
        if(FaceCount == 1){
            (*Node)->FaceIdx = Faces[0];
            (*Node)->MaxLevel = 1;
            (*Node)->IsLeaf  = true;
        } else {
            vec3 Center = (Min+Max)*0.5f;
            u32 Dim = Depth%3;
            f32 SplitAt = *(((f32 *)&Center) + Dim); 

            u32 FaceCountLeft = 0;
            for(u32 FaceIdx = 1;
                    FaceIdx < FaceCount;
                    FaceIdx++){
                u32 TripleFaceIdx = 3*Faces[FaceIdx];
                u32  I0 = Indices[TripleFaceIdx + 0];
                vec3 P0 = Vertices[I0].Position;
                
                f32 Pos = *(((f32 *)&P0) + Dim);
                if(Pos < SplitAt){
                    u32 SwapTemp = Faces[FaceCountLeft];
                    Faces[FaceCountLeft] = Faces[FaceIdx];
                    Faces[FaceIdx] = SwapTemp;

                    FaceCountLeft++;
                }
            }
            
            if(FaceCountLeft == 0 || FaceCountLeft == FaceCount){
                // NOTE(furkan): Divide them by half if 
                // they are all on the exact same point
                FaceCountLeft = FaceCount/2;
            }
            
            u32 NextDepth = Depth + 1;
            
            (*Node)->Left = 0;
            u32 LevelL = bvh::ConstructFromIndexedMesh(&(*Node)->Left, 
                                                       Faces, FaceCountLeft, 
                                                       Vertices, VertexCount, 
                                                       Indices, IndexCount, 
                                                       NextDepth);

            (*Node)->Right = 0;
            u32 LevelR = bvh::ConstructFromIndexedMesh(&(*Node)->Right, 
                                                       Faces+FaceCountLeft, 
                                                       FaceCount-FaceCountLeft, 
                                                       Vertices, VertexCount, 
                                                       Indices, IndexCount, 
                                                       NextDepth);
            
            MaxLevel = LevelL;
            if(MaxLevel < LevelR){ MaxLevel = LevelR; }
            MaxLevel++;
            
            (*Node)->MaxLevel = MaxLevel;
            (*Node)->IsLeaf = false;
        }
    } else {
        MaxLevel = 0;
        Error("BVH construction has failed");
    }
    
    // if(Depth == 0){
    //     printf("AllocCount: %5u, AllocBytes: %6u, ", AllocCount, (AllocCount*sizeof(bvh_node)));
    // }
    
    return MaxLevel;
}
