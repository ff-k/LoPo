#include "kdb_util.h"
#include "kdb_string.h"

using namespace kadabra;

b32
kadabra::IsWhitespace(char C){
    b32 Result = false;

    static char WhitespaceChars[] = {
        '\t',
        '\n',
        '\r',
        ' '
    };

    for(u32 W=0;
            W < StaticArraySize(WhitespaceChars);
            W++){
        if(WhitespaceChars[W] == C){
            Result = true;
            break;
        }
    }

    return Result;
}

b32
kadabra::IsNumeric(char C){
    b32 Result = false;

    if(C >= '0' && C <= '9'){
        Result = true;
    }

    return Result;
}

size_t
kadabra::StringLength(char *Str){
    // NOTE(furkan): This function assumes Str to be null-terminated

    size_t Result = 0;

    while(Str[Result] != '\0'){
        Result++;
    }

    return Result;
}

s32
kadabra::StringCompare(char *A, size_t LengthA, char *B, size_t LengthB){
    s32 Result = 0;

    if(LengthA == LengthB){
        for(size_t i=0;
                   i < LengthA;
                   i++){
            if(A[i] < B[i]){
                Result = -1;
                break;
            } else if (A[i] > B[i]){
                Result = 1;
                break;
            }
        }
    } else {
        Result = (LengthA < LengthB) ? -1 : 1;
    }

    return Result;
}

s32
kadabra::StringCompareN(char *A, size_t LengthA, char *B, size_t LengthB){
    s32 Result = 0;

    size_t LengthCmp = MinOf(LengthA, LengthB);

    for(size_t i=0;
               i < LengthCmp;
               i++){
        if(A[i] < B[i]){
            Result = -1;
            break;
        } else if (A[i] > B[i]){
            Result = 1;
            break;
        }
    }

    return Result;
}

u32
kadabra::StringToU32(char *Str, char **CursorEnd){
    u32 Result = 0;

    while(IsWhitespace(Str[0])){
        Str++;
    }

    while(true){
#if 0
        if(Str[0] == '\0' || IsWhitespace(Str[0])){
            break;
        } else {
            Assert(Str[0] >= '0' && Str[0] <= '9');

            Result *= 10;
            Result += Str[0] - '0';
            Str++;
        }
#else
        if(Str[0] >= '0' && Str[0] <= '9'){
            Result *= 10;
            Result += Str[0] - '0';
            Str++;
        } else {
            break;
        }
#endif
    }

    if(CursorEnd){
        *CursorEnd = Str;
    }

    return Result;
}

s32
kadabra::StringToS32(char *Str, char **CursorEnd){
    s32 Result = 0;

    while(IsWhitespace(Str[0])){
        Str++;
    }

    b32 IsNegative = false;
    if(Str[0] == '-'){
        IsNegative = true;
        Str++;
    }

    while(true){
        if(Str[0] == '\0' || IsWhitespace(Str[0])){
            break;
        } else {
            Assert(Str[0] >= '0' && Str[0] <= '9');

            Result *= 10;
            Result += Str[0] - '0';
            Str++;
        }
    }

    if(IsNegative){
        Result = -Result;
    }

    if(CursorEnd){
        *CursorEnd = Str;
    }

    return Result;
}

f32
kadabra::StringToF32(char *Str, char **CursorEnd){
    f32 Result = 0.0f;

    while(IsWhitespace(Str[0])){
        Str++;
    }

    b32 IsNegative = false;
    if(Str[0] == '-'){
        IsNegative = true;
        Str++;
    }

    s32 Wholes = 0;
    while(true){
        if(Str[0] =='\0' || IsWhitespace(Str[0]) || Str[0] == '.' || 
           Str[0] == 'e' || Str[0] == 'E'){
            break;
        } else {
            Assert(Str[0] >= '0' && Str[0] <= '9');

            Wholes *= 10;
            Wholes += Str[0] - '0';
            Str++;
        }
    }

    f32 FracFactor = 1.0f;
    f32 Frac = 0.0f;
    if(Str[0] == '.'){
        Str++;
    
        while(true){
            if(Str[0] == '\0' || IsWhitespace(Str[0]) ||
               Str[0] == 'e' || Str[0] == 'E'){
                break;
            } else {
                Assert(Str[0] >= '0' && Str[0] <= '9');

                FracFactor *= 10;
                Frac *= 10;
                Frac += Str[0] - '0';
                Str++;
            }
        }
    }

    Result = ((f32)Wholes) + (Frac/FracFactor);

    if(Str[0] == 'e' || Str[0] == 'E'){
        Str++;
    
        b32 NegativeExponent = false;
        if(Str[0] == '-'){
            NegativeExponent = true;
            Str++;
        } else if(Str[0] == '+'){
             Str++;
        }
        
        f32 Exponent = 0.0f;
        while(true){
            if(Str[0] == '\0' || IsWhitespace(Str[0])){
                break;
            } else {
                Assert(Str[0] >= '0' && Str[0] <= '9');

                Exponent *= 10;
                Exponent += Str[0] - '0';
                Str++;
            }
        }

        if(NegativeExponent){
            Exponent *= -1.0f;
        }

        f32 Factor = Power(10.0f, Exponent);
        Result *= Factor;
    }

    if(IsNegative){
        Result *= -1.0f;
    }

    if(CursorEnd){
        *CursorEnd = Str;
    }

    return Result;
}

vec2
kadabra::StringToVec2(char *Str, char **CursorEnd){
    vec2 Result = Vec2(0.0f, 0.0f);

    Result.x = StringToF32(Str, &Str); 
    Result.y = StringToF32(Str, &Str); 

    if(CursorEnd){
        *CursorEnd = Str;
    }

    return Result;
}

vec2u
kadabra::StringToVec2u(char *Str, char **CursorEnd){
    vec2u Result = Vec2u(0, 0);

    Result.x = StringToU32(Str, &Str); 
    Result.y = StringToU32(Str, &Str); 

    if(CursorEnd){
        *CursorEnd = Str;
    }

    return Result;
}

vec2i
kadabra::StringToVec2i(char *Str, char **CursorEnd){
    vec2i Result = Vec2i(0, 0);

    Result.x = StringToS32(Str, &Str); 
    Result.y = StringToS32(Str, &Str); 

    if(CursorEnd){
        *CursorEnd = Str;
    }

    return Result;
}

vec3
kadabra::StringToVec3(char *Str, char **CursorEnd){
    vec3 Result = Vec3(0.0f, 0.0f, 0.0f);

    Result.x = StringToF32(Str, &Str); 
    Result.y = StringToF32(Str, &Str); 
    Result.z = StringToF32(Str, &Str); 

    if(CursorEnd){
        *CursorEnd = Str;
    }

    return Result;
}

vec3u
kadabra::StringToVec3u(char *Str, char **CursorEnd){
    vec3u Result = Vec3u(0, 0, 0);

    Result.x = StringToU32(Str, &Str); 
    Result.y = StringToU32(Str, &Str); 
    Result.z = StringToU32(Str, &Str); 

    if(CursorEnd){
        *CursorEnd = Str;
    }

    return Result;
}

vec3i
kadabra::StringToVec3i(char *Str, char **CursorEnd){
    vec3i Result = Vec3i(0, 0, 0);

    Result.x = StringToS32(Str, &Str); 
    Result.y = StringToS32(Str, &Str); 
    Result.z = StringToS32(Str, &Str); 

    if(CursorEnd){
        *CursorEnd = Str;
    }

    return Result;
}

vec4
kadabra::StringToVec4(char *Str, char **CursorEnd){
    vec4 Result = Vec4(0.0f, 0.0f, 0.0f, 0.0f);

    Result.x = StringToF32(Str, &Str); 
    Result.y = StringToF32(Str, &Str); 
    Result.z = StringToF32(Str, &Str); 
    Result.w = StringToF32(Str, &Str); 

    if(CursorEnd){
        *CursorEnd = Str;
    }

    return Result;
}

vec4u
kadabra::StringToVec4u(char *Str, char **CursorEnd){
    vec4u Result = Vec4u(0, 0, 0, 0);

    Result.x = StringToU32(Str, &Str); 
    Result.y = StringToU32(Str, &Str); 
    Result.z = StringToU32(Str, &Str); 
    Result.w = StringToU32(Str, &Str); 

    if(CursorEnd){
        *CursorEnd = Str;
    }

    return Result;
}

vec4i
kadabra::StringToVec4i(char *Str, char **CursorEnd){
    vec4i Result = Vec4i(0, 0, 0, 0);

    Result.x = StringToS32(Str, &Str); 
    Result.y = StringToS32(Str, &Str); 
    Result.z = StringToS32(Str, &Str); 
    Result.w = StringToS32(Str, &Str); 

    if(CursorEnd){
        *CursorEnd = Str;
    }

    return Result;
}
