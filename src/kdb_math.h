#ifndef KDB_HEADER_MATH
#define KDB_HEADER_MATH

#include <math.h>

#include "kdb_common.h"

namespace kadabra {

    const static f64 Pi = 3.14159265359;
    const static f32 Pi_f32 = ((f32)Pi);
    
    #define MinOf(A, B) (((A) < (B)) ? (A) : (B))
    #define MaxOf(A, B) (((A) > (B)) ? (A) : (B))
    
    u32 RandomU32(u32 Max); // NOTE(furkan): Max is excluded
    f32 Random01();
    
    f32 Clamp(f32 Val, f32 Min, f32 Max);
    f32 Clamp01(f32 Val);
    
    f32 Ceil(f32 Val);
    s32 CeilAsS32(f32 Val);
    u32 CeilAsU32(f32 Val);
    f32 Floor(f32 Val);
    s32 FloorAsS32(f32 Val);
    u32 FloorAsU32(f32 Val);
    f32 Round(f32 Val);
    s32 RoundAsS32(f32 Val);
    u32 RoundAsU32(f32 Val);
    
    s32 Square(s32 Val);
    u32 Square(u32 Val);
    f32 Square(f32 Val);
    f32 Power(f32 Base, f32 Exp);
    f32 Exp(f32 Val);
    f32 Sqrt(f32 Val);
    f32 Abs(f32 Val);
    
    f32 DegreeToRadian(f32 Degree);
    f32 RadianToDegree(f32 Radian);
    
    f32 Sin(f32 Radians);
    f32 Cos(f32 Radians);
    f32 Tan(f32 Radians);
    
    f32 Asin(f32 x);
    f32 Acos(f32 x);
    f32 Atan(f32 y, f32 x);
    
    f32 LogE(f32 x);
    f32 Log10(f32 x);
    
    f32 Mod(f32 Numer, f32 Denom);
    
    f32 Frac(f32 x, f32 *Int);
    
    enum rotation_axis {
        RotationAxis_X,
        RotationAxis_Y,
        RotationAxis_Z
    };
    
    struct vec2 {
        union{
            struct{
                f32 x;
                f32 y;
            };
            struct{
                f32 u;
                f32 v;
            };
        };
    };
    
    vec2 Vec2(f32 x, f32 y);
    
    vec2 operator+(vec2  A, vec2 B);
    vec2 operator+(vec2  A, f32  B);
    vec2 operator+(f32   A, vec2 B);
    vec2 operator+=(vec2& A, vec2 B);
    vec2 operator+=(vec2& A, f32  B);
    vec2 operator-(vec2  A, vec2 B);
    vec2 operator-(vec2  A, f32  B);
    vec2 operator-(f32   A, vec2 B);
    vec2 operator-=(vec2& A, vec2 B);
    vec2 operator-=(vec2& A, f32  B);
    vec2 operator*(f32   A, vec2 B);
    vec2 operator*(vec2  A, f32  B);
    vec2 operator*=(vec2& A, f32  B);
    vec2 operator-(vec2 &v1);
    
    vec2 Hadamard(vec2  A, vec2  B);
    f32  Length(vec2 V);
    vec2 Normalize(vec2 V);
    f32  Dot(vec2 A, vec2 B);
    vec2 Reflect(vec2 V, vec2 R);
    
    struct vec2u {
        union{
            struct{
                u32 x;
                u32 y;
            };
            struct{
                u32 u;
                u32 v;
            };
        };
    };
    
    vec2u Vec2u(u32 x, u32 y);
    
    vec2u operator+(vec2u  A, vec2u B);
    vec2u operator+(vec2u  A, u32   B);
    vec2u operator+(u32    A, vec2u B);
    vec2u operator+=(vec2u& A, vec2u B);
    vec2u operator+=(vec2u& A, u32   B);
    vec2u operator-(vec2u  A, vec2u B);
    vec2u operator-(vec2u  A, u32   B);
    vec2u operator-(u32    A, vec2u B);
    vec2u operator-=(vec2u& A, vec2u B);
    vec2u operator-=(vec2u& A, u32   B);
    vec2u operator*(u32    A, vec2u B);
    vec2u operator*(vec2u  A, u32   B);
    vec2u operator*=(vec2u& A, u32   B);
    
    vec2u Hadamard(vec2u A, vec2u B);
    f32   Length(vec2u V);
    vec2  Normalize(vec2u V);
    u32   Dot(vec2u A, vec2u B);
    
    struct vec2i {
        union{
            struct{
                s32 x;
                s32 y;
            };
            struct{
                s32 u;
                s32 v;
            };
        };
    };
    
    vec2i Vec2i(s32 x, s32 y);
    
    vec2i operator+(vec2i  A, vec2i B);
    vec2i operator+(vec2i  A, s32   B);
    vec2i operator+(s32    A, vec2i B);
    vec2i operator+=(vec2i& A, vec2i B);
    vec2i operator+=(vec2i& A, s32   B);
    vec2i operator-(vec2i  A, vec2i B);
    vec2i operator-(vec2i  A, s32   B);
    vec2i operator-(s32    A, vec2i B);
    vec2i operator-=(vec2i& A, vec2i B);
    vec2i operator-=(vec2i& A, s32   B);
    vec2i operator*(s32    A, vec2i B);
    vec2i operator*(vec2i  A, s32   B);
    vec2i operator*=(vec2i& A, s32   B);
    vec2i operator-(vec2i &v1);
    
    vec2i Hadamard(vec2i A, vec2i B);
    f32   Length(vec2i V);
    vec2  Normalize(vec2i V);
    s32   Dot(vec2i A, vec2i B);
    
    struct vec3 {
        union{
            struct{
                f32 x;
                f32 y;
                f32 z;
            };
            struct{
                vec2 xy;
                f32  z;
            };
            struct{
                f32  x;
                vec2 yz;
            };
            struct{
                f32 r;
                f32 g;
                f32 b;
            };
        };
    };
    
    vec3 Vec3(f32 x, f32 y, f32 z);
    vec3 Vec3(vec2 xy, f32 z);
    vec3 Vec3(f32 x, vec2 yz);
    
    vec3 operator+(vec3  A, vec3 B);
    vec3 operator+(vec3  A, f32  B);
    vec3 operator+(f32   A, vec3 B);
    vec3 operator+=(vec3& A, vec3 B);
    vec3 operator+=(vec3& A, f32  B);
    vec3 operator-(vec3  A, vec3 B);
    vec3 operator-(vec3  A, f32  B);
    vec3 operator-(f32   A, vec3 B);
    vec3 operator-=(vec3& A, vec3 B);
    vec3 operator-=(vec3& A, f32  B);
    vec3 operator*(f32   A, vec3 B);
    vec3 operator*(vec3  A, f32  B);
    vec3 operator*=(vec3& A, f32  B);
    vec3 operator-(vec3 &v1);
    
    vec3 Hadamard(vec3  A, vec3  B);
    f32  Length(vec3 V);
    vec3 Normalize(vec3 V);
    f32  Dot(vec3 A, vec3 B);
    vec3 Reflect(vec3 V, vec3 R);
    vec3 Cross(vec3  A, vec3  B);
    vec3 RotateAround(vec3 V, f32 AngleDegrees, vec3 Axis);
    
    struct vec3u {
        union{
            struct{
                u32 x;
                u32 y;
                u32 z;
            };
            struct{
                vec2u xy;
                u32   z;
            };
            struct{
                u32   x;
                vec2u yz;
            };
            struct{
                u32 r;
                u32 g;
                u32 b;
            };
        };
    };
    
    vec3u Vec3u(u32 x, u32 y, u32 z);
    vec3u Vec3u(vec2u xy, u32 z);
    vec3u Vec3u(u32 x, vec2u yz);
    
    vec3u operator+(vec3u  A, vec3u B);
    vec3u operator+(vec3u  A, u32   B);
    vec3u operator+(u32    A, vec3u B);
    vec3u operator+=(vec3u& A, vec3u B);
    vec3u operator+=(vec3u& A, u32   B);
    vec3u operator-(vec3u  A, vec3u B);
    vec3u operator-(vec3u  A, u32   B);
    vec3u operator-(u32    A, vec3u B);
    vec3u operator-=(vec3u& A, vec3u B);
    vec3u operator-=(vec3u& A, u32   B);
    vec3u operator*(u32    A, vec3u B);
    vec3u operator*(vec3u  A, u32   B);
    vec3u operator*=(vec3u& A, u32   B);
    
    vec3u Hadamard(vec3u A, vec3u B);
    f32   Length(vec3u V);
    vec3  Normalize(vec3u V);
    u32   Dot(vec3u A, vec3u B);
    vec3u Cross(vec3u A, vec3u B);
    
    struct vec3i {
        union{
            struct{
                s32 x;
                s32 y;
                s32 z;
            };
            struct{
                vec2i xy;
                s32   z;
            };
            struct{
                s32   x;
                vec2i yz;
            };
            struct{
                s32 r;
                s32 g;
                s32 b;
            };
        };
    };
    
    vec3i Vec3i(s32 x, s32 y, s32 z);
    vec3i Vec3i(vec2i xy, s32 z);
    vec3i Vec3i(s32 x, vec2i yz);
    
    vec3i operator+(vec3i  A, vec3i B);
    vec3i operator+(vec3i  A, s32   B);
    vec3i operator+(s32    A, vec3i B);
    vec3i operator+=(vec3i& A, vec3i B);
    vec3i operator+=(vec3i& A, s32   B);
    vec3i operator-(vec3i  A, vec3i B);
    vec3i operator-(vec3i  A, s32   B);
    vec3i operator-(s32    A, vec3i B);
    vec3i operator-=(vec3i& A, vec3i B);
    vec3i operator-=(vec3i& A, s32   B);
    vec3i operator*(s32    A, vec3i B);
    vec3i operator*(vec3i  A, s32   B);
    vec3i operator*=(vec3i& A, s32   B);
    vec3i operator-(vec3i &v1);
    
    vec3i Hadamard(vec3i A, vec3i B);
    f32   Length(vec3i V);
    vec3  Normalize(vec3i V);
    s32   Dot(vec3i A, vec3i B);
    vec3i Cross(vec3i A, vec3i B);
    
    struct vec4 {
        union{
            struct{
                f32 x;
                f32 y;
                f32 z;
                f32 w;
            };
            struct{
                f32 r;
                f32 l;
                f32 b;
                f32 t;
            };
            struct{
                vec2 xy;
                f32 z;
                f32 w;
            };
            struct{
                vec3 xyz;
                f32 w;
            };
            struct {
                f32 x;
                f32 y;
                vec2 zw;
            };
            struct{
                f32 r;
                f32 g;
                f32 b;
                f32 a;
            };
            struct{
                vec3 rgb;
                f32 a;
            };
        };
    };
    
    vec4 Vec4(f32 x, f32 y, f32 z, f32 w);
    vec4 Vec4(vec2 xy, f32 z, f32 w);
    vec4 Vec4(f32 x, vec2 yz, f32 w);
    vec4 Vec4(f32 x, f32 y, vec2 zw);
    vec4 Vec4(vec2 xy, vec2 zw);
    vec4 Vec4(vec3 xyz, f32 w);
    vec4 Vec4(f32 x, vec3 yzw);
    
    vec4 operator+(vec4  A, vec4 B);
    vec4 operator+(vec4  A, f32  B);
    vec4 operator+(f32   A, vec4 B);
    vec4 operator+=(vec4& A, vec4 B);
    vec4 operator+=(vec4& A, f32  B);
    vec4 operator-(vec4  A, vec4 B);
    vec4 operator-(vec4  A, f32  B);
    vec4 operator-(f32   A, vec4 B);
    vec4 operator-=(vec4& A, vec4 B);
    vec4 operator-=(vec4& A, f32  B);
    vec4 operator*(f32   A, vec4 B);
    vec4 operator*(vec4  A, f32  B);
    vec4 operator*=(vec4& A, f32  B);
    vec4 operator-(vec4 &v1);
    
    vec4 Hadamard(vec4  A, vec4  B);
    f32  Length(vec4 V);
    vec4 Normalize(vec4 V);
    f32  Dot(vec4 A, vec4 B);
    
    struct vec4u {
        union{
            struct{
                u32 x;
                u32 y;
                u32 z;
                u32 w;
            };
            struct{
                u32 r;
                u32 l;
                u32 b;
                u32 t;
            };
            struct{
                vec2u xy;
                u32 z;
                u32 w;
            };
            struct{
                vec3u xyz;
                u32 w;
            };
            struct {
                u32 x;
                u32 y;
                vec2u zw;
            };
            struct{
                u32 r;
                u32 g;
                u32 b;
                u32 a;
            };
            struct{
                vec3u rgb;
                u32 a;
            };
        };
    };
    
    vec4u Vec4u(u32 x, u32 y, u32 z, u32 w);
    vec4u Vec4u(vec2u xy, u32 z, u32 w);
    vec4u Vec4u(u32 x, vec2u yz, u32 w);
    vec4u Vec4u(u32 x, u32 y, vec2u zw);
    vec4u Vec4u(vec2u xy, vec2u zw);
    vec4u Vec4u(vec3u xyz, u32 w);
    vec4u Vec4u(u32 x, vec3u yzw);
    
    vec4u operator+(vec4u  A, vec4u B);
    vec4u operator+(vec4u  A, u32   B);
    vec4u operator+(u32    A, vec4u B);
    vec4u operator+=(vec4u& A, vec4u B);
    vec4u operator+=(vec4u& A, u32   B);
    vec4u operator-(vec4u  A, vec4u B);
    vec4u operator-(vec4u  A, u32   B);
    vec4u operator-(u32    A, vec4u B);
    vec4u operator-=(vec4u& A, vec4u B);
    vec4u operator-=(vec4u& A, u32   B);
    vec4u operator*(u32    A, vec4u B);
    vec4u operator*(vec4u  A, u32   B);
    vec4u operator*=(vec4u& A, u32   B);
    
    vec4u Hadamard(vec4u A, vec4u B);
    f32   Length(vec4u V);
    vec4  Normalize(vec4u V);
    u32   Dot(vec4u A, vec4u B);
    
    struct vec4i {
        union{
            struct{
                s32 x;
                s32 y;
                s32 z;
                s32 w;
            };
            struct{
                s32 r;
                s32 l;
                s32 b;
                s32 t;
            };
            struct{
                vec2i xy;
                s32 z;
                s32 w;
            };
            struct{
                vec3i xyz;
                s32 w;
            };
            struct {
                s32 x;
                s32 y;
                vec2i zw;
            };
            struct{
                s32 r;
                s32 g;
                s32 b;
                s32 a;
            };
            struct{
                vec3i rgb;
                s32 a;
            };
        };
    };
    
    vec4i Vec4i(s32 x, s32 y, s32 z, s32 w);
    vec4i Vec4i(vec2i xy, s32 z, s32 w);
    vec4i Vec4i(s32 x, vec2i yz, s32 w);
    vec4i Vec4i(s32 x, s32 y, vec2i zw);
    vec4i Vec4i(vec2i xy, vec2i zw);
    vec4i Vec4i(vec3i xyz, s32 w);
    vec4i Vec4i(s32 x, vec3i yzw);
    
    vec4i operator+(vec4i  A, vec4i B);
    vec4i operator+(vec4i  A, s32   B);
    vec4i operator+(s32    A, vec4i B);
    vec4i operator+=(vec4i& A, vec4i B);
    vec4i operator+=(vec4i& A, s32   B);
    vec4i operator-(vec4i  A, vec4i B);
    vec4i operator-(vec4i  A, s32   B);
    vec4i operator-(s32    A, vec4i B);
    vec4i operator-=(vec4i& A, vec4i B);
    vec4i operator-=(vec4i& A, s32   B);
    vec4i operator*(s32    A, vec4i B);
    vec4i operator*(vec4i  A, s32   B);
    vec4i operator*=(vec4i& A, s32   B);
    vec4i operator-(vec4i &v1);
    
    vec4i Hadamard(vec4i A, vec4i B);
    f32   Length(vec4i V);
    vec4  Normalize(vec4i V);
    s32   Dot(vec4i A, vec4i B);
    
    struct basis2 {
        vec2 x;
        vec2 y;
    };
    
    struct basis3 {
        vec3 x;
        vec3 y;
        vec3 z;
    };
    
    struct mat4 {
        union{
            // NOTE(furkan) : 
            //      | E[0][0] E[0][1] E[0][2] E[0][3] |
            //      | E[1][0] E[1][1] E[1][2] E[1][3] |
            //      | E[2][0] E[2][1] E[2][2] E[2][3] |
            //      | E[3][0] E[3][1] E[3][2] E[3][3] |
    
            f32 E[4][4];
            f32 Ptr[16];
        };
    };
    
    mat4 operator*(mat4 A, mat4 B);
    vec4 operator*(mat4 A, vec4 B);
    mat4 operator-(mat4 &M);
    
    mat4 Mat4Identity();
    
    b32  Mat4Inverse(mat4 M, mat4 *Inverse);
    mat4 Mat4Transpose(mat4 Mat);
    
    mat4 Mat4Translate(mat4 Mat, vec3 Translation);
    mat4 Mat4Rotate(mat4 Mat, f32 Radians, rotation_axis RotateAxis);
    mat4 Mat4Scale(mat4 Mat, vec3 Scale);
    
    mat4 Mat4Orthographic(f32 Left, f32 Bottom, 
                                f32 Right, f32 Top, 
                                f32 NearZ, f32 FarZ);
                                
    mat4 Mat4Perspective(f32 FOVAngle, 
                                f32 Width, f32 Height, 
                                f32 NearZ, f32 FarZ);
    
    struct aabb {
        vec3 Min;
        vec3 Max;
    };
    
    b32 AABBsOverlap(const aabb& A, const aabb& B);
    
    mat4 BuildTransformationMatrix(vec3 Translation, 
                                   vec3 EulerAngles, 
                                   vec3 Scale);
    
    void AABBTransformInPlace(aabb *AABB, mat4 *XForm);
    
    void TriangleTransformInPlace(vec3 *V0, vec3 *V1, vec3 *V2, 
                                  mat4 *XForm);
                                  
    void TransformPointInPlace(vec3 *P, vec3 Translation, 
                               vec3 EulerAngles, vec3 Scale);
    void TransformNormalInPlace(vec3 *N, vec3 EulerAngles, 
                                vec3 Scale);
    
    vec2 EulerDegreesFromYAxis(vec3 Axis);
    vec2 EulerDegreesFromZAxis(vec3 Axis);
}

#endif