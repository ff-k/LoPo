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
                  
    CameraCount = 2;
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
    SpringJointCapacity = 6;
    u32 SceneObjectCount = 6 + 1 + SpringJointCapacity + SpringJointCapacity+1;
    
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
            AssetManager->SphereIdx,    // NOTE(furkan): Hand
            AssetManager->SphereIdx,    // NOTE(furkan): SpringEnd [6]
            
            AssetManager->SphereIdx,    // NOTE(furkan): Spring joints [7]-[7+SpringJointCapacity-1]
            AssetManager->CylinderIdx,  // NOTE(furkan): Spring joint connectors [7+SpringJointCapacity]-[7+SpringJointCapacity + SpringJointCapacity]
        };
        
        vec3 Position[] = {
            Vec3( 0.00f, 15.00f,  0.00f),
            Vec3( 0.00f, -2.00f,  0.00f), 
            Vec3( 0.00f,  4.30f,  0.00f),
            Vec3(-0.50f,  7.50f, 16.00f),
            Vec3( 0.00f,  7.50f,  0.00f),
            Vec3(-0.21f,  7.48f,  0.07f),
            Vec3(-0.21f,  7.48f,  0.07f),
            
            Vec3( 0.00f,  0.00f,  0.00f),
            Vec3( 0.00f,  0.00f,  0.00f),
        };
        
        vec3 EulerAngles[] = {
            Vec3(180.0f,  0.000f, 0.0f),
            Vec3(  0.0f,  0.000f, 0.0f),
            Vec3(  0.0f,  0.000f, 0.0f),
            Vec3(  0.0f,  0.000f, 0.0f),
            Vec3(  0.0f,-71.565f, 0.0f),
            Vec3(  0.0f,  0.000f, 0.0f),
            Vec3(  0.0f,  0.000f, 0.0f),
            
            Vec3(  0.0f, 0.0f, 0.0f),
            Vec3(  0.0f, 0.0f, 0.0f),
        };
        
        vec3 Scale[] = {
            Vec3(4.00f, 7.20f, 4.00f),
            Vec3(1.00f, 7.80f, 1.00f),
            Vec3(1.50f, 1.50f, 1.50f),
            Vec3(0.33f, 0.33f, 0.33f),
            Vec3(0.20f, 0.20f, 0.20f),
            Vec3(0.02f, 0.02f, 0.02f),
            Vec3(0.02f, 0.02f, 0.02f),
            
            Vec3(0.0200f, 0.0200f, 0.0200f),
            Vec3(0.0195f, 0.0200f, 0.0195f),
        };
        
        vec3 Velocity[] = {
            Vec3(  0.0f,  0.0f, 0.0f),
            Vec3(  0.0f,  0.0f, 0.0f),
            Vec3(  0.0f,  0.0f, 0.0f),
            Vec3( -1.0f,  0.0f, 0.0f),
            Vec3(  0.0f,  0.0f, 0.0f),
            Vec3(  0.0f,  0.0f, 0.0f),
            Vec3(  0.0f,  0.0f, 0.0f),
            
            Vec3(  0.0f,  0.0f, 0.0f),
            Vec3(  0.0f,  0.0f, 0.0f),
        };
        
        vec3 Gravity[] = {
            Vec3(  0.0f,  0.0f, 0.0f),
            Vec3(  0.0f,  0.0f, 0.0f),
            Vec3(  0.0f,  0.0f, 0.0f),
            Vec3(  0.0f,  0.0f, 0.0f),
            Vec3(  0.0f, -4.0f, 0.0f),
            Vec3(  0.0f, -30.0f, 0.0f),
            Vec3(  0.0f,  0.0f, 0.0f),
            
            Vec3(  0.0f, -30.0f, 0.0f),
            Vec3(  0.0f, 0.0f, 0.0f)
        };
        
        f32 Damping[] = {
            0.0f, 0.0f, 0.0f, 0.99f, 0.9f, 
            0.6f, 0.99f, 0.6f, 0.6f
        };
        
#if 1
        f32 Restitution[] = {
            0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 
            0.0f, 0.0f, 0.0f
        };
#else
        f32 Restitution[] = {
            0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
            0.0f, 0.0f, 0.0f
        };
#endif
        
        b32 PhysicsActive[] = {
            true, true, true, true, true, false, 
            false, true, false
        };
        
        b32 CanCollide[] = {
            true, true, true, true, true, false, 
            true, false, false
        };
        
        b32 EntityActive[] = {
            true, 
#if 1
            true, false, 
#else
            false, false, 
#endif
            true, true, true, 
            false, false, false
        };
        
        b32 IsStatic[] = {
            true, true, true, false, false, false, 
            false, false, false
        };
        
        for(u32 Idx=0; Idx<SceneObjectCount; Idx++){
            // if(Idx != 2 && Idx != 4){
            //     continue;
            // }

            u32 ArrIdx = Idx;
            if(ArrIdx > 6){
                if(ArrIdx > (6+SpringJointCapacity)){
                    ArrIdx = 8;
                } else {
                    ArrIdx = 7;
                }
            }
            
            component_particle *Physics = PhysicsCompos + EntityCount;
            *Physics = component_particle(Position[ArrIdx], Velocity[ArrIdx], 
                                          Gravity[ArrIdx], Damping[ArrIdx], 
                                          Restitution[ArrIdx], IsStatic[ArrIdx]);
            Physics->IsActive = PhysicsActive[ArrIdx];
            Physics->CanCollide = CanCollide[ArrIdx];
            
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
                        Vec3(0.0f, 7.9f, 1.25f),
                        Vec3(0.0f, 0.0f, 0.0f), true);
        *Camera = FreshCam;
    }
    
    Camera->UpdateAxes();
    
    if(!Freeform){
        entity *Hero = Entities + 4;
        vec3 CameraP = Hero->Transform.Position;
        CameraP += Camera->Axes.x * 0.0f;
        CameraP += Camera->Axes.y * 0.4f;
        CameraP += Camera->Axes.z * 2.5f;
        
        Camera->Transform.Position = CameraP;
    }
    
    if(Input->IsKeyWentDown(InputKey_Space)){
        vec3 CamP = Camera->Transform.Position;
        vec3 CamO = Camera->Transform.EulerRotation;
        Warning("CamP: (%f %f %f)", CamP.x, CamP.y, CamP.z);
        Warning("CamO: (%f %f %f)", CamO.x, CamO.y, CamO.z);
        Warning("Axes.x: (%f %f %f)", Camera->Axes.x.x, Camera->Axes.x.y, Camera->Axes.x.z);
        Warning("Axes.y: (%f %f %f)", Camera->Axes.y.x, Camera->Axes.y.y, Camera->Axes.y.z);
        Warning("Axes.z: (%f %f %f)", Camera->Axes.z.x, Camera->Axes.z.y, Camera->Axes.z.z);
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
kadabra::scene::FireSpring(vec3 HeroP, vec3 HeroForward, vec3 HandP){
    
    vec3 AnchorVelocity = RotateAround(HeroForward, 
                                       -45.0f, 
                                       Normalize(Cross(Vec3(0.0f, 1.0f, 0.0f), 
                                                       HeroForward)));
    
    entity *SpringAnchor = Entities + 6;
    SpringAnchor->IsActive = true;
    
    component_particle *SpringAnchorPhysics = SpringAnchor->Physics;
    SpringAnchorPhysics->Position = HandP;
    SpringAnchorPhysics->Velocity = AnchorVelocity*12.0f;
    SpringAnchorPhysics->IsActive = true;
    SpringAnchorPhysics->CanCollide = true;
}

void 
kadabra::scene::InitialiseSpring(){
    
    Entities[1].IsActive = false;
    // Entities[2].IsActive = false;
    // Entities[3].IsActive = false;
    // Entities[4].IsActive = false;
    
    //
    
    SpringActive = true;
    
    entity *Hand         = Entities + 5;
    entity *SpringAnchor = Entities + 6;
    
    SpringAnchor->Physics->Velocity = Vec3(0.0f, 0.0f, 0.0f);
    SpringAnchor->Physics->Gravity  = Vec3(0.0f, 0.0f, 0.0f);
    
    f32 SpringLengthFactor = 0.99f;
    
    vec3 SpringLength = SpringAnchor->Transform.Position - 
                        Hand->Transform.Position;
                        
    SpringLength *= SpringLengthFactor;
    vec3 Pbase = Hand->Transform.Position + (1.0f-SpringLengthFactor)*SpringLength;

    SpringJointCount = SpringJointCapacity;
    vec3 Pstep = Vec3(0.0f, 0.0f, 0.0f);
    while(SpringJointCount){
        Pstep = SpringLength*(1.0f/(f32)(SpringJointCount+1.0f));
        SpringRestLength = Length(Pstep);
        if(SpringRestLength < 0.8f){
            SpringJointCount--;
        } else {
            break;
        }
    }
    
    if(SpringJointCount == 0){
        Pstep = SpringLength;
        SpringRestLength = Length(Pstep);
    }
    
    Assert(SpringRestLength > 0.0f);
    
    // printf("SpringJointCount: %u, RestLength: %f\n", SpringJointCount, SpringRestLength);
    SpringK = 1500.0f;
    
    Hand->Transform.Position = Pbase;
    Hand->Physics->Position = Pbase;
    Hand->Physics->Velocity = Vec3(0.0f, 0.0f, 0.0f);
    Hand->Physics->TotalForce = Vec3(0.0f, 0.0f, 0.0f);
    Hand->Physics->IsActive = true;
    Hand->Physics->CanCollide = true; // TODO(furkan): This line breaks the rope
    for(u32 JointIdx=0; JointIdx<SpringJointCount; JointIdx++){
        vec3 P = Pbase + ((f32)(JointIdx+1.0f))*Pstep;
        entity *Joint = Entities + (7+JointIdx);
        Joint->Physics->Position = P;
        Joint->Physics->Velocity = Vec3(0.0f, 0.0f, 0.0f);
        Joint->Physics->TotalForce = Vec3(0.0f, 0.0f, 0.0f);
        Joint->Physics->IsActive = true;
        Joint->IsActive = true;
        // printf("Placed %u at (%f %f %f)\n", JointIdx, P.x, P.y, P.z);
    }
    
    entity *Hero = Entities + 4;
    HeroRestLength = Length(Hero->Transform.Position - Hand->Transform.Position);
    Hero->Physics->Velocity = Vec3(0.0f, 0.0f, 0.0f);
    Hero->Physics->TotalForce = Vec3(0.0f, 0.0f, 0.0f);
}

void 
kadabra::scene::ApplySpringForce(component_particle *Spring, vec3 AnchorP, f32 RestLength, f32 K){
    if(Spring->IsActive){
        vec3 SpringDistance = Spring->Position-AnchorP;
        f32 SpringLength = Length(SpringDistance);
        
        vec3 ForceDir = Normalize(SpringDistance);
        f32  ForceMag = K*(RestLength-SpringLength);
        vec3 Force    = ForceDir*ForceMag;
        
        if(isnan(Force.x) || isnan(Force.y) || isnan(Force.z)){
            Error("NaN found!");
            
            Error("%f %f %f", Spring->Position.x, Spring->Position.y, Spring->Position.z);
            Error("%f %f %f", AnchorP.x, AnchorP.y, AnchorP.z);
            Error("%f %f %f", SpringDistance.x, SpringDistance.y, SpringDistance.z);
            Error("%f %f %f", ForceDir.x, ForceDir.y, ForceDir.z);
            Error("%f %f %f", Force.x, Force.y, Force.z);
            Error("%f %f", SpringLength, ForceMag);
            exit(-1);
        }
        
        Spring->AddForce(Force);
    } else {
        Warning("ApplySpringForce called on an inactive spring");
    }
}

void 
kadabra::scene::SimulateSpring(){
    Assert(SpringActive);
    
    entity *Hero         = Entities + 4;
    entity *Hand         = Entities + 5;
    entity *SpringAnchor = Entities + 6;
    
    ApplySpringForce(Hand->Physics, Hero->Physics->Position, HeroRestLength, SpringK*(HeroRestLength/SpringRestLength));
    ApplySpringForce(Hero->Physics, Hand->Physics->Position, HeroRestLength, SpringK*(HeroRestLength/SpringRestLength));
    
    if(SpringJointCount){
        entity *FirstJoint = Entities + 7;
        
        ApplySpringForce(      Hand->Physics, FirstJoint->Physics->Position, SpringRestLength, SpringK);
        ApplySpringForce(FirstJoint->Physics,       Hand->Physics->Position, SpringRestLength, SpringK);
        
        for(u32 JointIdx=0; JointIdx<(SpringJointCount-1); JointIdx++){
            entity *NearHand   = Entities + (7+JointIdx);
            entity *NearAnchor = Entities + (8+JointIdx);
            
            ApplySpringForce(  NearHand->Physics, NearAnchor->Physics->Position, SpringRestLength, SpringK);
            ApplySpringForce(NearAnchor->Physics,   NearHand->Physics->Position, SpringRestLength, SpringK);
        }
        
        entity *LastJoint    = Entities + (6+SpringJointCount);
        
        ApplySpringForce(LastJoint->Physics, SpringAnchor->Physics->Position, SpringRestLength, SpringK);
    } else {
        ApplySpringForce(Hand->Physics, SpringAnchor->Physics->Position, SpringRestLength, SpringK);
    }
}

void 
kadabra::scene::DestroySpring(){
    SpringActive = false;
    
    entity *SpringAnchor = Entities + 6;
    SpringAnchor->IsActive = false;
    SpringAnchor->Physics->IsActive = false;
    SpringAnchor->Physics->CanCollide = false;
    
    entity *Hand = Entities + 5;
    Hand->Physics->IsActive = false;
    Hand->Physics->CanCollide = false;
    for(u32 JointIdx=0; JointIdx<SpringJointCount; JointIdx++){
        entity *Joint = Entities + (7+JointIdx);
        Joint->Physics->IsActive = false;
        Joint->IsActive = false;
    }
    
    u32 SJCCount = SpringJointCount+1;
    for(u32 SJCIdx=0; SJCIdx<SJCCount; SJCIdx++){
        entity *E = Entities + (7+SpringJointCapacity+SJCIdx);
        E->IsActive = false;
    }
    
    SpringJointCount = 0;
}

b32  
kadabra::scene::UpdatePhysics(f32 DeltaTime){
    b32 Success = true;
    
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
                if(Physics->IsActive && Physics->IsStatic == false){
                    
                    Physics->PrepareIntegration(dt);
                    Transform->Position = Physics->Next_Position;
                    vec3 P = Transform->Position;
                    if(isnan(P.x) || isnan(P.y) || isnan(P.z)){
                        Error("NaN found!");
                        
                        Error("%f %f %f", Physics->Position.x, Physics->Position.y, Physics->Position.z);
                        Error("%f %f %f", Physics->Velocity.x, Physics->Velocity.y, Physics->Velocity.z);
                        Error("%f %f %f", Physics->Gravity.x, Physics->Gravity.y, Physics->Gravity.z);
                        Error("%f %f %f", Physics->TotalForce.x, Physics->TotalForce.y, Physics->TotalForce.z);

                        Error("--------");
                        
                        Success = false;
                    }
                    
                    b32 Blocked = false;
                    if(Physics->CanCollide){
                        for(u32 Jdx=0; Jdx<EntityCount; Jdx++){
                            
                            b32 ProcessContact = false;
                            if(Idx == 3){
                                if(Jdx > 3){
                                    ProcessContact = true;
                                }
                            } else if(Idx > 3){
                                if(Jdx < 4){
                                    ProcessContact = true;
                                }
                            }
                            
                            if(ProcessContact && Idx != Jdx){
                                
                                b32 PairCollides = false;
                                
                                entity *Other = Entities + Jdx;
                                component_particle *OtherPhy = Other->Physics;
                                if(Other->IsActive && OtherPhy && 
                                   OtherPhy->IsActive && OtherPhy->CanCollide){
                                    collision_response Response = Collides(E, Other, 
                                                                           Physics, 
                                                                           OtherPhy);
                                    
                                    if(Response.Collided){
                                        // printf("Collision between %u and %u\n", Idx, Jdx);
                                        
                                        PairCollides = true;
                                        
                                        vec3 V = Physics->Velocity;
                                        f32  Vlength = Length(V);
                                        if(Vlength > 0.000001f){
                                            Assert(Other->Renderable);
                                            asset_mesh *Mesh_O = Other->Renderable->Mesh;
                                            
                                            component_transform *T_O = &Other->Transform;
                                            vec3 N = Mesh_O->FaceNormals[Response.CollidingFaceIdxB];
#if 0
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
#else
                                            // printf("Velocity Before: (%f %f %f)\n", V.x, V.y, V.z);
                                        
                                            TransformNormalInPlace(&N, T_O->EulerRotation, T_O->Scale);
                                            N = Normalize(N);
                                            
                                            // printf("Normal: (%f %f %f)\n", N.x, N.y, N.z);
                                            
                                            Assert(Abs(1.0f-Length(N)) < 0.000001f);
                                            
                                            f32  DotNV = Dot(N, V);
                                            if(DotNV > 0.0f){
                                                DotNV *= -1.0f;
                                            }
                                            
                                            vec3 Vproj = (DotNV/Vlength)*V;
                                            
                                            f32 Restitution = Physics->Restitution;
                                            V = V + (1.0f + Restitution)*Vproj;
                                            Physics->Velocity = V;
                                            
                                            f32 ResolutionPrecision = 0.0001f;
                                            vec3 ResolvedP = Physics->Position + 
                                                             N*ResolutionPrecision;
                                                             
                                            Transform->Position = Physics->Position = ResolvedP;
                                            P = Transform->Position;
                                            if(isnan(P.x) || isnan(P.y) || isnan(P.z)){
                                                Error("NaN found!");
                                                Success = false;
                                            }
                                            
                                            // printf("Velocity After: (%f %f %f)\n", V.x, V.y, V.z);
#endif
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
                                    
                                    if(Idx == 6){
                                        if(Jdx < 4){
                                            if(Jdx%2==0){
                                                if(!SpringActive){
                                                    InitialiseSpring();
                                                }
                                            } else {
                                                DestroySpring();
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    
                    if(!Blocked){
                        Physics->ApplyIntegration();
                    }
                    
                    Transform->Position = Physics->Position;
                    P = Transform->Position;
                    if(isnan(P.x) || isnan(P.y) || isnan(P.z)){
                        Error("NaN found!");
                        Success = false;
                    }
                }
            }
        }
        UpdateTimeRemaining -= dt;
        // printf("UPDATE END\n");
    }
    // printf("FRAME END\n");
    
    return Success;
}

b32 
kadabra::scene::Update(asset_manager *AssetManager, input *Input, 
                       window *Window, renderer *Renderer){
    b32 Success = true;
    
    static b32 FramePlaying = true;
    if(Input->IsKeyWentDown(InputKey_B)){
        FramePlaying = !FramePlaying;
    }
    
    b32 FrameUpdate = FramePlaying;
    if(Input->IsKeyDown(InputKey_M) || 
       Input->IsKeyWentDown(InputKey_N)){
        FrameUpdate = true;
    }
    
    entity *Hero = Entities + 4;
    entity *Hand = Entities + 5;
    
    camera *ActiveCamera = Cameras + ActiveCameraIndex;
    
    vec3 HeroP = Hero->Transform.Position;
    vec3 HeroForward = -ActiveCamera->Axes.z;
    HeroForward.y = 0.0f;
    if(Length(HeroForward) < 0.001f){
        f32 Fx = Random01()*2.0f - 1.0f;
        f32 Fz = Random01()*2.0f - 1.0f;
        
        HeroForward = Vec3(Fx, 0.0f, Fz);
    }
    
    HeroForward = Normalize(HeroForward);
    
    if(Input->IsMouseButtonWentDown(InputMouseButton_Middle)){
        if(SpringActive){
            DestroySpring();
        }
    }
    
    if(Input->IsMouseButtonWentDown(InputMouseButton_Left)){
        if(SpringActive){
            DestroySpring();
        }
        
        FireSpring(HeroP, HeroForward, Hand->Transform.Position);
    }
    
    if(FrameUpdate){
        if(SpringActive){
            entity *FirstJoint = Entities + 7;
            vec3 AccDir = HeroForward; //Normalize(FirstJoint->Physics->Velocity);
            f32  AccSensitivity = 20.0f;
            
            if(Input->IsKeyDown(InputKey_ArrowUp)){
                // Hand->Physics->Velocity = Vec3(0.0f, 0.0f, 0.0f);
                Hand->Physics->AddForce(AccDir*AccSensitivity);
            }
            
            if(Input->IsKeyDown(InputKey_ArrowDown)){
                // Hand->Physics->Velocity = Vec3(0.0f, 0.0f, 0.0f);
                Hand->Physics->AddForce(-AccDir*AccSensitivity);
            }
        }
        
        if(!UpdatePhysics(Input->DeltaTime)){
            FramePlaying = false;
        }
        
        if(Length(Hero->Physics->Velocity) > 0.01f){
            // Hero->Transform.EulerRotation.xy = EulerDegreesFromZAxis(Hero->Physics->Velocity);
            // Hero->Transform.EulerRotation.z = 0.0f;
        }
        
        if(!SpringActive){
            Hand->Transform.Position = HeroP + HeroForward*0.283f;
        } else {
            
            u32 SJCCount = SpringJointCount+1;
            for(u32 SJCIdx=0; SJCIdx<SJCCount; SJCIdx++){
                entity *E = Entities + (7+SpringJointCapacity+SJCIdx);
                
                entity *NearHand   = Entities + (7+SJCIdx-1);
                entity *NearAnchor = Entities + (7+SJCIdx);
                
                if(SJCIdx == 0){
                    NearHand = Entities + 5;   // NOTE(furkan): Hand
                }
                
                if(SJCIdx == SpringJointCount){
                    NearAnchor = Entities + 6; // NOTE(furkan): Anchor
                }
                
                vec3 NAP = NearAnchor->Transform.Position;
                vec3 NHP = NearHand->Transform.Position;
                
                vec3 Dist = NAP-NHP;
                vec3 P = NHP;

                vec2 Orient = EulerDegreesFromYAxis(Dist);
                
                E->Transform.Position = P;
                E->Transform.EulerRotation = Vec3(Orient.x, Orient.y, 0.0f);
                E->Transform.Scale.y = Length(Dist)*0.5f;
                E->IsActive = true;
            }
        }
    }
    
    if(Input->IsKeyWentDown(InputKey_C)){
        ActiveCameraIndex = ActiveCameraIndex ^ 0x1;
    }
    
    UpdateRenderer(Input, Renderer);
    UpdateCamera(Input, ActiveCamera, ActiveCameraIndex!=0);
    if(ShowGizmo){
        UpdateGizmo(Window);
    }
    
#if 1
    for(u32 Idx=0; Idx<EntityCount; Idx++){
        entity *E = Entities + Idx;
        if(E->IsActive){
            component_particle *Phy = E->Physics;
            if(Phy){
                if(Phy->IsActive){
                    vec3 Ptrans = E->Transform.Position;
                    vec3 Pphy = Phy->Position;
                    f32 Pdiff = Abs(Length(Ptrans - Pphy));
                    if(Pdiff > 0.001f){
                        Warning("Entity[%u]'s position values do not match!", Idx);
                    }
                }
            }
        }
    }
#endif
    
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
