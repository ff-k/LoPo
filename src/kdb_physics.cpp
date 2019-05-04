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
    
    bvh_node *BVHNodeA = MeshA->BVHRoot;
    bvh_node *BVHNodeB = MeshB->BVHRoot;
    
    Assert(BVHNodeA);
    Assert(BVHNodeB);
    
    Result = physics::NarrowPhaseCollision(MeshA, BVHNodeA, XFormA, MeshB, BVHNodeB, XFormB);
    
    return Result;
}

b32 
kadabra::physics::NarrowPhaseCollision(asset_mesh *MeshA, bvh_node *BVHNodeA, component_transform *XFormA, 
                                       asset_mesh *MeshB, bvh_node *BVHNodeB, component_transform *XFormB){
    b32 Result = false;

    aabb BB_A = BVHNodeA->AABB;
    AABBTransformInPlace(&BB_A, XFormA->Position, XFormA->EulerRotation, XFormA->Scale);
    
    aabb BB_B = BVHNodeB->AABB;
    AABBTransformInPlace(&BB_B, XFormB->Position, XFormB->EulerRotation, XFormB->Scale);
    
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
                
                TriangleTransformInPlace(&V0A, &V1A, &V2A, XFormA->Position, XFormA->EulerRotation, XFormA->Scale);
                TriangleTransformInPlace(&V0B, &V1B, &V2B, XFormB->Position, XFormB->EulerRotation, XFormB->Scale);
                
                Result = gjk::Run(V0A, V1A, V2A, V0B, V1B, V2B);
                
            } else {
                b32 CollidesLeft  = physics::NarrowPhaseCollision(MeshA, BVHNodeA, XFormA, MeshB, BVHNodeB->Left,  XFormB);
                b32 CollidesRight = physics::NarrowPhaseCollision(MeshA, BVHNodeA, XFormA, MeshB, BVHNodeB->Right, XFormB);
                
                Result = CollidesLeft || CollidesRight;
            }
        } else {
            b32 CollidesLeft  = physics::NarrowPhaseCollision(MeshA, BVHNodeA->Left,  XFormA, MeshB, BVHNodeB, XFormB);
            b32 CollidesRight = physics::NarrowPhaseCollision(MeshA, BVHNodeA->Right, XFormA, MeshB, BVHNodeB, XFormB);
            
            Result = CollidesLeft || CollidesRight;
        }
    }
    
    return Result;
}

b32 
kadabra::gjk::Run(vec3 V0A, vec3 V1A, vec3 V2A, vec3 V0B, vec3 V1B, vec3 V2B){
    
    b32 Result = true;
    
    const u32 MaxIter = 64;
    const f32 GJKErrorTolerance = 0.000001f;
                
    // TODO(furkan): Get translations for both 
    // meshes and compute R
    vec3 R = Vec3(0.0f, 0.0f, 0.0f);
    
    f32  T = 0;
    vec3 S = Vec3(0.0f, 0.0f, 0.0f);
    vec3 N = Vec3(0.0f, 0.0f, 0.0f);
    vec3 V = V0A - V0B;
    vec3 W[4];
    u32  K = 0;
    
    u32 Iter = 0;
    f32  V_norm2 = V.x*V.x + V.y*V.y + V.z*V.z;
    while(Result && Iter < MaxIter && 
          V_norm2 > GJKErrorTolerance){
        
        vec3 P = TriangleSupport(V0A, V1A, V2A, -V) - 
                 TriangleSupport(V0B, V1B, V2B,  V);
        
        f32 DotVP = Dot(V, P);
        f32 DotVR = Dot(V, R);
        if(DotVP > T*DotVR){
            if(DotVR > 0.0f){
                T = DotVP / DotVR;
                Assert(T >= 0.0f);
                
                if(T > 1.0f){
                    Result = false;
                }
                
                S = T*R;
                K = 0;
                N = -V;
            } else {
                Result = false;
            }
        }
        
        Assert(K < 4);
        vec3 W_k = P-S;
        W[K] = W_k;
        K++;
        
        if(K == 1){
            V = W[0];
        } else if(K == 2){
            V = gjk::Simplex2(W, &K);
        } else if(K == 3){
            V = gjk::Simplex3(W, &K);
        } else if(K == 4){
            b32 Inside = false;
            V = gjk::Simplex4(W, &K, &Inside);
            if(Inside){
                break;
            }
        } else {
            Assert(!"This must be impossible");
        }
        
        Iter++;
        V_norm2 = V.x*V.x + V.y*V.y + V.z*V.z;
    }
    
    if(Iter == MaxIter){
        Error("GJK iterated MaxIter times\n");
    }
    
    if(Result){
        // printf("Collision normal: (%f %f %f)\n", N.x, N.y, N.z);
    }
    
    return Result;
}

kadabra::vec3 
kadabra::gjk::TriangleSupport(vec3 V0, vec3 V1, vec3 V2, vec3 Dir){
    
    vec3 Result;
    
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
    
    IsInside = *Inside;
    
    return Result;
}

b32 
kadabra::gjk::IsOriginAwayFromTetrahedronFace(vec3 V0, vec3 V1, vec3 V2, vec3 Other){
    // NOTE(furkan): V[0-2] are vertices of the 
    // face. Other is the 4th vertex of the tetrahedron
    
    vec3 FaceNormal = Cross(V1-V0, V2-V0);
    return (Dot(-V0, FaceNormal)*Dot(Other-V0, FaceNormal)) < 0.0f;
}