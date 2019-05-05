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

    
    u32 SceneObjectCount = 5;
    
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
            AssetManager->IcosphereIdx
        };
        
        vec3 Position[] = {
            Vec3( 0.0f, 15.0f,  0.0f),
            Vec3( 0.0f,  0.0f,  0.0f), 
            Vec3( 0.0f,  4.3f,  0.0f),
            Vec3(-0.5f,  7.5f, 16.0f),
            Vec3( 0.0f,  8.0f,  0.0f)
        };
        
        vec3 EulerAngles[] = {
            Vec3(180.0f, 0.0f, 0.0f),
            Vec3(  0.0f, 0.0f, 0.0f),
            Vec3(  0.0f, 0.0f, 0.0f),
            Vec3(  0.0f, 0.0f, 0.0f),
            Vec3(  0.0f, 0.0f, 0.0f)
        };
        
        vec3 Scale[] = {
            Vec3(1.00f, 7.20f, 1.00f),
            Vec3(1.00f, 7.80f, 1.00f),
            Vec3(1.50f, 1.50f, 1.50f),
            Vec3(0.33f, 0.33f, 0.33f),
            Vec3(0.20f, 0.20f, 0.20f)
        };
        
        vec3 Velocity[] = {
            Vec3(  0.0f,  0.0f, 0.0f),
            Vec3(  0.0f,  0.0f, 0.0f),
            Vec3(  0.0f,  0.0f, 0.0f),
            Vec3( -1.0f,  0.0f, 0.0f),
            Vec3(  0.0f,  0.0f, 0.0f),
        };
        
        vec3 Gravity[] = {
            Vec3(  0.0f,  0.0f, 0.0f),
            Vec3(  0.0f,  0.0f, 0.0f),
            Vec3(  0.0f,  0.0f, 0.0f),
            Vec3(  0.0f,  0.0f, 0.0f),
            Vec3(  0.0f, -1.0f, 0.0f),
        };
        
        f32 Damping[] = {
            0.0f, 0.0f, 0.0f, 0.99f, 0.95f
        };
        
        b32 IsStatic[] = {
            true, true, true, false, false
        };
        
        for(u32 Idx=0; Idx<SceneObjectCount; Idx++){
            // if(Idx == 0){
            //     continue;
            // }
            // 
            // if(Idx == 1){
            //     continue;
            // }
            // 
            // if(Idx == 2){
            //     continue;
            // }
            // 
            // if(Idx == 3){
            //     continue;
            // }
            // 
            // if(Idx == 4){
            //     continue;
            // }
            
            component_particle *Physics = PhysicsCompos + EntityCount;
            *Physics = component_particle(Position[Idx], Velocity[Idx], Gravity[Idx], Damping[Idx], IsStatic[Idx]);
            
            component_renderable *Renderable = Renderables + EntityCount;
            Renderable->Material = &AssetManager->LoPoMaterials[Material[Idx]];
            Renderable->Mesh = AssetManager->Meshes + Material[Idx];
            Renderable->RenderMeshAABB = false;
            Renderable->ToBeRendered = true;
            
            entity *Entity = Entities + EntityCount;
            Entity->Transform = component_transform();
            Entity->Transform.Position      = Position[Idx];
            Entity->Transform.EulerRotation = EulerAngles[Idx];
            Entity->Transform.Scale         = Scale[Idx];
            Entity->Renderable = Renderable;
            Entity->Physics = Physics;
            Entity->IsActive = true;
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

b32 
kadabra::scene::Update(asset_manager *AssetManager, input *Input, 
                       window *Window, renderer *Renderer){
    b32 Success = true;
    
    // TODO(furkan): Pause, NextFrame, PlayIfPressed etc
    static b32 FramePlaying = true;
    if(Input->IsKeyWentDown(InputKey_B)){
        FramePlaying = !FramePlaying;
    }
    
    b32 FrameUpdate = FramePlaying;
    if(Input->IsKeyDown(InputKey_M) || 
       Input->IsKeyWentDown(InputKey_N)){
        FrameUpdate = true;
    }
    
    // if(Input->IsKeyWentDown(InputKey_C)){
    //     ActiveCameraIndex = ActiveCameraIndex ^ 0x1;
    // }
    
    UpdateRenderer(Input, Renderer);
    UpdateCamera(Input, Cameras+ActiveCameraIndex, ActiveCameraIndex==0);
    if(ShowGizmo){
        UpdateGizmo(Window);
    }
    
    if(FrameUpdate){
        // printf("FRAME BEGIN\n");
        f32 UpdateTimeRemaining = Input->DeltaTime;
        while(UpdateTimeRemaining > 0.0f){
            f32 dt = MinOf(FixedDeltaTime, UpdateTimeRemaining);
            
            // printf("UPDATE BEGIN // dt:%f\n", dt);
            for(u32 Idx=0; Idx<EntityCount; Idx++){
                entity *E = Entities + Idx;
                component_particle *Physics = E->Physics;
                if(Physics){
                    if(Physics->IsStatic == false){
                        
                        Physics->PrepareIntegration(dt);
                        
                        b32 Blocked = false;
                        for(u32 Jdx=0; Jdx<EntityCount; Jdx++){
                            if(Idx != Jdx){
                                entity *Other = Entities + Jdx;
                                component_particle *OtherPhy = Other->Physics;
                                if(OtherPhy){
                                    collision_response Response = Collides(E, Other, 
                                                                           Physics, 
                                                                           OtherPhy);
                                    
                                    if(Response.Collided){
                                        Blocked = true;
                                        
                                        Assert(Other->Renderable);
                                        asset_mesh *Mesh_O = Other->Renderable->Mesh;
                                        vec3 N = Mesh_O->FaceNormals[Response.CollidingFaceIdxB];
                                        
                                        Assert(Abs(1.0f-Length(N)) < 0.000001f);
                                        
                                        Physics->Position += N*0.01f;
                                        
                                        vec3 V = Physics->Velocity;
                                        f32  Vlength = Length(V);
                                        if(Vlength){
                                            vec3 Vproj = (Dot(N, V)/Vlength)*V;
                                            
                                            f32 Restitution = 1.0f;
                                            V = V + (1.0f + Restitution)*Vproj;
                                            Physics->Velocity = V;
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
                            }
                        }
                        
                        if(!Blocked){
                            Physics->ApplyIntegration();
                        }
                        
                        component_transform *Transform = &E->Transform;
                        Transform->Position = Physics->Position;
                    }
                }
            }
            UpdateTimeRemaining -= dt;
            // printf("UPDATE END\n");
        }
        // printf("FRAME END\n");
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
    
    vec3 DeltaP_E = Physics->GetDeltaPosition();
    vec3 DeltaP_O = OtherPhy->GetDeltaPosition();
    
    vec3 RelDeltaP = DeltaP_O - DeltaP_E;
    
    collision_response Response = physics::Collides(Mesh_E, T_E, 
                                                    Mesh_O, T_O, 
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
