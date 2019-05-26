#ifndef KDB_HEADER_COMPONENT
#define KDB_HEADER_COMPONENT

#include "kdb_common.h"
#include "kdb_math.h"
#include "kdb_asset.h"

namespace kadabra {

    struct component_transform {
        vec3 Position;
        vec3 EulerRotation;
        vec3 Scale;
        
        component_transform(){
            
            Position      = Vec3(0.0f, 0.0f, 0.0f);
            EulerRotation = Vec3(0.0f, 0.0f, 0.0f);
            Scale         = Vec3(1.0f, 1.0f, 1.0f);
            
        }
    };
    
    struct component_renderable {
        asset_material *Material;
        asset_mesh *Mesh;
    
        b32 RenderMeshAABB;
        b32 ToBeRendered;
    };
    
    struct component_attractor {
        vec3 Position;
    };
    
}

#endif