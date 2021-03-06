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
    
    JointBaseIdx          = 6;
    JointConnectorBaseIdx = 6+SpringJointCapacity;
    FliTriBaseIdx         = JointConnectorBaseIdx + SpringJointCapacity+1;
    
    SpringJointCapacity = 6;
    FliTriCount         = 9;
    
    u32 SceneObjectCount = 6 + SpringJointCapacity + SpringJointCapacity+1 + FliTriCount;
    
    Assert(SceneObjectCount <= SceneCapacity);
    
    component_attractor  *Attractors = 0;
    component_renderable *Renderables = 0;
    component_particle   *PhysicsCompos = 0;
    if(platform::MemoryAllocate((void **)&Attractors, 
                                sizeof(component_attractor)*FliTriCount) &&
       platform::MemoryAllocate((void **)&Renderables, 
                                sizeof(component_renderable)*SceneObjectCount) &&
       platform::MemoryAllocate((void **)&PhysicsCompos, 
                                sizeof(component_particle)*SceneObjectCount)){
        u32 Material[] = {
            AssetManager->CeilingIdx,
            AssetManager->FloorIdx,
            AssetManager->PlatformIdx,
            AssetManager->IcosphereIdx, 
            AssetManager->SphereIdx,    // NOTE(furkan): Hand
            AssetManager->SphereIdx,    // NOTE(furkan): SpringEnd
            
            AssetManager->SphereIdx,    // NOTE(furkan): Spring joints
            AssetManager->CylinderIdx,  // NOTE(furkan): Spring joint connectors
            
            AssetManager->FliTriIdx,
        };
        
        // NOTE(furkan): Idx to ArrIdx mapping:
        // [0                                         - 5                                                    ] -> Direct
        // [6                                         - 5+SpringJointCapacity                                ] -> Joint (6)
        // [6+SpringJointCapacity                     - 6+SpringJointCapacity+SpringJointCapacity+1          ] -> Joint connector (7)
        // [7+SpringJointCapacity+SpringJointCapacity - 7+SpringJointCapacity+SpringJointCapacity+FliTriCount] -> FliTri (8)
        
        JointBaseIdx          = 6;
        JointConnectorBaseIdx = 6+SpringJointCapacity;
        FliTriBaseIdx         = JointConnectorBaseIdx + SpringJointCapacity+1;
        
        u32 AttractorIdx = 0;
        for(u32 Idx=0; Idx<SceneObjectCount; Idx++){
            component_attractor *Attractor = 0;
            u32 ArrIdx = Idx;
            if(ArrIdx >= FliTriBaseIdx){
                ArrIdx = 8;
                
                Attractor = Attractors + AttractorIdx;
                AttractorIdx++;
            } else if(ArrIdx >= JointConnectorBaseIdx){
                ArrIdx = 7;
            } else if(ArrIdx >= JointBaseIdx){
                ArrIdx = 6;
            }
            
            component_renderable *Renderable = Renderables + EntityCount;
            Renderable->Material = &AssetManager->LoPoMaterials[Material[ArrIdx]];
            Renderable->Mesh = AssetManager->Meshes + Material[ArrIdx];
            
            entity *Entity = Entities + EntityCount;
            Entity->Renderable = Renderable;
            Entity->Physics = PhysicsCompos + EntityCount;
            Entity->Attractor = Attractor;
            
            EntityCount++;
        }
        
        Ceiling      = Entities + 0;
        Floor        = Entities + 1;
        Platform     = Entities + 2;
        Hero         = Entities + 3;
        Hand         = Entities + 4;
        SpringAnchor = Entities + 5;
        
        Reset();
    } else {
        Error("Renderable allocation failed");
    }
}

void 
kadabra::scene::Reset(){
    
    if(SpringActive){
        DestroySpring();
    }
    
    vec3 Position[] = {
        Vec3( 0.00f, 18.00f,  0.00f),
        Vec3( 0.00f, -4.00f,  0.00f), 
        Vec3( 0.00f,  4.30f,  0.00f),
        Vec3( 0.00f,  8.00f,  0.00f),
        Vec3(-0.21f,  7.48f,  0.07f),
        Vec3(-0.21f,  7.48f,  0.07f),
        
        Vec3( 0.00f,  0.00f,  0.00f),
        Vec3( 0.00f,  0.00f,  0.00f),
        
        Vec3(-0.50f,  7.50f, 16.00f),
    };
    
    vec3 EulerAngles[] = {
        Vec3(180.0f,  0.000f, 0.0f),
        Vec3(  0.0f,  0.000f, 0.0f),
        Vec3(  0.0f,  0.000f, 0.0f),
        Vec3(  0.0f,-71.565f, 0.0f),
        Vec3(  0.0f,  0.000f, 0.0f),
        Vec3(  0.0f,  0.000f, 0.0f),
        
        Vec3(  0.0f, 0.0f, 0.0f),
        Vec3(  0.0f, 0.0f, 0.0f),
        
        Vec3(  0.0f,  0.000f, 0.0f),
    };
    
    vec3 Scale[] = {
        Vec3(1.00f, 7.20f, 1.00f),
        Vec3(1.00f, 7.80f, 1.00f),
        Vec3(1.50f, 1.50f, 1.50f),
        Vec3(0.20f, 0.20f, 0.20f),
        Vec3(0.02f, 0.02f, 0.02f),
        Vec3(0.02f, 0.02f, 0.02f),
        
        Vec3(0.0200f, 0.0200f, 0.0200f),
        Vec3(0.0195f, 0.0200f, 0.0195f),
        
        Vec3(0.33f, 0.33f, 0.33f),
    };
    
    vec3 Velocity[] = {
        Vec3(  0.0f,  0.0f, 0.0f),
        Vec3(  0.0f,  0.0f, 0.0f),
        Vec3(  0.0f,  0.0f, 0.0f),
        Vec3(  0.0f,  0.0f, 0.0f),
        Vec3(  0.0f,  0.0f, 0.0f),
        Vec3(  0.0f,  0.0f, 0.0f),
        
        Vec3(  0.0f,  0.0f, 0.0f),
        Vec3(  0.0f,  0.0f, 0.0f),
        
        Vec3( -9.9f,  0.0f, 0.0f),
    };
    
    vec3 Gravity[] = {
        Vec3(  0.0f,  0.0f, 0.0f),
        Vec3(  0.0f,  0.0f, 0.0f),
        Vec3(  0.0f,  0.0f, 0.0f),
        Vec3(  0.0f, -4.0f, 0.0f),
        Vec3(  0.0f, -30.0f, 0.0f),
        Vec3(  0.0f,  0.0f, 0.0f),
        
        Vec3(  0.0f, -30.0f, 0.0f),
        Vec3(  0.0f, 0.0f, 0.0f),
        
        Vec3(  0.0f,  0.0f, 0.0f),
    };
    
    f32 Damping[] = {
        0.0f, 0.0f, 0.0f, 0.9f, 0.6f, 0.99f, 
        0.6f, 0.6f, 
        0.99f
    };
        
#if 1
    f32 Restitution[] = {
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
        0.0f, 0.0f, 
        0.0f,
    };
#else
    f32 Restitution[] = {
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
        0.0f, 0.0f, 0.0f
    };
#endif
    
    b32 PhysicsActive[] = {
        true, true, true, true, false, false, 
        true, false, 
        true
    };
    
    b32 CanCollide[] = {
        true, true, true, true, false, true, 
        false, false, 
        true
    };
    
    b32 EntityActive[] = {
        true, 
#if 1
        true, true, 
#else
        false, false, 
#endif
        true, true, false, 
        false, false, 
        true
    };
    
    u32 CollisionMask[] = {
        0x1FE, // NOTE(furkan): Ceiling
        0x1FD, // NOTE(furkan): Floor
        0x1FB, // NOTE(furkan): Platform
        0x00F, // NOTE(furkan): Icosphere
        0x00F, // NOTE(furkan): Hand
        0x00F, // NOTE(furkan): Anchor
        0x00F, // NOTE(furkan): Joint
        0x00F, // NOTE(furkan): JointConnector
        0x1FF, // NOTE(furkan): FliTri
    };
    
    u32 IdentifierBit[] = {
        IDBit_Ceiling, 
        IDBit_Floor, 
        IDBit_Platform, 
        IDBit_Hero, 
        IDBit_Hand, 
        IDBit_Anchor, 
        IDBit_Joint, 
        IDBit_JointConnector, 
        IDBit_FliTri, 
    };       
    
    u32 IsStatic[] = {
        true, true, true, false, false, false, 
        false, false, 
        false
    };
    
    vec3 FliTriOrigin[] = {
        Vec3(-34.47f, 8.0f,  55.33f),
        Vec3( -7.14f, 8.0f,  34.20f),
        Vec3( 41.04f, 8.0f,  43.34f),
        Vec3(-46.47f, 8.0f,  11.42f),
        Vec3(-12.29f, 8.0f,  -1.43f),
        Vec3( 21.61f, 8.0f,   1.71f),
        Vec3(-47.04f, 8.0f, -35.06f),
        Vec3( 13.72f, 8.0f, -39.91f),
        Vec3(-47.04f, 8.0f, -35.06f)
    };
    
    u32 AttractorIdx = 0;
    for(u32 Idx=0; Idx<EntityCount; Idx++){
        u32 ArrIdx = Idx;
        if(ArrIdx >= FliTriBaseIdx){
            ArrIdx = 8;
        } else if(ArrIdx >= JointConnectorBaseIdx){
            ArrIdx = 7;
        } else if(ArrIdx >= JointBaseIdx){
            ArrIdx = 6;
        }
        
        entity *Entity = Entities + Idx;
        Entity->Transform = component_transform();
        Entity->Transform.Position      = Position[ArrIdx];
        Entity->Transform.EulerRotation = EulerAngles[ArrIdx];
        Entity->Transform.Scale         = Scale[ArrIdx];
        Entity->IsActive = EntityActive[ArrIdx];
        
        component_particle *Physics = Entity->Physics;
        *Physics = component_particle(Position[ArrIdx], Velocity[ArrIdx], 
                                      Gravity[ArrIdx], Damping[ArrIdx], 
                                      Restitution[ArrIdx], IsStatic[ArrIdx]);
        Physics->CollisionMask = CollisionMask[ArrIdx];
        Physics->IdentifierBit = IdentifierBit[ArrIdx];
        Physics->IsActive = PhysicsActive[ArrIdx];
        Physics->CanCollide = CanCollide[ArrIdx];
        
        component_renderable *Renderable = Entity->Renderable;
        Renderable->RenderMeshAABB = false;
        Renderable->ToBeRendered = true;
        
        if(ArrIdx == 8){
            component_attractor *Attractor = Entity->Attractor;
            Attractor->Position = FliTriOrigin[AttractorIdx];
            AttractorIdx++;
            
            f32  Radius = 15.0f + 5.0f*Random01();
            vec3 R = Normalize(Vec3(1.0f + Random01(), 
                                    0.5f * Random01(), 
                                    1.0f + Random01())) * Radius;
            vec3 P = Attractor->Position + R;
            vec3 V = (5.0f + Random01()*4.22f)*Normalize(Cross(R, Vec3(0.0f, 1.0f, 0.0f)));
            
            Entity->Transform.Position = P;
            Physics->Position = P;
            Physics->Velocity = V;
        }
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
kadabra::scene::FireSpring(vec3 HeroForward, vec3 HandP, f32 HeroSpeed){
    
    vec3 AnchorVelocity = RotateAround(HeroForward, 
                                       -45.0f, 
                                       Normalize(Cross(Vec3(0.0f, 1.0f, 0.0f), 
                                                       HeroForward)));
    
    SpringAnchor->IsActive = true;
    
    component_particle *SpringAnchorPhysics = SpringAnchor->Physics;
    SpringAnchorPhysics->Position = HandP;
    SpringAnchorPhysics->Velocity = AnchorVelocity*MaxOf(10.0f, HeroSpeed);
    SpringAnchorPhysics->IsActive = true;
    SpringAnchorPhysics->CanCollide = true;
}

void 
kadabra::scene::InitialiseSpring(){
    
    // Floor->IsActive = false;
    Platform->IsActive = false;
    
    SpringActive = true;
    
    
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
        entity *Joint = Entities + (JointBaseIdx+JointIdx);
        Joint->Physics->Position = P;
        Joint->Physics->Velocity = Vec3(0.0f, 0.0f, 0.0f);
        Joint->Physics->TotalForce = Vec3(0.0f, 0.0f, 0.0f);
        Joint->Physics->IsActive = true;
        Joint->IsActive = true;
        // printf("Placed %u at (%f %f %f)\n", JointIdx, P.x, P.y, P.z);
    }
    
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
    
    ApplySpringForce(Hand->Physics, Hero->Physics->Position, HeroRestLength, SpringK*(HeroRestLength/SpringRestLength));
    ApplySpringForce(Hero->Physics, Hand->Physics->Position, HeroRestLength, SpringK*(HeroRestLength/SpringRestLength));
    
    if(SpringJointCount){
        
        entity *FirstJoint = Entities + JointBaseIdx;
        
        ApplySpringForce(      Hand->Physics, FirstJoint->Physics->Position, SpringRestLength, SpringK);
        ApplySpringForce(FirstJoint->Physics,       Hand->Physics->Position, SpringRestLength, SpringK);
        
        for(u32 JointIdx=0; JointIdx<(SpringJointCount-1); JointIdx++){
            entity *NearHand   = Entities + (JointBaseIdx+JointIdx);
            entity *NearAnchor = Entities + (JointBaseIdx+JointIdx+1);
            
            ApplySpringForce(  NearHand->Physics, NearAnchor->Physics->Position, SpringRestLength, SpringK);
            ApplySpringForce(NearAnchor->Physics,   NearHand->Physics->Position, SpringRestLength, SpringK);
        }
        
        entity *LastJoint = Entities + (JointBaseIdx-1+SpringJointCount);
        
        ApplySpringForce(LastJoint->Physics, SpringAnchor->Physics->Position, SpringRestLength, SpringK);
    } else {
        ApplySpringForce(Hand->Physics, SpringAnchor->Physics->Position, SpringRestLength, SpringK);
    }
}

void 
kadabra::scene::DestroySpring(){
    SpringActive = false;
    
    SpringAnchor->IsActive = false;
    SpringAnchor->Physics->IsActive = false;
    SpringAnchor->Physics->CanCollide = false;
    
    Hand->Physics->IsActive = false;
    Hand->Physics->CanCollide = false;
    for(u32 JointIdx=0; JointIdx<SpringJointCount; JointIdx++){
        entity *Joint = Entities + (JointBaseIdx+JointIdx);
        Joint->Physics->IsActive = false;
        Joint->IsActive = false;
    }
    
    u32 SJCCount = SpringJointCount+1;
    for(u32 SJCIdx=0; SJCIdx<SJCCount; SJCIdx++){
        entity *E = Entities + (JointConnectorBaseIdx+SJCIdx);
        E->IsActive = false;
    }
    
    SpringJointCount = 0;
}

b32 
kadabra::scene::CheckCollisionMask(entity *E, entity *Other){
    b32 Result;

    Assert(E->Physics);
    Assert(Other->Physics);
    
    Result = E->Physics->CollisionMask & Other->Physics->IdentifierBit;
    
    return Result;
}

void 
kadabra::scene::OnCollision(entity *E, entity *Other){
    
    Assert(E->Physics);
    Assert(Other->Physics);
    
    if(    E->Physics->IdentifierBit & IDBit_Anchor || 
       Other->Physics->IdentifierBit & IDBit_Anchor){
           
        component_particle *AnchorPhy, *ObstaclePhy;
        if(E->Physics->IdentifierBit & IDBit_Anchor){
            AnchorPhy = E->Physics;
            ObstaclePhy = Other->Physics;
        } else {
            AnchorPhy = Other->Physics;
            ObstaclePhy = E->Physics;
        }
        
        if(ObstaclePhy->IdentifierBit & (IDBit_Ceiling | IDBit_Platform)){
            if(!SpringActive){
                InitialiseSpring();
            }
        } else if (ObstaclePhy->IdentifierBit & (IDBit_Floor | IDBit_FliTri)){
            DestroySpring();
        }
    }
    
    if(    E->Physics->IdentifierBit & IDBit_Hero || 
       Other->Physics->IdentifierBit & IDBit_Hero){
        if(!(    E->Physics->IdentifierBit & IDBit_Platform || 
             Other->Physics->IdentifierBit & IDBit_Platform)){
            Reset();
        }
    }
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
                        Error("NaN found! %u", Idx);
                        
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
                            
                            entity *Other = Entities + Jdx;
                            
                            b32 ProcessContact = CheckCollisionMask(E, Other);
                            if(ProcessContact && Idx != Jdx){
                                
                                b32 PairCollides = false;
                                
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
                                    OnCollision(E, Other);
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
    
    static b32 FramePlaying = false;
    if(Input->IsKeyWentDown(InputKey_B)){
        FramePlaying = !FramePlaying;
    }
    
    b32 FrameUpdate = FramePlaying;
    if(Input->IsKeyDown(InputKey_M) || 
       Input->IsKeyWentDown(InputKey_N)){
        FrameUpdate = true;
    }
    
    if(Input->IsMouseButtonWentDown(InputMouseButton_Middle)){
        if(SpringActive){
            DestroySpring();
        }
    }
    
    camera *ActiveCamera = Cameras + ActiveCameraIndex;
    if(Input->IsMouseButtonWentDown(InputMouseButton_Left)){
        if(SpringActive){
            DestroySpring();
        }
    
        vec3 HeroForward = -ActiveCamera->Axes.z;
        FireSpring(HeroForward, Hand->Transform.Position, Length(Hero->Physics->Velocity));
    }
    
    if(FrameUpdate){
        if(SpringActive){
            vec3 AccDir = -ActiveCamera->Axes.z;
            AccDir.y = 0.0f;
            if(Length(AccDir) < 0.001f){
                f32 Fx = Random01()*2.0f - 1.0f;
                f32 Fz = Random01()*2.0f - 1.0f;
                
                AccDir = Vec3(Fx, 0.0f, Fz);
            }
            
            AccDir = Normalize(AccDir);
            
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
        
        for(u32 FTIdx=0; FTIdx<FliTriCount; FTIdx++){
            entity *FliTri = Entities + (FliTriBaseIdx + FTIdx);
            component_particle *FliTriPhy = FliTri->Physics;
            f32 FliTriSpeed = Length(FliTriPhy->Velocity);
            vec3 Centripetal = (1.0f/ FliTriPhy->InverseMass)*
                            FliTriSpeed * FliTriSpeed *
                            (-FliTriPhy->Position + FliTri->Attractor->Position);
            FliTriPhy->AddForce(Input->DeltaTime*Centripetal);
        }
        
        
        
        if(!UpdatePhysics(Input->DeltaTime)){
            FramePlaying = false;
        }
        
        for(u32 FTIdx=0; FTIdx<FliTriCount; FTIdx++){
            entity *FliTri = Entities + (FliTriBaseIdx + FTIdx);
            component_particle *FliTriPhy = FliTri->Physics;
            if(Length(FliTriPhy->Velocity) > 0.001f){
                vec2 Orient = EulerDegreesFromYAxis(FliTriPhy->Velocity);
                FliTri->Transform.EulerRotation = Vec3(90.0f-Orient.x, Orient.y+90.0f, 0.0f);
            }
        }
        
        if(!SpringActive){
            vec3 HeroForward = -ActiveCamera->Axes.z;
            Hand->Transform.Position = Hero->Transform.Position + HeroForward*0.283f;
            
            if(SpringAnchor->IsActive){
                
                vec3 Dist = SpringAnchor->Transform.Position - 
                            Hand->Transform.Position;

                vec2 Orient = EulerDegreesFromYAxis(Dist);
                
                entity *JointConnector = Entities + (JointConnectorBaseIdx);
                JointConnector->Transform.Position = Hand->Transform.Position;
                JointConnector->Transform.EulerRotation = Vec3(Orient.x, Orient.y, 0.0f);
                JointConnector->Transform.Scale.y = Length(Dist)*0.5f;
                JointConnector->IsActive = true;
            }
        } else {
            
            u32 SJCCount = SpringJointCount+1;
            for(u32 SJCIdx=0; SJCIdx<SJCCount; SJCIdx++){
                entity *E = Entities + (JointConnectorBaseIdx+SJCIdx);
                
                entity *NearHand   = Entities + (JointBaseIdx+SJCIdx-1);
                entity *NearAnchor = Entities + (JointBaseIdx+SJCIdx);
                
                if(SJCIdx == 0){
                    NearHand = Hand;
                }
                
                if(SJCIdx == SpringJointCount){
                    NearAnchor = SpringAnchor;
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
        
        if(Hero->Transform.Position.y < Floor->Transform.Position.y){
            Reset();
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
