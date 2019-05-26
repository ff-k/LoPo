#ifndef KDB_HEADER_SCENE
#define KDB_HEADER_SCENE

#include "kdb_common.h"
#include "kdb_math.h"
#include "kdb_entity.h"
#include "kdb_component.h"
#include "kdb_camera.h"
#include "kdb_input.h"
#include "kdb_window.h"
#include "kdb_mouse_pick.h"

#include "kdb_physics.h"

namespace kadabra {
    
    enum entity_id_bit {
        IDBit_Ceiling        = 0x001,
        IDBit_Floor          = 0x002,
        IDBit_Platform       = 0x004,
        IDBit_FliTri         = 0x008,
        IDBit_Hero           = 0x010,
        IDBit_Hand           = 0x020,
        IDBit_Anchor         = 0x040,
        IDBit_Joint          = 0x080,
        IDBit_JointConnector = 0x100,
    };
    
    class renderer;
    
    class scene {
        public:
        
        entity *Ceiling;
        entity *Floor;
        entity *Platform;
        entity *Hero;
        entity *Hand;
        entity *SpringAnchor;
        u32 JointBaseIdx;
        u32 JointConnectorBaseIdx;
        
        u32 FliTriCount;
        u32 FliTriBaseIdx;
        
        f32 HeroRestLength;
        
        f32 SpringRestLength;
        f32 SpringK;
        u32 SpringJointCapacity;
        u32 SpringJointCount;
        b32 SpringActive;
        
        //
        
        entity Entities[SceneCapacity];
        u32 EntityCount;
        u32 ActiveEntityIndex;
        
        camera Cameras[CameraCapacityPerScene];
        u32 CameraCount;
        u32 ActiveCameraIndex;
    
        component_transform  GizmoTransform;
        component_renderable GizmoRenderable;
        f32 GizmoSphereRadius;
    
        vec2i GizmoViewportOffset;
        vec2i GizmoViewportSize;
        
        b32 ShowGizmo;
        
        void Initialise(asset_manager *AssetManager, window *Window);
        b32  Update(asset_manager *AssetManager, input *Input, 
                    window *Window, renderer *Renderer);
        
        entity *FindAvailableEntity();
        mouse_pick PickEntity(camera *Camera, input *Input, window *Window);
        
        private:
        
        void Reset();
        void OnCollision(entity *E, entity *Other);
        b32  CheckCollisionMask(entity *E, entity *Other);
        
        //
        
        void FireSpring(vec3 HeroForward, vec3 HandP);
        void InitialiseSpring();
        void ApplySpringForce(component_particle *Spring, vec3 AnchorP, 
                              f32 RestLength, f32 K);
        void SimulateSpring();
        void DestroySpring();
        void UpdateRenderer(input *Input, renderer *Renderer);
        void UpdateCamera(input *Input, camera *Camera, b32 Freeform);
        void UpdateGizmo(window *Window);
        b32  UpdatePhysics(f32 DeltaTime);
        
        collision_response Collides(entity *Entity, entity *Other, 
                                    component_particle *Physics, 
                                    component_particle *OtherPhy);
    };
    
}

#endif