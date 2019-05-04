#include "kdb_physics.h"

void 
kadabra::component_particle::Integrate(f32 DeltaTime){
    
    Prev_Position = Position;
    Prev_Velocity = Velocity;
    
    Position += Velocity*DeltaTime;
    
    vec3 Acceleration = Gravity;
    Velocity = (Velocity + Acceleration*DeltaTime) * Power(Damping, DeltaTime);
}

void 
kadabra::component_particle::UndoLastIntegration(){
    Position = Prev_Position;
    Velocity = Prev_Velocity;
}

b32 
kadabra::physics::Collides(asset_mesh *MeshA, component_transform *XFormA, 
                           asset_mesh *MeshB, component_transform *XFormB){
    b32 Result = false;
    
    bvh_node *BVHNode_A = MeshA->BVHRoot;
    bvh_node *BVHNode_B = MeshB->BVHRoot;
    
    Assert(BVHNode_A);
    Assert(BVHNode_B);
    
    Result = physics::NarrowPhaseCollision(BVHNode_A, XFormA, BVHNode_B, XFormB);
    
    return Result;
}

b32 
kadabra::physics::NarrowPhaseCollision(bvh_node *BVHNode_A, component_transform *XFormA, 
                                       bvh_node *BVHNode_B, component_transform *XFormB){
    b32 Result = false;

    aabb BB_A = BVHNode_A->AABB;
    AABBTransformInPlace(&BB_A, XFormA->Position, XFormA->EulerRotation, XFormA->Scale);
    
    aabb BB_B = BVHNode_B->AABB;
    AABBTransformInPlace(&BB_B, XFormB->Position, XFormB->EulerRotation, XFormB->Scale);
    
    if(AABBsOverlap(BB_A, BB_B)){
        if(BVHNode_A->IsLeaf){
            if(BVHNode_B->IsLeaf){
                Result = true; // TODO(furkan): Call GJK here
            } else {
                b32 CollidesLeft  = physics::NarrowPhaseCollision(BVHNode_A, XFormA, BVHNode_B->Left,  XFormB);
                b32 CollidesRight = physics::NarrowPhaseCollision(BVHNode_A, XFormA, BVHNode_B->Right, XFormB);
                
                Result = CollidesLeft || CollidesRight;
            }
        } else {
            b32 CollidesLeft  = physics::NarrowPhaseCollision(BVHNode_A->Left,  XFormA, BVHNode_B, XFormB);
            b32 CollidesRight = physics::NarrowPhaseCollision(BVHNode_A->Right, XFormA, BVHNode_B, XFormB);
            
            Result = CollidesLeft || CollidesRight;
        }
    }
    
    return Result;
}
