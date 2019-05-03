#ifndef KDB_HEADER_UTIL
#define KDB_HEADER_UTIL

#include "kdb_config.h"

#if KDBDevelopmentBuild

    #include <stdio.h>
    
    #if defined(_MSC_VER) && UseVisualStudioConsole
        #define OEMRESOURCE
        #define WIN32_LEAN_AND_MEAN
        #include <windows.h>
        #undef CreateWindow
        #include <stdarg.h>
        
        #define DebugMessage(Label, ...) \
            VisualStudioDebugMessage_("%s ", Label); \
            VisualStudioDebugMessage_(__VA_ARGS__); \
            VisualStudioDebugMessage_(" (%s:%d, %s)\n", __FUNCTION__, __LINE__, __FILE__);
        #if KDBVerboseLog
            #define Log(...) DebugMessage("[ DEBUG ]", __VA_ARGS__);
        #else
            #define Log(...)
        #endif
        #define Warning(...) DebugMessage("[WARNING]", __VA_ARGS__);
        #define Error(...) DebugMessage("[ ERROR ]", __VA_ARGS__);
        
        static void VisualStudioDebugMessage_(char * Format, ...){
            char MessageBuffer[1024];
            int MessageLength;
            
            va_list VA_Args;
            va_start(VA_Args, Format);
            
            MessageLength = _vsnprintf_s(MessageBuffer, 1024, 1024, Format, 
                                                                      VA_Args);
            
            if (MessageLength > -1){
                OutputDebugStringA(MessageBuffer);
            } else {
                OutputDebugStringA("[ ERROR ] : MessageBuffer is not large enough to print a message! Check VisualStudioDebugMessage_ function.");
            }
            
            va_end(VA_Args);
        }
    #else
        #define DebugMessage(Label, ...) \
            printf("%s ", Label); \
            printf(__VA_ARGS__); \
            printf(" (%s:%d, %s)\n", __FUNCTION__, __LINE__, __FILE__);
        #if KDBVerboseLog
            #define Log(...) DebugMessage("[ DEBUG ]", __VA_ARGS__);
        #else
            #define Log(...)
        #endif
        #define Warning(...) DebugMessage("[WARNING]", __VA_ARGS__);
        #define Error(...) DebugMessage("[ ERROR ]", __VA_ARGS__);
    #endif

#define UnexpectedDefaultCase \
    default:{ \
        Error("Encountered an unexpected default case"); \
    } break;
    
#define Assert(Expression) if(!(Expression)) { Error("Assertion failed!"); *((int *)0) = 0; }
    
#else
    #define Log(...)
    #define Warning(...)
    #define Error(...)
    #define UnexpectedDefaultCase
    #define Assert(Expression)
#endif

#if !KDBVerboseLog
    #pragma warning(disable : 4100)
    #pragma warning(disable : 4189)
#endif

#include <memory.h>
#include <stddef.h>

#define ZeroStruct(Str) memset((Str), 0, sizeof(*Str))
#define StaticArraySize(Array) (sizeof((Array)) / sizeof((Array)[0]))
#define OffsetOf(Type, Member) offsetof(Type, Member)

inline void
SwapMemoryContents(void *A, void *B, size_t Size){
    if(A != B){
        unsigned char *A_u8 = (unsigned char *)A;
        unsigned char *B_u8 = (unsigned char *)B;
        for(size_t Byte=0; Byte<Size; Byte++){
            A_u8[Byte] = A_u8[Byte] ^ B_u8[Byte];
            B_u8[Byte] = B_u8[Byte] ^ A_u8[Byte];
            A_u8[Byte] = A_u8[Byte] ^ B_u8[Byte];
        }
    }
}

#endif