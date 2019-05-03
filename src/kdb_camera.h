#ifndef KDB_HEADER_CAMERA
#define KDB_HEADER_CAMERA

#include "kdb_common.h"
#include "kdb_util.h"
#include "kdb_math.h"
#include "kdb_component.h"

namespace kadabra {
    
    enum camera_projection {
        CameraProjection_Perspective,
        CameraProjection_Orthographic
    };
    
    class camera {
        public:
        component_transform Transform;
        basis3 Axes;
        
        f32 FOV;
        f32 ClippingPlaneNear;
        f32 ClippingPlaneFar;
        
        camera_projection ProjectionType;
        
        camera(f32 FOV, f32 Near, f32 Far, 
               camera_projection ProjectionType=CameraProjection_Perspective, 
               vec3 Position=Vec3(0.0f, 0.0f, 0.0f), 
               vec3 Rotation=Vec3(0.0f, 0.0f, 0.0f), 
               b32 ComputeAxes=false);
               
        camera(){
            camera(45.0f, 0.1f, 1000.0f);
        }
               
        void LookAt(vec3 TargetPosition);
        void UpdateAxes();
        mat4 GetViewTranslationMatrix();
        mat4 GetViewRotationMatrix();
        mat4 GetViewMatrix();
        mat4 GetProjectionMatrix(vec2 ViewportSize);
    };

}

#endif