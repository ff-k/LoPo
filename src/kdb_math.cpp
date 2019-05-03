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
kadabra::AABBsOverlap(kadabra::aabb A, kadabra::aabb B){
    b32 Result = false;

    kadabra::vec3 Ps[] = {
        kadabra::Vec3(B.Min.x, B.Min.y, B.Min.z),
        kadabra::Vec3(B.Min.x, B.Min.y, B.Max.z),
        kadabra::Vec3(B.Min.x, B.Max.y, B.Min.z),
        kadabra::Vec3(B.Min.x, B.Max.y, B.Max.z),
        kadabra::Vec3(B.Max.x, B.Min.y, B.Min.z),
        kadabra::Vec3(B.Max.x, B.Min.y, B.Max.z),
        kadabra::Vec3(B.Max.x, B.Max.y, B.Min.z),
        kadabra::Vec3(B.Max.x, B.Max.y, B.Max.z)
    };

    for(u32 PIdx=0; PIdx<StaticArraySize(Ps); PIdx++){
        kadabra::vec3 P = Ps[PIdx];
        b32 Overlap = (P.x >= A.Min.x && P.x <= A.Max.x) &&
                      (P.y >= A.Min.y && P.y <= A.Max.y) &&
                      (P.z >= A.Min.z && P.z <= A.Max.z);
        if(Overlap){
            Result = true;
            break;
        }
    }

    return Result;
}
