#ifndef KDB_HEADER_BVH
#define KDB_HEADER_BVH

#include "kdb_common.h"
#include "kdb_util.h"
#include "kdb_math.h"
#include "kdb_asset.h"

namespace kadabra {
    
    struct vertex;
    
    struct bvh_node {
        // TODO(furkan): This BVH structure will work on
        // triangular meshes only. Support other 
        // primitives, too
        
        aabb AABB;
        
        union {
            struct { // NOTE(furkan): Leaf
                u32 FaceIdx;
            };
            struct { // NOTE(furkan): Node
                bvh_node *Right;
                bvh_node *Left;
            };
        };
        
        u32 MaxLevel; // NOTE(furkan): Level(Leaf) == 1 && 
                      //               Level(Leaf) < Level(Root)
        b32 IsLeaf;
    };
    
    class BVH {
        public:
        static u32 BVH::ConstructFromIndexedMesh(bvh_node **Node, 
                                                 u32 *Faces, u32 FaceCount, 
                                                 vertex *Vertices, u32 VertexCount, 
                                                 u32 *Indices, u32 IndexCount, 
                                                 u32 Depth = 0);
    };
    
}

#endif