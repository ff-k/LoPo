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
            Vec3( 0.0f,  7.5f,  0.0f)
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
            // if(Idx == 0 || Idx == 1 || Idx == 3){
            //     continue;
            // }
            
            component_particle *Physics = PhysicsCompos + EntityCount;
            *Physics = component_particle(Position[Idx], Velocity[Idx], Gravity[Idx], Damping[Idx], IsStatic[Idx]);
            
            component_renderable *Renderable = Renderables + EntityCount;
            Renderable->Material = &AssetManager->LoPoMaterials[Material[Idx]];
            Renderable->Mesh = AssetManager->Meshes + Material[Idx];
            Renderable->RenderMeshAABB = true;
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
    
    if(Input->IsKeyWentDown(InputKey_P)){
        
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
    
    // if(Input->IsKeyWentDown(InputKey_C)){
    //     ActiveCameraIndex = ActiveCameraIndex ^ 0x1;
    // }
    
    UpdateRenderer(Input, Renderer);
    UpdateCamera(Input, Cameras+ActiveCameraIndex, ActiveCameraIndex==0);
    if(ShowGizmo){
        UpdateGizmo(Window);
    }
    
    f32 UpdateTimeRemaining = Input->DeltaTime;
    while(UpdateTimeRemaining > 0.0f){
        f32 dt = MinOf(FixedDeltaTime, UpdateTimeRemaining);
        for(u32 Idx=0; Idx<EntityCount; Idx++){
            entity *E = Entities + Idx;
            component_particle *Physics = E->Physics;
            if(Physics){
                if(Physics->IsStatic == false){
                    Physics->Integrate(dt);
                    
                    for(u32 Jdx=0; Jdx<EntityCount; Jdx++){
                        if(Idx != Jdx){
                            entity *Other = Entities + Jdx;
                            if(Other->Physics){
                                if(Collides(E, Other)){
                                    Physics->UndoLastIntegration();
                                    
                                    // TODO(furkan): OnCollision
                                }
                            }
                        }
                    }
                    
                    component_transform *Transform = &E->Transform;
                    Transform->Position = Physics->Position;
                }
            }
        }
        UpdateTimeRemaining -= dt;
    }
    
    return Success;
}

b32
kadabra::scene::Collides(entity *Entity, entity *Other){
    
    b32 Result = false;

#if 0
    aabb *BB_E = 0;
    if(Entity->Renderable){
        if(Entity->Renderable->Mesh){
            BB_E = &Entity->Renderable->Mesh->AABB;
        }
    }
    
    aabb *BB_O = 0;
    if(Other->Renderable){
        if(Other->Renderable->Mesh){
            BB_O = &Other->Renderable->Mesh->AABB;
        }
    }
    
    if(BB_E && BB_O){
        aabb BB_E_World = *BB_E;
        component_transform *T_E = &Entity->Transform;
        AABBTransformInPlace(&BB_E_World, T_E->Position, T_E->EulerRotation, T_E->Scale);
        
        aabb BB_O_World = *BB_O;
        component_transform *T_O = &Other->Transform;
        AABBTransformInPlace(&BB_O_World, T_O->Position, T_O->EulerRotation, T_O->Scale);
        
        Result = AABBsOverlap(BB_E_World, BB_O_World);
    }
#else
    Assert(Entity->Renderable);
    Assert( Other->Renderable);

    asset_mesh *Mesh_E = Entity->Renderable->Mesh;
    asset_mesh *Mesh_O = Other->Renderable->Mesh;
    
    component_transform *T_E = &Entity->Transform;
    component_transform *T_O = &Other->Transform;
    
    Result = physics::Collides(Mesh_E, T_E, Mesh_O, T_O);
#endif
    
    return Result;
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
