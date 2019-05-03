#ifndef KDB_HEADER_COMMON
#define KDB_HEADER_COMMON

#include "kdb_config.h"

#if KDBDevelopmentBuild

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#endif

#include <inttypes.h>
#include <float.h>

typedef int64_t  s64;
typedef int32_t  s32;
typedef int16_t  s16;
typedef int8_t   s8;

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef double   f64;
typedef float    f32;

typedef u64      b64;
typedef u32      b32;
typedef u16      b16;
typedef u8       b8;

#define DefinePairStruct(Type)\
    typedef struct {          \
        Type Val[2];          \
    } Type##_pair;
    
#define DefineTripletStruct(Type)\
    typedef struct {             \
        Type Val[3];             \
    } Type##_triplet;

DefinePairStruct(s64);
DefinePairStruct(s32);
DefinePairStruct(s16);
DefinePairStruct(s8 );
DefinePairStruct(u64);
DefinePairStruct(u32);
DefinePairStruct(u16);
DefinePairStruct(u8 );
DefinePairStruct(f64);
DefinePairStruct(f32);
DefinePairStruct(b64);
DefinePairStruct(b32);
DefinePairStruct(b16);
DefinePairStruct(b8 );

DefineTripletStruct(s64);
DefineTripletStruct(s32);
DefineTripletStruct(s16);
DefineTripletStruct(s8 );
DefineTripletStruct(u64);
DefineTripletStruct(u32);
DefineTripletStruct(u16);
DefineTripletStruct(u8 );
DefineTripletStruct(f64);
DefineTripletStruct(f32);
DefineTripletStruct(b64);
DefineTripletStruct(b32);
DefineTripletStruct(b16);
DefineTripletStruct(b8 );

#ifndef __cplusplus
    #ifndef _BOOL
        typedef b32 bool;
        #define true 1
        #define false 0
    #endif
#endif 

/* TODO(furkan) : Get data type limits from platform */
#define f32_Max  FLT_MAX
#define f32_Min -FLT_MAX
#define u32_Max  0xFFFFFFFF
#define u32_Min  0x0

#endif