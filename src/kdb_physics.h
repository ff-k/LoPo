#ifndef KDB_HEADER_PHYSICS
#define KDB_HEADER_PHYSICS

#include "kdb_common.h"
#include "kdb_math.h"
#include "kdb_component.h"

namespace kadabra {
    
    const static f32 FixedDeltaTime = 0.008f;
    
    struct component_particle {
        vec3 Position;
        vec3 Velocity;
        vec3 Gravity;
        f32  Damping;
        b32  IsStatic;
        
        vec3 Prev_Position;
        vec3 Prev_Velocity;
        
        component_particle(){
            Position = Vec3(0.0f, 0.0f, 0.0f);
            Velocity = Vec3(0.0f, 0.0f, 0.0f);
            Gravity  = Vec3(0.0f, 0.0f, 0.0f);
            Damping  = 0.0f;
            IsStatic = false;
            
            Prev_Position = Position;
            Prev_Velocity = Velocity;
        }
        
        component_particle(vec3 Position, vec3 Velocity, vec3 Gravity, 
                           f32 Damping, b32 IsStatic){
            this->Position = Position;
            this->Velocity = Velocity;
            this->Gravity  = Gravity;
            this->Damping  = Damping;
            this->IsStatic = IsStatic;
            
            Prev_Position = Position;
            Prev_Velocity = Velocity;
        }
        
        void Integrate(f32 DeltaTime);
        void UndoLastIntegration();
    };
    
    class physics {
        public:
        
        static b32 Collides(asset_mesh *MeshA, component_transform *XFormA, 
                            asset_mesh *MeshB, component_transform *XFormB);
                            
        static b32 NarrowPhaseCollision(asset_mesh *MeshA, bvh_node *BVHNodeA, component_transform *XFormA, 
                                        asset_mesh *MeshB, bvh_node *BVHNodeB, component_transform *XFormB);
    };
    
    class gjk {
        public:
        
        static b32  Run(vec3 V0A, vec3 V1A, vec3 V2A, vec3 V0B, vec3 V1B, vec3 V2B);
        static vec3 TriangleSupport(vec3 V0, vec3 V1, vec3 V2, vec3 Dir);
        static vec3 Simplex2(vec3 *W, u32 *K);
        static vec3 Simplex3(vec3 *W, u32 *K);
        static vec3 Simplex4(vec3 *W, u32 *K, b32 *Inside);
        static b32 IsOriginAwayFromTetrahedronFace(vec3 V0, vec3 V1, vec3 V2, vec3 Other);
    };
}

#endif

