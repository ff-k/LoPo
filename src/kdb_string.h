#ifndef KDB_HEADER_STRING
#define KDB_HEADER_STRING

#include "kdb_common.h"
#include "kdb_math.h"

namespace kadabra {
    
    b32 IsWhitespace(char C);
    b32 IsNumeric(char C);
        
    size_t StringLength(char *Str);
    s32    StringCompare(char *A, size_t LengthA, char *B, size_t LengthB);
    s32    StringCompareN(char *A, size_t LengthA, char *B, size_t LengthB);
        
    u32 StringToU32(char *Str, char **CursorEnd=0);
    s32 StringToS32(char *Str, char **CursorEnd=0);
    f32 StringToF32(char *Str, char **CursorEnd=0);
        
    vec2  StringToVec2(char *Str, char **CursorEnd=0);
    vec2u StringToVec2u(char *Str, char **CursorEnd=0);
    vec2i StringToVec2i(char *Str, char **CursorEnd=0);
    
    vec3  StringToVec3(char *Str, char **CursorEnd=0);
    vec3u StringToVec3u(char *Str, char **CursorEnd=0);
    vec3i StringToVec3i(char *Str, char **CursorEnd=0);
    
    vec4  StringToVec4(char *Str, char **CursorEnd=0);
    vec4u StringToVec4u(char *Str, char **CursorEnd=0);
    vec4i StringToVec4i(char *Str, char **CursorEnd=0);
    
}

#endif