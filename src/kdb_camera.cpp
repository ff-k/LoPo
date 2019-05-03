#include "kdb_camera.h"
#include "kdb_util.h"

using namespace kadabra;

kadabra::camera::camera(f32 FOV, f32 Near, f32 Far, 
                        camera_projection ProjectionType, 
                        vec3 Position, vec3 Rotation,
                        b32 ComputeAxes){

    this->Transform = component_transform();
    this->Transform.Position = Position;
    this->Transform.EulerRotation = Rotation;
    
    this->FOV = FOV;
    this->ClippingPlaneNear = Near;
    this->ClippingPlaneFar = Far;
    
    this->ProjectionType = ProjectionType;
   
    if(ComputeAxes){
        mat4 ModelMatrix = Mat4Identity();        
        ModelMatrix = Mat4Rotate(ModelMatrix, Rotation.z,
                                        RotationAxis_Z);
        ModelMatrix = Mat4Rotate(ModelMatrix, Rotation.y,
                                        RotationAxis_Y);
        ModelMatrix = Mat4Rotate(ModelMatrix, Rotation.x,
                                        RotationAxis_X);
        
        this->Axes.x = (ModelMatrix * Vec4(Vec3(1.0f , 0.0f, 0.0f), 1.0f)).xyz;
        this->Axes.y = (ModelMatrix * Vec4(Vec3(0.0f , 1.0f, 0.0f), 1.0f)).xyz;
        this->Axes.z = (ModelMatrix * Vec4(Vec3(0.0f , 0.0f, 1.0f), 1.0f)).xyz;
    }
}

void 
kadabra::camera::LookAt(vec3 TargetPosition){

    vec3 Distance = Transform.Position - TargetPosition;
    f32 X = Distance.x;
    f32 Y = Distance.y;
    f32 Z = Distance.z;
    f32 XZHypo = Sqrt(X*X + Z*Z);

    Transform.EulerRotation.y = RadianToDegree(Atan(Z, X))-90.0f;
    Transform.EulerRotation.x = RadianToDegree(Atan(-Y, XZHypo));
}

void
kadabra::camera::UpdateAxes(){

    vec3 BaseCameraTarget = Vec3(0.0f, 0.0f, 1.0f);
    vec3 BaseCameraUp     = Vec3(0.0f, 1.0f, 0.0f);
    
    f32 RotationHorizontal = Transform.EulerRotation.y;
    f32 RotationVertical  = Transform.EulerRotation.x;
    
    vec3 TargetRotatedHorizontally = RotateAround(BaseCameraTarget, 
                                                  -RotationHorizontal,
                                                  BaseCameraUp);
    
    vec3 CameraRight = Normalize(Cross(BaseCameraUp, 
                                       TargetRotatedHorizontally));
    
    vec3 CameraTarget = RotateAround(TargetRotatedHorizontally, 
                                     RotationVertical, 
                                     CameraRight);
                                     
    vec3 CameraUp = Normalize(Cross(CameraTarget, 
                                    CameraRight));
    
    Axes.x = CameraRight;
    Axes.y = CameraUp;
    Axes.z = CameraTarget;

}

mat4
kadabra::camera::GetViewTranslationMatrix(){

    mat4 Result = Mat4Identity(); 

    Result.E[0][3] = -Transform.Position.x;
    Result.E[1][3] = -Transform.Position.y;
    Result.E[2][3] = -Transform.Position.z;

    return Result;
}

mat4
kadabra::camera::GetViewRotationMatrix(){

    UpdateAxes();

    mat4 Result;
    ZeroStruct(&Result);
    Result.E[0][0] = Axes.x.x;
    Result.E[0][1] = Axes.x.y;
    Result.E[0][2] = Axes.x.z;

    Result.E[1][0] = Axes.y.x;
    Result.E[1][1] = Axes.y.y;
    Result.E[1][2] = Axes.y.z;

    Result.E[2][0] = Axes.z.x;
    Result.E[2][1] = Axes.z.y;
    Result.E[2][2] = Axes.z.z;
    
    Result.E[3][3] = 1.0f;

    return Result;
}

mat4
kadabra::camera::GetViewMatrix(){
    mat4 ViewTranslation = GetViewTranslationMatrix();
    mat4 ViewRotation = GetViewRotationMatrix();

    mat4 ViewMatrix = ViewRotation * ViewTranslation;

    return ViewMatrix;
}

mat4
kadabra::camera::GetProjectionMatrix(vec2 ViewportSize){
    // TODO(furkan): Cache this matrix.
    // However, note that window resize event should
    // be considered when implementing this cache
    // mechanism.

    mat4 Result;
    
    switch(ProjectionType){
        case CameraProjection_Perspective: {
            Result = Mat4Perspective(FOV, 
                                     ViewportSize.x,
                                     ViewportSize.y,
                                     ClippingPlaneNear, 
                                     ClippingPlaneFar);
        } break;
        case CameraProjection_Orthographic: {
            f32 HalfWidth  = (ViewportSize.x)/2.0f;
            f32 HalfHeight = (ViewportSize.y)/2.0f;
            Result = Mat4Orthographic(-HalfWidth, -HalfHeight, 
                                       HalfWidth,  HalfHeight, 
                                      ClippingPlaneNear, 
                                      ClippingPlaneFar);
        } break;
        default: {
            Assert(!"Invalid code path");
            Result = Mat4Identity();
        } break;
    }

    return Result;
}
