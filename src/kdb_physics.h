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
                            
        static b32 NarrowPhaseCollision(bvh_node *BVHNode_A, component_transform *XFormA, 
                                        bvh_node *BVHNode_B, component_transform *XFormB);
    };
}

#endif

