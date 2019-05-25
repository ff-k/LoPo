#include "kdb_util.h"
#include "kdb_math.h"

#include <stdlib.h>

u32
kadabra::RandomU32(u32 Max){
    // NOTE(furkan): Max is excluded
    u32 Result;

    if(Max > RAND_MAX){
        Warning("Maximum return value of RandomU32: %u (Requested max: %u)", 
                                                             RAND_MAX, Max);
    }

    Result = rand() % Max;

    return Result;
}

f32
kadabra::Random01(){
    f32 Result;

    Result = ((f32)rand())/RAND_MAX;

    return Result;
}

f32 
kadabra::Clamp(f32 Val, f32 Min, f32 Max){
    f32 Result;

    Result = MaxOf(Min, MinOf(Val, Max));

    return Result;
}

f32 
kadabra::Clamp01(f32 Val){
    f32 Result;

    Result = Clamp(Val, 0.0f, 1.0f);

    return Result;
}

f32
kadabra::Ceil(f32 Val){
    f32 Result;

    Result = ceilf(Val);

    return Result;
}

s32
kadabra::CeilAsS32(f32 Val){
    s32 Result;

    Result = (s32)ceilf(Val);

    return Result;
}

u32
kadabra::CeilAsU32(f32 Val){
    u32 Result;

    Result = (u32)ceilf(Val);

    return Result;
}

f32
kadabra::Floor(f32 Val){
    f32 Result;

    Result = floorf(Val);

    return Result;
}

s32
kadabra::FloorAsS32(f32 Val){
    s32 Result;

    Result = (s32)floorf(Val);

    return Result;
}

u32
kadabra::FloorAsU32(f32 Val){
    u32 Result;

    Result = (u32)floorf(Val);

    return Result;
}

f32
kadabra::Round(f32 Val){
    f32 Result;

    Result = floorf(Val+0.5f);

    return Result;
}

s32 
kadabra::RoundAsS32(f32 Val){
    s32 Result;

    Result = (s32)(Val+0.5f);

    return Result;
}

u32 
kadabra::RoundAsU32(f32 Val){
    u32 Result;

    Result = (u32)(Val+0.5f);

    return Result;
}

s32
kadabra::Square(s32 Val){
    s32 Result;
    
    Result = Val*Val;

    return Result;
}

u32
kadabra::Square(u32 Val){
    u32 Result;
    
    Result = Val*Val;

    return Result;
}

f32
kadabra::Square(f32 Val){
    f32 Result;
    
    Result = Val*Val;

    return Result;
}

f32
kadabra::Power(f32 Base, f32 Exp){
    f32 Result;

    Result = powf(Base, Exp);
    
    return Result;
}

f32 
kadabra::Exp(f32 Val){
    f32 Result;

    Result = expf(Val);

    return Result;
}

f32
kadabra::Sqrt(f32 Val){
    f32 Result;

    Result = sqrtf(Val);

    return Result;
}

f32 
kadabra::Abs(f32 Val){
    f32 Result;

    Result = (f32)fabs(Val);

    return Result;
}

f32
kadabra::DegreeToRadian(f32 Degree){
    f32 Result;

    Result = (Degree/180.0f)*kadabra::Pi_f32;

    return Result;
}

f32 
kadabra::RadianToDegree(f32 Radian){
    f32 Result;

    Result = (Radian*180.0f)/kadabra::Pi_f32;

    return Result;
}

f32
kadabra::Sin(f32 Radians){
    f32 Result;

    Result = sinf(Radians);

    return Result;
}

f32
kadabra::Cos(f32 Radians){
    f32 Result;

    Result = cosf(Radians);

    return Result;
}

f32
kadabra::Tan(f32 Radians){
    f32 Result;

    Result = tanf(Radians);

    return Result;
}

f32 
kadabra::Asin(f32 x){
    f32 Result;

    Result = asinf(x);

    return Result;
}

f32 
kadabra::Acos(f32 x){
    f32 Result;

    Result = acosf(x);

    return Result;
}

f32
kadabra::Atan(f32 y, f32 x){
    f32 Result;

    Result = atan2f(y, x);

    return Result;
}

f32 
kadabra::LogE(f32 x){
    f32 Result;

    Result = logf(x);

    return Result;
}

f32 
kadabra::Log10(f32 x){
    f32 Result;

    Result = log10f(x);

    return Result;
}

f32 
kadabra::Mod(f32 Numer, f32 Denom){
    f32 Result;

    Result = fmodf(Numer, Denom);

    return Result;
}

f32 
kadabra::Frac(f32 x, f32 *Int){
    f32 Result;

    Result = modff(x, Int);

    return Result;
}

#include "kdb_math_vec2.cpp"
#include "kdb_math_vec2i.cpp"
#include "kdb_math_vec2u.cpp"

#include "kdb_math_vec3.cpp"
#include "kdb_math_vec3i.cpp"
#include "kdb_math_vec3u.cpp"

#include "kdb_math_vec4.cpp"
#include "kdb_math_vec4i.cpp"
#include "kdb_math_vec4u.cpp"

#include "kdb_math_mat4.cpp"

b32 
kadabra::AABBsOverlap(const kadabra::aabb &A, const kadabra::aabb &B){
    b32 Result = false;

    Result = A.Max.x > B.Min.x &&
             A.Min.x < B.Max.x &&
             A.Max.y > B.Min.y &&
             A.Min.y < B.Max.y &&
             A.Max.z > B.Min.z &&
             A.Min.z < B.Max.z;

    return Result;
}

kadabra::mat4 
kadabra::BuildTransformationMatrix(kadabra::vec3 Translation, 
                                   kadabra::vec3 EulerAngles, 
                                   kadabra::vec3 Scale){
    mat4 TM = Mat4Identity();
    TM = Mat4Translate(TM, Translation);
    TM = Mat4Rotate(TM, EulerAngles.z, RotationAxis_Z);
    TM = Mat4Rotate(TM, EulerAngles.y, RotationAxis_Y);
    TM = Mat4Rotate(TM, EulerAngles.x, RotationAxis_X);
    TM = Mat4Scale(TM, Scale);
    
    return TM;
}

void
kadabra::AABBTransformInPlace(kadabra::aabb *AABB, 
                              kadabra::mat4 *XForm){
    kadabra::vec3 Ps[] = {
        kadabra::Vec3(AABB->Min.x, AABB->Min.y, AABB->Min.z),
        kadabra::Vec3(AABB->Min.x, AABB->Min.y, AABB->Max.z),
        kadabra::Vec3(AABB->Min.x, AABB->Max.y, AABB->Min.z),
        kadabra::Vec3(AABB->Min.x, AABB->Max.y, AABB->Max.z),
        kadabra::Vec3(AABB->Max.x, AABB->Min.y, AABB->Min.z),
        kadabra::Vec3(AABB->Max.x, AABB->Min.y, AABB->Max.z),
        kadabra::Vec3(AABB->Max.x, AABB->Max.y, AABB->Min.z),
        kadabra::Vec3(AABB->Max.x, AABB->Max.y, AABB->Max.z)
    };
    
    vec3 Min = Vec3(f32_Max, f32_Max, f32_Max);
    vec3 Max = Vec3(f32_Min, f32_Min, f32_Min);
    for(u32 Idx=0; Idx<8; Idx++){
        vec3 TP = ((*XForm)*Vec4(Ps[Idx], 1.0f)).xyz;
        
        if(Min.x > TP.x){ Min.x = TP.x; }
        if(Min.y > TP.y){ Min.y = TP.y; }
        if(Min.z > TP.z){ Min.z = TP.z; }
        
        if(Max.x < TP.x){ Max.x = TP.x; }
        if(Max.y < TP.y){ Max.y = TP.y; }
        if(Max.z < TP.z){ Max.z = TP.z; }
    }
    
    AABB->Min = Min;
    AABB->Max = Max;
}

void 
kadabra::TriangleTransformInPlace(kadabra::vec3 *V0, 
                                  kadabra::vec3 *V1, 
                                  kadabra::vec3 *V2, 
                                  kadabra::mat4 *XForm){
    
    vec3 T0 = ((*XForm)*Vec4(*V0, 1.0f)).xyz;
    vec3 T1 = ((*XForm)*Vec4(*V1, 1.0f)).xyz;
    vec3 T2 = ((*XForm)*Vec4(*V2, 1.0f)).xyz;
    
    *V0 = T0; *V1 = T1; *V2 = T2;
}

void
kadabra::TransformPointInPlace(vec3 *V, vec3 Translation, 
                               vec3 EulerAngles, vec3 Scale){
    mat4 TransMat = BuildTransformationMatrix(Translation, 
                                              EulerAngles, 
                                              Scale);
    
    vec3 TV = (TransMat*Vec4(*V, 1.0f)).xyz;
    
    *V = TV;
}

void
kadabra::TransformNormalInPlace(vec3 *N, vec3 EulerAngles, vec3 Scale){
    mat4 TransMat = Mat4Identity();
    TransMat = Mat4Rotate(TransMat, EulerAngles.z, RotationAxis_Z);
    TransMat = Mat4Rotate(TransMat, EulerAngles.y, RotationAxis_Y);
    TransMat = Mat4Rotate(TransMat, EulerAngles.x, RotationAxis_X);
    TransMat = Mat4Scale(TransMat, Scale);
    
    mat4 TransInv;
    if(Mat4Inverse(TransMat, &TransInv)){
        TransMat = Mat4Transpose(TransInv);
    } else {
        Warning("Mat4Inverse failed in TransformNormalInPlace");
    }
    
    vec3 TN = (TransMat*Vec4(*N, 1.0f)).xyz;
    
    *N = TN;
}

kadabra::vec2 
kadabra::EulerDegreesFromXAxis(vec3 Axis){
    vec2 Result;
    
    vec3 Axis_n = Normalize(Axis);
                
    f32 EulerX = Acos(Axis_n.x);
    f32 SinEulerX = Sin(EulerX);
    
    f32 EulerY = 0.0f;
    if(Abs(SinEulerX) > 0.001f){
        f32 AsinArg = Axis_n.z/SinEulerX;
        if(AsinArg > 1.0f){
            AsinArg = 1.0f;
        } else if(AsinArg < -1.0f){
            AsinArg = -1.0f;
        }
        
        EulerY = Asin(AsinArg);
    }
    
    EulerX = RadianToDegree(EulerX);
    EulerY = RadianToDegree(EulerY);
    
    if(Axis_n.y < 0.0f){
        EulerY = 180.0f - EulerY;
    }
    
    Result = Vec2(EulerX, EulerY);
    
    return Result;
}

kadabra::vec2 
kadabra::EulerDegreesFromYAxis(vec3 Axis){
    vec2 Result;
    
    vec3 Axis_n = Normalize(Axis);
                
    f32 EulerX = Acos(Axis_n.y);
    f32 SinEulerX = Sin(EulerX);
    
    f32 EulerY = 0.0f;
    if(Abs(SinEulerX) > 0.001f){
        f32 AsinArg = Axis_n.x/SinEulerX;
        if(AsinArg > 1.0f){
            AsinArg = 1.0f;
        } else if(AsinArg < -1.0f){
            AsinArg = -1.0f;
        }
        
        EulerY = Asin(AsinArg);
    }
    
    EulerX = RadianToDegree(EulerX);
    EulerY = RadianToDegree(EulerY);
    
    if(Axis_n.z < 0.0f){
        EulerY = 180.0f - EulerY;
    }
    
    Result = Vec2(EulerX, EulerY);
    
    return Result;
}

kadabra::vec2 
kadabra::EulerDegreesFromZAxis(vec3 Axis){
    vec2 Result;
    
    vec3 Axis_n = Normalize(Axis);
                
    f32 EulerX = Acos(Axis_n.z);
    f32 SinEulerX = Sin(EulerX);
    
    f32 EulerY = 0.0f;
    if(Abs(SinEulerX) > 0.001f){
        f32 AsinArg = Axis_n.y/SinEulerX;
        if(AsinArg > 1.0f){
            AsinArg = 1.0f;
        } else if(AsinArg < -1.0f){
            AsinArg = -1.0f;
        }
        
        EulerY = Asin(AsinArg);
    }
    
    EulerX = RadianToDegree(EulerX);
    EulerY = RadianToDegree(EulerY);
    
    if(Axis_n.x < 0.0f){
        EulerY = 180.0f - EulerY;
    }
    
    Result = Vec2(EulerX, EulerY);
    
    return Result;
}