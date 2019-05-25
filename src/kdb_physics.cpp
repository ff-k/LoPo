#include "kdb_physics.h"

void 
kadabra::component_particle::AddForce(kadabra::vec3 Force){
    TotalForce += Force;
}

void 
kadabra::component_particle::PrepareIntegration(f32 DeltaTime){
    vec3 Acceleration = Gravity;
    Acceleration += TotalForce*InverseMass;
    
#if 0
    Next_Position = Position + Velocity*DeltaTime;
    Next_Velocity = (Velocity + Acceleration*DeltaTime) * Power(Damping, DeltaTime);
#else
    Next_Velocity = (Velocity + Acceleration*DeltaTime) * Power(Damping, DeltaTime);
    Next_Position = Position + Next_Velocity*DeltaTime;
#endif
    
    // TODO(furkan): Clear TotalForce in ApplyIntegration??
    TotalForce = Vec3(0.0f, 0.0f, 0.0f);
}

void 
kadabra::component_particle::ApplyIntegration(){
    Assert(IsActive);
    
    Prev_Position = Position;
    Prev_Velocity = Velocity;
    
    Position = Next_Position;
    Velocity = Next_Velocity;
}

void 
kadabra::component_particle::Integrate(f32 DeltaTime){
    PrepareIntegration(DeltaTime);
    ApplyIntegration();
}

void 
kadabra::component_particle::UndoLastIntegration(){
    Next_Position = Position;
    Next_Velocity = Velocity;
    
    Position = Prev_Position;
    Velocity = Prev_Velocity;
}

kadabra::vec3 
kadabra::component_particle::GetDeltaPosition(){
    vec3 Result;
    
    Result = Next_Position-Position;
    
    return Result;
}

kadabra::vec3 
kadabra::component_particle::GetDeltaVelocity(){
    vec3 Result;
    
    Result = Next_Velocity-Velocity;
    
    return Result;
}

kadabra::collision_response 
kadabra::physics::Collides(asset_mesh *MeshA, mat4 *XFormA, 
                           asset_mesh *MeshB, mat4 *XFormB, 
                           vec3 RelativeDeltaPosition){
    collision_response Result;
    
    bvh_node *BVHNodeA = MeshA->BVHRoot;
    bvh_node *BVHNodeB = MeshB->BVHRoot;
    
    Assert(BVHNodeA);
    Assert(BVHNodeB);
    
    aabb BB_A = BVHNodeA->AABB;
    AABBTransformInPlace(&BB_A, XFormA);
    
    aabb BB_B = BVHNodeB->AABB;
    AABBTransformInPlace(&BB_B, XFormB);
    
    if(MeshA->IsConvex && MeshB->IsConvex){
        Result = physics::NarrowPhaseCollisionCC(MeshA, XFormA, BB_A, 
                                                 MeshB, XFormB, BB_B, 
                                                 RelativeDeltaPosition);
    } else if(MeshA->IsConvex){
        Result = physics::NarrowPhaseCollisionC(MeshA,           XFormA, BB_A, 
                                                MeshB, BVHNodeB, XFormB, BB_B, 
                                                RelativeDeltaPosition);
    } else if(MeshB->IsConvex){
        Result = physics::NarrowPhaseCollisionC(MeshB,           XFormB, BB_B, 
                                                MeshA, BVHNodeA, XFormA, BB_A, 
                                                RelativeDeltaPosition);
    } else {
        Result = physics::NarrowPhaseCollision(MeshA, BVHNodeA, XFormA, BB_A, 
                                               MeshB, BVHNodeB, XFormB, BB_B, 
                                               RelativeDeltaPosition);
    }
    
    
    
    return Result;
}

kadabra::collision_response 
kadabra::physics::NarrowPhaseCollisionCC(asset_mesh *MeshA, mat4 *XFormA, aabb BB_A, 
                                         asset_mesh *MeshB, mat4 *XFormB, aabb BB_B, 
                                         vec3 RelativeDeltaPosition){
    collision_response Result;
    Result.Collided = false;

    if(AABBsOverlap(BB_A, BB_B)){
        gjk_shape ShapeA;
        ShapeA.Mesh  = MeshA;
        ShapeA.XForm = XFormA;
        ShapeA.Type  = GJKShape_ConvexMesh;
        
        gjk_shape ShapeB;
        ShapeB.Mesh  = MeshB;
        ShapeA.XForm = XFormB;
        ShapeB.Type  = GJKShape_ConvexMesh;
        
        Result = gjk::Run(&ShapeA, &ShapeB, RelativeDeltaPosition);
    }
    
    return Result;
}

kadabra::collision_response 
kadabra::physics::NarrowPhaseCollisionC(asset_mesh *Convex   ,                          mat4 *XFormConvex, aabb BB_Convex, 
                                        asset_mesh *MeshOther, bvh_node *BVHNodeOther , mat4 *XFormOther , aabb BB_Other, 
                                        vec3 RelativeDeltaPosition){
    collision_response Result;
    Result.Collided = false;

    if(AABBsOverlap(BB_Convex, BB_Other)){
        if(BVHNodeOther->IsLeaf){
            
            // NOTE(furkan): Refer to "Collision Detection in Interactive 
            // 3D Environments" and "Game Physics Pearls" for explanation 
            // in detail. This function implements Algorithm 2 in "Smooth 
            // Mesh Contacts", Game Physics Pearls.
            //
            // Also check the 5th Chapter of "Real-Time Collision 
            // Detection" for closest point tests.
            
            u32  BaseIndexB = 3*BVHNodeOther->FaceIdx;
            
            u32  I0B = MeshOther->Indices[BaseIndexB + 0];
            u32  I1B = MeshOther->Indices[BaseIndexB + 1];
            u32  I2B = MeshOther->Indices[BaseIndexB + 2];
            
            vec3 V0B = MeshOther->Vertices[I0B].Position;
            vec3 V1B = MeshOther->Vertices[I1B].Position;
            vec3 V2B = MeshOther->Vertices[I2B].Position;
            
            TriangleTransformInPlace(&V0B, &V1B, &V2B, XFormOther);
            
            gjk_shape ShapeA;
            ShapeA.Mesh  = Convex;
            ShapeA.XForm = XFormConvex;
            ShapeA.Type  = GJKShape_ConvexMesh;
            
            gjk_shape ShapeB;
            ShapeB.V0   = V0B;
            ShapeB.V1   = V1B;
            ShapeB.V2   = V2B;
            ShapeB.Type = GJKShape_Triangle;
            
            Result = gjk::Run(&ShapeA, &ShapeB, RelativeDeltaPosition);
            
            if(Result.Collided){
                Result.CollidingFaceIdxB = BVHNodeOther->FaceIdx;
            }
            
        } else {
            aabb BB_Bl = BVHNodeOther->Left->AABB;
            aabb BB_Br = BVHNodeOther->Right->AABB;
            AABBTransformInPlace(&BB_Bl, XFormOther);
            AABBTransformInPlace(&BB_Br, XFormOther);
            
            collision_response Left  = physics::NarrowPhaseCollisionC(Convex   ,                      XFormConvex, BB_Convex, 
                                                                      MeshOther, BVHNodeOther->Left , XFormOther , BB_Bl, 
                                                                      RelativeDeltaPosition);
            collision_response Right = physics::NarrowPhaseCollisionC(Convex   ,                      XFormConvex, BB_Convex, 
                                                                      MeshOther, BVHNodeOther->Right, XFormOther , BB_Br, 
                                                                      RelativeDeltaPosition);
            
            if(Left.Collided && Right.Collided){
                if(Left.Distance < Right.Distance){
                    Result = Left;
                } else {
                    Result = Right;
                }
            } else if (Left.Collided){
                Result = Left;
            } else if (Right.Collided){
                Result = Right;
            }
        }
    }
    
    return Result;
}

kadabra::collision_response 
kadabra::physics::NarrowPhaseCollision(asset_mesh *MeshA, bvh_node *BVHNodeA, mat4 *XFormA, aabb BB_A, 
                                       asset_mesh *MeshB, bvh_node *BVHNodeB, mat4 *XFormB, aabb BB_B, 
                                       vec3 RelativeDeltaPosition){
    collision_response Result;
    Result.Collided = false;

    if(AABBsOverlap(BB_A, BB_B)){
        if(BVHNodeA->IsLeaf){
            if(BVHNodeB->IsLeaf){
                
                // NOTE(furkan): Refer to "Collision Detection in Interactive 
                // 3D Environments" and "Game Physics Pearls" for explanation 
                // in detail. This function implements Algorithm 2 in "Smooth 
                // Mesh Contacts", Game Physics Pearls.
                //
                // Also check the 5th Chapter of "Real-Time Collision 
                // Detection" for closest point tests.
                
                u32  BaseIndexA = 3*BVHNodeA->FaceIdx;
                
                u32  I0A = MeshA->Indices[BaseIndexA + 0];
                u32  I1A = MeshA->Indices[BaseIndexA + 1];
                u32  I2A = MeshA->Indices[BaseIndexA + 2];
                
                vec3 V0A = MeshA->Vertices[I0A].Position;
                vec3 V1A = MeshA->Vertices[I1A].Position;
                vec3 V2A = MeshA->Vertices[I2A].Position;
                
                u32  BaseIndexB = 3*BVHNodeB->FaceIdx;
                
                u32  I0B = MeshB->Indices[BaseIndexB + 0];
                u32  I1B = MeshB->Indices[BaseIndexB + 1];
                u32  I2B = MeshB->Indices[BaseIndexB + 2];
                
                vec3 V0B = MeshB->Vertices[I0B].Position;
                vec3 V1B = MeshB->Vertices[I1B].Position;
                vec3 V2B = MeshB->Vertices[I2B].Position;
                
                TriangleTransformInPlace(&V0A, &V1A, &V2A, XFormA);
                TriangleTransformInPlace(&V0B, &V1B, &V2B, XFormB);
                
                gjk_shape ShapeA;
                ShapeA.V0   = V0A;
                ShapeA.V1   = V1A;
                ShapeA.V2   = V2A;
                ShapeA.Type = GJKShape_Triangle;
                
                gjk_shape ShapeB;
                ShapeB.V0   = V0B;
                ShapeB.V1   = V1B;
                ShapeB.V2   = V2B;
                ShapeB.Type = GJKShape_Triangle;
                
                Result = gjk::Run(&ShapeA, &ShapeB, RelativeDeltaPosition);
                
                if(Result.Collided){
                    Result.CollidingFaceIdxA = BVHNodeA->FaceIdx;
                    Result.CollidingFaceIdxB = BVHNodeB->FaceIdx;
                }
                
            } else {
                aabb BB_Bl = BVHNodeB->Left->AABB;
                aabb BB_Br = BVHNodeB->Right->AABB;
                AABBTransformInPlace(&BB_Bl, XFormB);
                AABBTransformInPlace(&BB_Br, XFormB);
                
                collision_response Left  = physics::NarrowPhaseCollision(MeshA, BVHNodeA       , XFormA, BB_A, 
                                                                         MeshB, BVHNodeB->Left , XFormB, BB_Bl, 
                                                                         RelativeDeltaPosition);
                collision_response Right = physics::NarrowPhaseCollision(MeshA, BVHNodeA       , XFormA, BB_A, 
                                                                         MeshB, BVHNodeB->Right, XFormB, BB_Br, 
                                                                         RelativeDeltaPosition);
                
                if(Left.Collided && Right.Collided){
                    if(Left.Distance < Right.Distance){
                        Result = Left;
                    } else {
                        Result = Right;
                    }
                } else if (Left.Collided){
                    Result = Left;
                } else if (Right.Collided){
                    Result = Right;
                }
            }
        } else {
            aabb BB_Al = BVHNodeA->Left->AABB;
            aabb BB_Ar = BVHNodeA->Right->AABB;
            AABBTransformInPlace(&BB_Al, XFormA);
            AABBTransformInPlace(&BB_Ar, XFormA);
            
            collision_response Left  = physics::NarrowPhaseCollision(MeshA, BVHNodeA->Left , XFormA, BB_Al, 
                                                                     MeshB, BVHNodeB       , XFormB, BB_B, 
                                                                     RelativeDeltaPosition);
            collision_response Right = physics::NarrowPhaseCollision(MeshA, BVHNodeA->Right, XFormA, BB_Ar, 
                                                                     MeshB, BVHNodeB       , XFormB, BB_B, 
                                                                     RelativeDeltaPosition);
            
            if(Left.Collided && Right.Collided){
                if(Left.Distance < Right.Distance){
                    Result = Left;
                } else {
                    Result = Right;
                }
            } else if (Left.Collided){
                Result = Left;
            } else if (Right.Collided){
                Result = Right;
            }
        }
    }
    
    return Result;
}

kadabra::collision_response 
kadabra::gjk::Run(gjk_shape *ShapeA, gjk_shape *ShapeB, vec3 R){
    
    collision_response Result;
    
    const f32 GJKErrorTolerance = 0.0001f;
    const u32 MaxIter = 12;
    
    // TODO(furkan): T and N does not return properly
    // Disable R until this problem is solved.
    R = Vec3(0.0f, 0.0f, 0.0f);
    f32  T = 0.0f;
    vec3 S = Vec3(0.0f, 0.0f, 0.0f);
    vec3 N = Vec3(0.0f, 0.0f, 0.0f);
    vec3 V = ShapeA->VInitial() - ShapeB->VInitial();
    vec3 W[4];
    u32  K = 0;
    
    b32 Collided = true;
    u32 Iter = 0;
    f32 V_norm2 = V.x*V.x + V.y*V.y + V.z*V.z;
    while(Iter < MaxIter && Collided && 
          V_norm2 > GJKErrorTolerance){
        
        Iter++;
        
        vec3 P = ShapeA->Support(-V) - ShapeB->Support(V);
        
        f32 DotVP = Dot(V, P);
        f32 DotVR = Dot(V, R);
        if(DotVP > -GJKErrorTolerance && 
           DotVP > T*DotVR){
            if(DotVR > 0.0f){
                T = DotVP / DotVR;
                Assert(T >= 0.0f);
                
                if(T > 1.0f){
                    Collided = false;
                }
                
                S = T*R;
                K = 0;
                N = Normalize(-V);
            } else {
                Collided = false;
            }
        }
        
        Assert(K < 4);
        vec3 W_k = P-S;
        W[K] = W_k;
        K++;
        
        if(K == 1){
            V = W[0];
            Assert(K <= 1);
            
        } else if(K == 2){
            V = gjk::Simplex2(W, &K);
            Assert(K <= 2);
            
        } else if(K == 3){
            V = gjk::Simplex3(W, &K);
            Assert(K <= 3);
            
        } else if(K == 4){
            b32 Inside = false;
            V = gjk::Simplex4(W, &K, &Inside);
            if(Inside){
                V_norm2 = 0.0f;
                break;
            }
            
            Assert(K < 4);
        } else {
            Assert(!"This must be impossible");
        }
        
        V_norm2 = V.x*V.x + V.y*V.y + V.z*V.z;
    }
    
    Result.Collided = Collided;
    Result.Distance = V_norm2;
    
    return Result;
}

kadabra::vec3 
kadabra::gjk_shape::VInitial(){
    vec3 Result;
    
    switch(Type){
        case GJKShape_ConvexMesh: {
            Assert(Mesh->Indices);
            
            u32 VIdx  = Mesh->Indices[0];
            vertex *V = Mesh->Vertices + VIdx;
            vec3 P    = V->Position;
            Result    = ((*XForm)*Vec4(P, 1.0f)).xyz;
            
        } break;
        case GJKShape_Triangle: {
            Result = V0;
        } break;
        UnexpectedDefaultCase;
    }
    
    return Result;
}

kadabra::vec3 
kadabra::gjk_shape::Support(vec3 Dir){
    vec3 Result;
    
    switch(Type){
        case GJKShape_ConvexMesh: {
            Assert(!"Not implemented");
        } break;
        case GJKShape_Triangle: {
            f32 Dot0 = Dot(V0, Dir);
            f32 Dot1 = Dot(V1, Dir);
            f32 Dot2 = Dot(V2, Dir);
            
            if(Dot0 > Dot1){
                if(Dot0 > Dot2){
                    Result = V0;
                } else {
                    Result = V2;
                }
            } else {
                if(Dot1 > Dot2){
                    Result = V1;
                } else {
                    Result = V2;
                }
            }
        } break;
        UnexpectedDefaultCase;
    }
    
    return Result;
}

kadabra::vec3 
kadabra::gjk::Simplex2(vec3 *W, u32 *K){
    vec3 Result;
    
    vec3 W0toW1 = W[1] - W[0];
                            
    f32 Numer = Dot( -W[0], W0toW1);
    f32 Denom = Dot(W0toW1, W0toW1);
    if(Denom == 0.0f){
        Denom = 0.000001f;
    }
    
    f32 InterpolationT = Numer/Denom;
    if(InterpolationT <= 0.0f){
        Result = W[0];
        *K = 1;
    } else if(InterpolationT >= 1.0f){
        Result = W[1];
        W[0] = W[1];
        *K = 1;
    } else {
        Result = W[0] + InterpolationT*W[1];
    }
    
    return Result;
}

kadabra::vec3 
kadabra::gjk::Simplex3(vec3 *W, u32 *K){
    // NOTE(furkan): Adapted from "Real-Time Collision 
    // Detection", page 141
    
    vec3 Result;
    
    vec3 W0toW1 = W[1] - W[0];
    vec3 W0toW2 = W[2] - W[0];
    
    f32 d1 = Dot(W0toW1, -W[0]);
    f32 d2 = Dot(W0toW2, -W[0]);
    
    if(d1 <= 0.0f && d2 <= 0.0f){
        Result = W[0];
        *K = 1;
    } else {
        f32 d3 = Dot(W0toW1, -W[1]);
        f32 d4 = Dot(W0toW2, -W[1]);
        
        if(d3 >= 0.0f && d4 <= d3){
            Result = W[1];
            W[0] = W[1];
            *K = 1;
        } else {
            f32 vc = d1*d4 - d3*d2;
            
            if(vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f){
                f32 v = d1/(d1-d3);
                Result = W[0] + v*W0toW1;
                *K = 2;
            } else {
                f32 d5 = Dot(W0toW1, -W[2]);
                f32 d6 = Dot(W0toW2, -W[2]);
                if(d6 >= 0.0f && d5 <= d6){
                    Result = W[2];
                    W[0] = W[2];
                    *K = 1;
                } else {
                    f32 vb = d5*d2 - d1*d6;
                    
                    if(vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f){
                        f32 w = d2/(d2-d6);
                        Result = W[0] + w*W0toW2;
                        W[1] = W[2];
                        *K = 2;
                    } else {
                        f32 va = d3*d6 - d5*d4;
                        
                        f32 d43 = d4-d3;
                        f32 d56 = d5-d6;
                        if(va <= 0.0f && d43 >= 0.0f && d56 >= 0.0f){
                            f32 w = d43/(d43+d56);
                            Result = W[1] + w*(W[2]-W[1]);
                            W[0] = W[1];
                            W[1] = W[2];
                            *K = 2;
                        } else {
                            f32 Denom = 1.0f/(va+vb+vc);
                            f32 v = vb*Denom;
                            f32 w = vc*Denom;
                            Result = W[0] + W0toW1*v + W0toW2*w;
                        }
                    }
                }
            }
        }
    }
    
    return Result;
}

kadabra::vec3 
kadabra::gjk::Simplex4(vec3 *W, u32 *K, b32 *Inside){
    vec3 Result;
    
    // NOTE(furkan): Get the closest point
    vec3 IW[12];
    vec3 Intermediate[4];
    u32  IK[4];
    
    f32 MinDist = f32_Max;
    u32 ClosestIdx = u32_Max;
    for(u32 Idx=0; Idx<4; Idx++){
        
        u32 Kdx = 0;
        for(u32 Jdx=0; Jdx<4; Jdx++){
            if(Idx != Jdx){
                IW[3*Idx + Kdx] = W[Jdx];
                Kdx++;
            }
        }
        
        IK[Idx] = 3;
        Intermediate[Idx] = Simplex3(&IW[3*Idx], &IK[Idx]);
        
        f32 Dist = Length(Intermediate[Idx]);
        if(MinDist > Dist){
            ClosestIdx = Idx;
            MinDist = Dist;
        }
    }
    
    Result = Intermediate[ClosestIdx];
    
    // NOTE(furkan): Check if the origin is inside the tetrahedron
    b32 IsInside = !IsOriginAwayFromTetrahedronFace(W[0], W[1], W[2], W[3]) &&
                   !IsOriginAwayFromTetrahedronFace(W[0], W[1], W[3], W[2]) &&
                   !IsOriginAwayFromTetrahedronFace(W[0], W[2], W[3], W[1]) &&
                   !IsOriginAwayFromTetrahedronFace(W[1], W[2], W[3], W[0]);
    
    if(!IsInside){
        // NOTE(furkan): Update simplex
        u32 NewK = IK[ClosestIdx];
        for(u32 Idx=0; Idx<NewK; Idx++){
            W[Idx] = IW[3*ClosestIdx + Idx];
        }
        
        *K = NewK;
    }
    
    *Inside = IsInside;
    
    return Result;
}

b32 
kadabra::gjk::IsOriginAwayFromTetrahedronFace(vec3 V0, vec3 V1, vec3 V2, vec3 Other){
    // NOTE(furkan): V[0-2] are vertices of the 
    // face. Other is the 4th vertex of the tetrahedron
    
    vec3 FaceNormal = Cross(V1-V0, V2-V0);
    return (Dot(-V0, FaceNormal)*Dot(Other-V0, FaceNormal)) < 0.0f;
}