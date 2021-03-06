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
        vec3 TotalForce;
        f32  InverseMass; // TODO(furkan): Set this properly
        f32  Damping;
        f32  Restitution;
        u32  CollisionMask;
        u32  IdentifierBit;
        b32  IsStatic;
        b32  IsActive;
        b32  CanCollide;
        
        vec3 Next_Position;
        vec3 Next_Velocity;
        vec3 Prev_Position;
        vec3 Prev_Velocity;
        
        component_particle(){
            Position      = Vec3(0.0f, 0.0f, 0.0f);
            Velocity      = Vec3(0.0f, 0.0f, 0.0f);
            Gravity       = Vec3(0.0f, 0.0f, 0.0f);
            TotalForce    = Vec3(0.0f, 0.0f, 0.0f);
            InverseMass   = 10.0f;
            Damping       = 0.0f;
            Restitution   = 0.0f;
            CollisionMask = 0x0;
            IdentifierBit = 0x0;
            IsStatic      = false;
            IsActive      = false;
            
            Prev_Position = Position;
            Prev_Velocity = Velocity;
            Next_Position = Position;
            Next_Velocity = Velocity;
        }
        
        component_particle(vec3 Position, vec3 Velocity, vec3 Gravity, 
                           f32 Damping, f32 Restitution, b32 IsStatic){
            this->Position      = Position;
            this->Velocity      = Velocity;
            this->Gravity       = Gravity;
            this->TotalForce    = Vec3(0.0f, 0.0f, 0.0f);
            this->InverseMass   = 10.0f;
            this->Damping       = Damping;
            this->Restitution   = Restitution;
            this->CollisionMask = 0x0;
            this->IdentifierBit = 0x0;
            this->IsStatic      = IsStatic;
            this->IsActive      = false;
            
            Prev_Position = Position;
            Prev_Velocity = Velocity;
            Next_Position = Position;
            Next_Velocity = Velocity;
        }
        
        void AddForce(vec3 Force);
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
                            
        static collision_response NarrowPhaseCollisionCC(asset_mesh *MeshA, mat4 *XFormA, aabb BB_A, 
                                                         asset_mesh *MeshB, mat4 *XFormB, aabb BB_B, 
                                                         vec3 RelativeDeltaPosition);
                                                    
        static collision_response NarrowPhaseCollisionC(asset_mesh *Convex   , mat4 *XFormConvex, aabb BB_Convex, 
                                                        asset_mesh *MeshOther, bvh_node *BVHNodeOther, 
                                                        mat4 *XFormOther, aabb BB_Other, vec3 RelativeDeltaPosition);
                            
        static collision_response NarrowPhaseCollision(asset_mesh *MeshA, bvh_node *BVHNodeA, mat4 *XFormA, aabb BB_A, 
                                                       asset_mesh *MeshB, bvh_node *BVHNodeB, mat4 *XFormB, aabb BB_B, 
                                                       vec3 RelativeDeltaPosition);
    };
    
    enum gjk_shape_type {
        GJKShape_ConvexMesh, 
        GJKShape_Triangle,
    };
    
    struct gjk_shape {
        union {
            struct {
                vec3 V0;
                vec3 V1;
                vec3 V2;
            };
            struct {
                asset_mesh *Mesh;
                mat4       *XForm;
            };
        };
        
        gjk_shape_type Type;
        
        vec3 VInitial();
        vec3 Support(vec3 Dir);
    };
    
    class gjk {
        public:
        
        static collision_response Run(gjk_shape *ShapeA, gjk_shape *ShapeB, vec3 R);
        static vec3 Simplex2(vec3 *W, u32 *K);
        static vec3 Simplex3(vec3 *W, u32 *K);
        static vec3 Simplex4(vec3 *W, u32 *K, b32 *Inside);
        static b32 IsOriginAwayFromTetrahedronFace(vec3 V0, vec3 V1, vec3 V2, vec3 Other);
    };
}

#endif

