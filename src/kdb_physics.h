#ifndef KDB_HEADER_PHYSICS
#define KDB_HEADER_PHYSICS

#include "kdb_common.h"
#include "kdb_math.h"
#include "kdb_component.h"

namespace kadabra {
    
    const static f32 FixedDeltaTime = 0.005556f; //0.008f;
    
    struct component_particle {
        vec3 Position;
        vec3 Velocity;
        vec3 Gravity;
        f32  Damping;
        f32  Restitution;
        b32  IsStatic;
        b32  IsActive;
        
        vec3 Next_Position;
        vec3 Next_Velocity;
        vec3 Prev_Position;
        vec3 Prev_Velocity;
        
        component_particle(){
            Position    = Vec3(0.0f, 0.0f, 0.0f);
            Velocity    = Vec3(0.0f, 0.0f, 0.0f);
            Gravity     = Vec3(0.0f, 0.0f, 0.0f);
            Damping     = 0.0f;
            Restitution = 0.0f;
            IsStatic    = false;
            IsActive    = false;
            
            Prev_Position = Position;
            Prev_Velocity = Velocity;
            Next_Position = Position;
            Next_Velocity = Velocity;
        }
        
        component_particle(vec3 Position, vec3 Velocity, vec3 Gravity, 
                           f32 Damping, f32 Restitution, b32 IsStatic){
            this->Position    = Position;
            this->Velocity    = Velocity;
            this->Gravity     = Gravity;
            this->Damping     = Damping;
            this->Restitution = Restitution;
            this->IsStatic    = IsStatic;
            this->IsActive    = false;
            
            Prev_Position = Position;
            Prev_Velocity = Velocity;
            Next_Position = Position;
            Next_Velocity = Velocity;
        }
        
        void Integrate(f32 DeltaTime);
        void PrepareIntegration(f32 DeltaTime);
        void ApplyIntegration();
        void UndoLastIntegration();
        
        vec3 GetDeltaPosition();
        vec3 GetDeltaVelocity();
    };
    
    struct collision_response {
        f32 Distance;
        
        u32 CollidingFaceIdxA;
        u32 CollidingFaceIdxB;
        
        b32 Collided;
    };
    
    class physics {
        public:
        
        static collision_response Collides(asset_mesh *MeshA, mat4 *XFormA, 
                                           asset_mesh *MeshB, mat4 *XFormB, 
                                           vec3 RelativeDeltaPosition);
                            
        static collision_response NarrowPhaseCollision(asset_mesh *MeshA, bvh_node *BVHNodeA, mat4 *XFormA, 
                                                       asset_mesh *MeshB, bvh_node *BVHNodeB, mat4 *XFormB, 
                                                       vec3 RelativeDeltaPosition);
    };
    
    class gjk {
        public:
        
        static collision_response Run(vec3 V0A, vec3 V1A, vec3 V2A, 
                                      vec3 V0B, vec3 V1B, vec3 V2B, vec3 R);
        static vec3 TriangleSupport(vec3 V0, vec3 V1, vec3 V2, vec3 Dir);
        static vec3 Simplex2(vec3 *W, u32 *K);
        static vec3 Simplex3(vec3 *W, u32 *K);
        static vec3 Simplex4(vec3 *W, u32 *K, b32 *Inside);
        static b32 IsOriginAwayFromTetrahedronFace(vec3 V0, vec3 V1, vec3 V2, vec3 Other);
    };
}

#endif

