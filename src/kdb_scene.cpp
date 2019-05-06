#include "kdb_util.h"
#include "kdb_math.h"
#include "kdb_ray.h"
#include "kdb_scene.h"
#include "kdb_renderer.h"

using namespace kadabra;

void 
kadabra::scene::Initialise(asset_manager *AssetManager, window *Window){
    
    ZeroStruct(this);

    camera Camera(45.0f, 0.1f, 1000.0f, 
                  CameraProjection_Perspective,
                  Vec3( 1.6f,   7.9f, -0.6f), 
                  Vec3(-5.0f, 250.0f,  0.0f), true);
                  
    CameraCount = 1;
    for(u32 CamIdx=0; CamIdx<CameraCount; CamIdx++){
        Cameras[CamIdx] = Camera;
    }
    
    ActiveCameraIndex = 0;
    
    ShowGizmo = true;
    
    GizmoTransform = component_transform();
    GizmoRenderable.Material = &AssetManager->MaterialOpaque;
    GizmoRenderable.Mesh = &AssetManager->Gizmo;
    GizmoRenderable.RenderMeshAABB = false;
    GizmoRenderable.ToBeRendered = true;
    GizmoSphereRadius = 200.0f;

    //
    
    SpringActive = false;
    SpringJointCount = 10;
    u32 SceneObjectCount = 6 + 1 + SpringJointCount;
    
    Assert(SceneObjectCount <= SceneCapacity);
    
    component_renderable *Renderables = 0;
    component_particle   *PhysicsCompos = 0;
    if(platform::MemoryAllocate((void **)&Renderables, 
                                sizeof(component_renderable)*SceneObjectCount) &&
       platform::MemoryAllocate((void **)&PhysicsCompos, 
                                sizeof(component_particle)*SceneObjectCount)){
        
        u32 Material[] = {
            AssetManager->CeilingIdx,
            AssetManager->FloorIdx,
            AssetManager->PlatformIdx,
            AssetManager->FliTriIdx,
            AssetManager->IcosphereIdx,
            AssetManager->SphereIdx,
            
            AssetManager->CubeOutwardIdx,
        };
        
        vec3 Position[] = {
            Vec3( 0.00f, 15.00f,  0.00f),
            Vec3( 0.00f,  0.00f,  0.00f), 
            Vec3( 0.00f,  4.30f,  0.00f),
            Vec3(-0.50f,  7.50f, 16.00f),
            Vec3( 0.00f,  7.48f,  0.00f),
            Vec3(-0.21f,  7.48f,  0.07f),
            
            Vec3(-0.21f,  7.48f,  0.07f),
        };
        
        vec3 EulerAngles[] = {
            Vec3(180.0f,  0.000f, 0.0f),
            Vec3(  0.0f,  0.000f, 0.0f),
            Vec3(  0.0f,  0.000f, 0.0f),
            Vec3(  0.0f,  0.000f, 0.0f),
            Vec3(  0.0f,-71.565f, 0.0f),
            Vec3(  0.0f, 0.0f, 0.0f),
            
            Vec3(  0.0f, 0.0f, 0.0f),
        };
        
        vec3 Scale[] = {
            Vec3(1.00f, 7.20f, 1.00f),
            Vec3(1.00f, 7.80f, 1.00f),
            Vec3(1.50f, 1.50f, 1.50f),
            Vec3(0.33f, 0.33f, 0.33f),
            Vec3(0.20f, 0.20f, 0.20f),
            Vec3(0.02f, 0.02f, 0.02f),
            
            Vec3(0.02f, 0.02f, 0.02f),
        };
        
        vec3 Velocity[] = {
            Vec3(  0.0f,  0.0f, 0.0f),
            Vec3(  0.0f,  0.0f, 0.0f),
            Vec3(  0.0f,  0.0f, 0.0f),
            Vec3( -1.0f,  0.0f, 0.0f),
            Vec3(  0.0f,  0.0f, 0.0f),
            Vec3(  0.0f,  0.0f, 0.0f),
            
            Vec3(  0.0f,  0.0f, 0.0f),
        };
        
        vec3 Gravity[] = {
            Vec3(  0.0f,  0.0f, 0.0f),
            Vec3(  0.0f,  0.0f, 0.0f),
            Vec3(  0.0f,  0.0f, 0.0f),
            Vec3(  0.0f,  0.0f, 0.0f),
            Vec3(  0.0f, -1.0f, 0.0f),
            Vec3(  0.0f,  0.0f, 0.0f),
            
            Vec3(  0.0f,  0.0f, 0.0f),
        };
        
        f32 Damping[] = {
            0.0f, 0.0f, 0.0f, 0.99f, 0.95f, 0.99f, 
            0.99f
        };
        
        f32 Restitution[] = {
            0.0f, 0.0f, 0.0f, 0.0, 0.0f, 0.0f, 
            0.0f
        };
        
        b32 PhysicsActive[] = {
            true, true, true, true, true, false, 
            false
        };
        
        b32 EntityActive[] = {
            true, true, true, true, true, true, 
            false
        };
        
        b32 IsStatic[] = {
            true, true, true, false, false, false, 
            false
        };
        
        for(u32 Idx=0; Idx<SceneObjectCount; Idx++){
            // if(Idx != 2 && Idx != 4){
            //     continue;
            // }
            
            u32 ArrIdx = (Idx < 6) ? Idx : 6;
            
            component_particle *Physics = PhysicsCompos + EntityCount;
            *Physics = component_particle(Position[ArrIdx], Velocity[ArrIdx], 
                                          Gravity[ArrIdx], Damping[ArrIdx], 
                                          Restitution[ArrIdx], IsStatic[ArrIdx]);
            Physics->IsActive = PhysicsActive[ArrIdx];
            
            component_renderable *Renderable = Renderables + EntityCount;
            Renderable->Material = &AssetManager->LoPoMaterials[Material[ArrIdx]];
            Renderable->Mesh = AssetManager->Meshes + Material[ArrIdx];
            Renderable->RenderMeshAABB = false;
            Renderable->ToBeRendered = true;
            
            entity *Entity = Entities + EntityCount;
            Entity->Transform = component_transform();
            Entity->Transform.Position      = Position[ArrIdx];
            Entity->Transform.EulerRotation = EulerAngles[ArrIdx];
            Entity->Transform.Scale         = Scale[ArrIdx];
            Entity->Renderable = Renderable;
            Entity->Physics = Physics;
            Entity->IsActive = EntityActive[ArrIdx];
            EntityCount++;
        }
    } else {
        Error("Renderable allocation failed");
    }
}

void 
kadabra::scene::UpdateRenderer(input *Input, renderer *Renderer){
    if(Input->IsKeyDown(InputKey_1)){
        Renderer->PolygonMode = RendererPolygonMode_Fill;
    }

    if(Input->IsKeyDown(InputKey_2)){
        Renderer->PolygonMode = RendererPolygonMode_Wireframe;
    }

    if(Input->IsKeyDown(InputKey_3)){
        Renderer->PolygonMode = RendererPolygonMode_Point;
    }
    
    if(Input->IsKeyWentDown(InputKey_4)){
        for(u32 Idx=0; Idx<EntityCount; Idx++){
            entity *E = Entities + Idx;
            component_renderable *R = E->Renderable;
            R->RenderMeshAABB = !R->RenderMeshAABB;
        }
    }
}

void 
kadabra::scene::UpdateCamera(input *Input, camera *Camera, b32 Freeform){
    
    f32 DeltaTime     = Input->DeltaTime;
    f32 MovementSpeed = DeltaTime * 25.0f;
    f32 RotationSpeed = DeltaTime * 2500.0f;
    
    if(Input->IsKeyWentDown(InputKey_O)){
        
        if(Camera->ProjectionType == CameraProjection_Perspective){
            Camera->ProjectionType = CameraProjection_Orthographic;
        } else {
            Camera->ProjectionType = CameraProjection_Perspective;
        }
    }

    if(Input->IsKeyDown(InputKey_Shift) || 
       Input->IsMouseButtonDown(InputMouseButton_Right)){
        vec2 MouseDelta = Input->GetNormalisedMousePositionDelta();

        Camera->Transform.EulerRotation.x -= MouseDelta.y * RotationSpeed;
        if(Camera->Transform.EulerRotation.x > 89.0f){
            Camera->Transform.EulerRotation.x = 89.0f;
        } else if(Camera->Transform.EulerRotation.x < -89.0f){
            Camera->Transform.EulerRotation.x = -89.0f;
        }

        if(Camera->Transform.EulerRotation.x > 89.0f){
            Camera->Transform.EulerRotation.x = 89.0f;
        }

        Camera->Transform.EulerRotation.y -= MouseDelta.x * RotationSpeed;
    }
    
    if(Input->IsKeyDown(InputKey_Ctrl) || 
       Input->IsMouseButtonDown(InputMouseButton_Middle)){
        vec2 MouseDelta = Input->GetNormalisedMousePositionDelta();
         
        Camera->Transform.Position += Camera->Axes.x * 
                                      MovementSpeed * MouseDelta.x * 100.0f;
        Camera->Transform.Position += Camera->Axes.y * 
                                      MovementSpeed * MouseDelta.y * 100.0f;
    }

    if(Input->IsKeyDown(InputKey_W)){
        Camera->Transform.Position -= Camera->Axes.z * MovementSpeed;
    }

    if(Input->IsKeyDown(InputKey_A)){
        Camera->Transform.Position -= Camera->Axes.x * MovementSpeed;
    }

    if(Input->IsKeyDown(InputKey_S)){
        Camera->Transform.Position += Camera->Axes.z * MovementSpeed;
    }

    if(Input->IsKeyDown(InputKey_D)){
        Camera->Transform.Position += Camera->Axes.x * MovementSpeed;
    }

    if(Input->IsKeyDown(InputKey_Q)){
        Camera->Transform.Position -= Camera->Axes.y * MovementSpeed;
    }

    if(Input->IsKeyDown(InputKey_E)){
        Camera->Transform.Position += Camera->Axes.y * MovementSpeed;
    }
    
    if(Input->IsKeyDown(InputKey_Home)){
        camera FreshCam(45.0f, 0.1f, 1000.0f, 
                        CameraProjection_Perspective,
                        Vec3( 1.6f,   7.9f, -0.6f), 
                        Vec3(-5.0f, 250.0f,  0.0f), true);
        *Camera = FreshCam;
    }
    
    if(!Freeform){
        vec3 CameraBoundaryMin = Vec3(-330.0f, 85.00f, -330.0f);
        vec3 CameraBoundaryMax = Vec3( 330.0f, 85.00f,  330.0f);
        
        vec3 CameraP = Camera->Transform.Position;
        if(CameraP.x < CameraBoundaryMin.x){
            CameraP.x = CameraBoundaryMin.x;
        } else if(CameraP.x > CameraBoundaryMax.x){
            CameraP.x = CameraBoundaryMax.x;
        }
        
        if(CameraP.y < CameraBoundaryMin.y){
            CameraP.y = CameraBoundaryMin.y;
        } else if(CameraP.y > CameraBoundaryMax.y){
            CameraP.y = CameraBoundaryMax.y;
        }
        
        if(CameraP.z < CameraBoundaryMin.z){
            CameraP.z = CameraBoundaryMin.z;
        } else if(CameraP.z > CameraBoundaryMax.z){
            CameraP.z = CameraBoundaryMax.z;
        }
        Camera->Transform.Position = CameraP;
    }
    
    Camera->UpdateAxes();
    
    if(Input->IsKeyWentDown(InputKey_Space)){
        vec3 CamP = Camera->Transform.Position;
        vec3 CamO = Camera->Transform.EulerRotation;
        Warning("CamP: (%f %f %f), CamO: (%f %f %f)", CamP.x, CamP.y, CamP.z, CamO.x, CamO.y, CamO.z);
    }
}

void 
kadabra::scene::UpdateGizmo(window *Window){
    s32 GizmoSize = Window->ClientSize.y/4;
    GizmoViewportOffset = Vec2i(Window->ClientSize.x-GizmoSize,
                                Window->ClientSize.y-GizmoSize);
    GizmoViewportSize = Vec2i(GizmoSize, GizmoSize);

}

void 
kadabra::scene::InitialiseSpring(){
    SpringActive = true;
    
    entity *Hand         = Entities + 5;
    entity *SpringAnchor = Entities + 6;
    
    vec3 SpringLength = SpringAnchor->Transform.Position - 
                        Hand->Transform.Position;
    vec3 Pstep = SpringLength*(1.0f/(f32)(SpringJointCount+1.0f));
    vec3 Pbase = Hand->Transform.Position;
    
    SpringRestLength = Length(Pstep);
    SpringK = 4.0f;
    
    Hand->Physics->IsActive = true;
    for(u32 JointIdx=0; JointIdx<SpringJointCount; JointIdx++){
        vec3 P = Pbase + ((f32)(JointIdx+1.0f))*Pstep;
        entity *Joint = Entities + (7+JointIdx);
        Joint->Transform.Position = P;
        Joint->Physics->Position = P;
        Joint->Physics->Gravity = Vec3(0.0f, -0.01f, 0.0f);
        Joint->Physics->IsActive = true;
        Joint->IsActive = true;
    }
}

void 
kadabra::scene::ApplySpringForce(component_particle *Spring, vec3 AnchorP){
    vec3 SpringDistance = Spring->Position-AnchorP;
    f32 SpringLength = Length(SpringDistance);
    
    vec3 ForceDir = Normalize(SpringDistance);
    f32  ForceMag = SpringK*(SpringRestLength-SpringLength);
    
    Spring->AddForce(ForceDir*ForceMag);
}

void 
kadabra::scene::SimulateSpring(){
    Assert(SpringActive);
    
    if(SpringJointCount){
        
        entity *Hand       = Entities + 5;
        entity *FirstJoint = Entities + 7;
        
        ApplySpringForce(      Hand->Physics, FirstJoint->Transform.Position);
        ApplySpringForce(FirstJoint->Physics,       Hand->Transform.Position);
        
        for(u32 JointIdx=0; JointIdx<(SpringJointCount-1); JointIdx++){
            entity *NearHand   = Entities + (7+JointIdx);
            entity *NearAnchor = Entities + (8+JointIdx);
            
            ApplySpringForce(  NearHand->Physics, NearAnchor->Transform.Position);
            ApplySpringForce(NearAnchor->Physics,   NearHand->Transform.Position);
        }
        
        entity *SpringAnchor = Entities + 6;
        entity *LastJoint    = Entities + (6+SpringJointCount);
        
        ApplySpringForce(LastJoint->Physics, SpringAnchor->Transform.Position);
    }
}

void 
kadabra::scene::DestroySpring(){
    SpringActive = false;
    
    entity *Hand = Entities + 5;
    Hand->Physics->IsActive = false;
    for(u32 JointIdx=0; JointIdx<SpringJointCount; JointIdx++){
        entity *Joint = Entities + (7+JointIdx);
        Joint->Physics->IsActive = false;
        Joint->IsActive = false;
    }
}

void 
kadabra::scene::UpdatePhysics(f32 DeltaTime){
    // printf("FRAME BEGIN\n");
    f32 UpdateTimeRemaining = DeltaTime;
    while(UpdateTimeRemaining > 0.0f){
        f32 dt = MinOf(FixedDeltaTime, UpdateTimeRemaining);
        
        // printf("UPDATE BEGIN // dt:%f\n", dt);
        if(SpringActive){
            SimulateSpring();
        }
        
        for(u32 Idx=0; Idx<EntityCount; Idx++){
            entity *E = Entities + Idx;
            component_transform *Transform = &E->Transform;
            component_particle *Physics = E->Physics;
            if(E->IsActive && Physics){
                if(Physics->IsActive && 
                   Physics->IsStatic == false){
                    
                    Physics->PrepareIntegration(dt);
                    Transform->Position = Physics->Position;
                    
                    b32 Blocked = false;
                    for(u32 Jdx=0; Jdx<EntityCount; Jdx++){
                        if(Idx != Jdx){
                            
                            b32 PairCollides = false;
                            
                            entity *Other = Entities + Jdx;
                            component_particle *OtherPhy = Other->Physics;
                            if(Other->IsActive && OtherPhy && 
                               OtherPhy->IsActive){
                                collision_response Response = Collides(E, Other, 
                                                                       Physics, 
                                                                       OtherPhy);
                                
                                if(Response.Collided){
                                    PairCollides = true;
                                    
                                    vec3 V = Physics->Velocity;
                                    f32  Vlength = Length(V);
                                    if(Vlength){
                                        Assert(Other->Renderable);
                                        asset_mesh *Mesh_O = Other->Renderable->Mesh;
                                        
                                        component_transform *T_O = &Other->Transform;
                                        vec3 N = Mesh_O->FaceNormals[Response.CollidingFaceIdxB];
                                        TransformInPlace(&N, T_O->EulerRotation, T_O->Scale);
                                        N = Normalize(N);
                                        
                                        Assert(Abs(1.0f-Length(N)) < 0.000001f);
                                        
                                        f32  DotNV = Dot(N, V);
                                        if(DotNV < 0.0f){
                                            vec3 Vproj = (DotNV/Vlength)*V;
                                        
                                            f32 Restitution = Physics->Restitution;
                                            V = V + (1.0f + Restitution)*Vproj;
                                            Physics->Velocity = V;
                                        }
                                    }
                                }
                                
                                // if(E->Renderable){
                                //     asset_mesh *Mesh_E = E->Renderable->Mesh;
                                //     if(Response.Collided){
                                //         Mesh_E->CollidingFaceIdx = Response.CollidingFaceIdxA;
                                //     } else {
                                //         Mesh_E->CollidingFaceIdx = u32_Max;
                                //     }
                                // }
                                // 
                                // if(Other->Renderable){
                                //     asset_mesh *Mesh_O = Other->Renderable->Mesh;
                                //     if(Response.Collided){
                                //         Mesh_O->CollidingFaceIdx = Response.CollidingFaceIdxB;
                                //     } else {
                                //         Mesh_O->CollidingFaceIdx = u32_Max;
                                //     }
                                // }
                            }
                            
                            if(PairCollides){
                                Blocked = true;
                                if(Idx == 6 || Jdx == 0){
                                    if(!SpringActive){
                                        InitialiseSpring();
                                    }
                                }
                            }
                        }
                    }
                    
                    if(!Blocked){
                        Physics->ApplyIntegration();
                    }
                    
                    Transform->Position = Physics->Position;
                }
            }
        }
        UpdateTimeRemaining -= dt;
        // printf("UPDATE END\n");
    }
    // printf("FRAME END\n");
}

b32 
kadabra::scene::Update(asset_manager *AssetManager, input *Input, 
                       window *Window, renderer *Renderer){
    b32 Success = true;
    
    // if(Input->IsKeyWentDown(InputKey_C)){
    //     ActiveCameraIndex = ActiveCameraIndex ^ 0x1;
    // }
    
    UpdateRenderer(Input, Renderer);
    UpdateCamera(Input, Cameras+ActiveCameraIndex, ActiveCameraIndex==0);
    if(ShowGizmo){
        UpdateGizmo(Window);
    }
    
    static b32 FramePlaying = true;
    if(Input->IsKeyWentDown(InputKey_B)){
        FramePlaying = !FramePlaying;
    }
    
    b32 FrameUpdate = FramePlaying;
    if(Input->IsKeyDown(InputKey_M) || 
       Input->IsKeyWentDown(InputKey_N)){
        FrameUpdate = true;
    }
    
    if(FrameUpdate){
        UpdatePhysics(Input->DeltaTime);
    }
    
    vec3 HeroP = Entities[4].Transform.Position;
    vec3 HeroForward = RotateAround(Vec3(0.0f, 0.0f, 1.0f), 
                                    Entities[4].Transform.EulerRotation.y,
                                    Vec3(0.0f, 1.0f, 0.0f));
    
    Entities[5].Transform.Position = HeroP + HeroForward*0.283f;
    
    if(Input->IsMouseButtonWentDown(InputMouseButton_Left)){
        if(SpringActive){
            DestroySpring();
        }
        
        Entities[6].Transform.Position = HeroP + HeroForward*0.283f;
        
        HeroForward = RotateAround(HeroForward, 
                                   -45.0f, 
                                   Normalize(Cross(Vec3(0.0f, 1.0f, 0.0f), 
                                                   HeroForward)));
        
        component_particle *Physics = Entities[6].Physics;
        Physics->Position = Entities[6].Transform.Position;
        Physics->Velocity = HeroForward*10.0f;
        
        Entities[6].IsActive = true;
        Physics->IsActive = true;
    }
    
    return Success;
}

collision_response 
kadabra::scene::Collides(entity *Entity, entity *Other, 
                         component_particle *Physics, 
                         component_particle *OtherPhy){
    
    Assert(Entity->Renderable);
    Assert( Other->Renderable);

    asset_mesh *Mesh_E = Entity->Renderable->Mesh;
    asset_mesh *Mesh_O = Other->Renderable->Mesh;
    
    component_transform *T_E = &Entity->Transform;
    component_transform *T_O = &Other->Transform;
    
    mat4 XForm_E = BuildTransformationMatrix(T_E->Position, 
                                             T_E->EulerRotation, 
                                             T_E->Scale);
    mat4 XForm_O = BuildTransformationMatrix(T_O->Position, 
                                             T_O->EulerRotation, 
                                             T_O->Scale);
    
    vec3 DeltaP_E = Physics->GetDeltaPosition();
    vec3 DeltaP_O = OtherPhy->GetDeltaPosition();
    
    vec3 RelDeltaP = DeltaP_O - DeltaP_E;
    
    collision_response Response = physics::Collides(Mesh_E, &XForm_E, 
                                                    Mesh_O, &XForm_O, 
                                                    RelDeltaP);
    
    return Response;
}

entity *
kadabra::scene::FindAvailableEntity(){
    entity *Result = 0;
    
    u32 StartIdx = EntityCount;
    u32 Idx = StartIdx;
    while(Result == 0){
        if(Idx == SceneCapacity){
            Idx = 0;
        }
            
        if(!Entities[Idx].IsActive){
            Result = Entities + Idx;
        } else {
            Idx++;
            
            if(Idx == StartIdx){
                break;
            }    
        }
    }
    
    return Result;
}

mouse_pick 
kadabra::scene::PickEntity(camera *Camera, input *Input, window *Window){
                                    
    mouse_pick Result;
    ZeroStruct(&Result);
    Result.IsEmpty = true;
    
    vec2 ViewportSize = Vec2((f32)Window->ClientSize.x, 
                             (f32)Window->ClientSize.y);
    mat4 ProjectionMatrix = Camera->GetProjectionMatrix(ViewportSize);
    mat4 ViewMatrix = Camera->GetViewMatrix();
    mat4 InverseProjMat;
    mat4 InverseViewMat;
    if(Mat4Inverse(ProjectionMatrix, &InverseProjMat) &&
       Mat4Inverse(ViewMatrix, &InverseViewMat)){
    
        vec2 NormalisedMouseP = Input->GetNormalisedMousePosition();
        
        vec3 CameraP = Camera->Transform.Position;
        
        vec4 RayClip = Vec4(2.0f*NormalisedMouseP.x - 1.0f, 
                            2.0f*NormalisedMouseP.y - 1.0f, 
                            -1.0f, 1.0f);
        
        vec4 RayEye = InverseProjMat * RayClip;
        RayEye = Vec4(RayEye.xy, -1.0f, 0.0f);
        
        vec3 RayWorld = (InverseViewMat * RayEye).xyz;
        
        vec3 RayOrigin = CameraP;
        vec3 RayDirection = Normalize(RayWorld);
        
        ray3 Ray = {RayOrigin, RayDirection};
        ray_hit ClosestHit;
        ClosestHit.t = f32_Max;
    
        for(u32 EntityIndex=0;
                EntityIndex < EntityCount;
                EntityIndex++){
            entity *Entity = Entities + EntityIndex;
            component_renderable *Renderable = Entity->Renderable;
            if(Entity->IsActive && Renderable && 
               Renderable->ToBeRendered){
                asset_mesh *Mesh = Renderable->Mesh;
                if(Mesh){
                    ray_hit Hit;
                    Hit.t = f32_Max;
    
                    u32 FaceCount = Mesh->IndexCount/3;
                    for(u32 FaceIndex=0;
                            FaceIndex < FaceCount;
                            FaceIndex++){
    
                        u32 I0 = Mesh->Indices[FaceIndex*3 + 0];
                        u32 I1 = Mesh->Indices[FaceIndex*3 + 1];
                        u32 I2 = Mesh->Indices[FaceIndex*3 + 2];
    
                        vertex *V0 = Mesh->Vertices + I0;
                        vertex *V1 = Mesh->Vertices + I1;
                        vertex *V2 = Mesh->Vertices + I2;
    
                        vec3 P0 = V0->Position;
                        vec3 P1 = V1->Position;
                        vec3 P2 = V2->Position;
    
                        if(RayTriangleIntersection(&Hit, Ray, P0, P1, P2)){
                            if(Hit.t < ClosestHit.t){
                                ClosestHit = Hit;
    
                                Result.IsEmpty = false;
                                Result.Entity = Entity;
                                Result.FaceIndex = FaceIndex;
    
                                f32 Dist0 = Length(Hit.P - P0);
                                f32 Dist1 = Length(Hit.P - P1);
                                f32 Dist2 = Length(Hit.P - P2);
    
                                if(Dist0 < Dist1){
                                    if(Dist0 < Dist2){
                                        Result.ClosestVertIdx = 0;
                                    } else {
                                        Result.ClosestVertIdx = 2;
                                    }
                                } else {
                                    if(Dist1 < Dist2){
                                        Result.ClosestVertIdx = 1;
                                    } else {
                                        Result.ClosestVertIdx = 2;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    return Result;
}
