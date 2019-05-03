#ifndef KDB_HEADER_IO
#define KDB_HEADER_IO

#include "kdb_common.h"

namespace kadabra {

    enum file_access_mode {
        FileAccessMode_Read  = 0x1,
        FileAccessMode_Write = 0x2,
    };
    
    enum file_share_mode {
        FileShareMode_Read   = 0x1,
        FileShareMode_Write  = 0x2,
        FileShareMode_Delete = 0x4,
    };
    
    enum file_creation_attrib {
        FileCreationAttrib_CreateAlways      = 0x01,
        FileCreationAttrib_CreateIfNotExist  = 0x02,
        FileCreationAttrib_OpenAlways        = 0x04,
        FileCreationAttrib_OpenIfExist       = 0x08,
        FileCreationAttrib_OpenAndTruncate   = 0x10,
    };
    
    enum file_seek_pivot {
        FileSeekPivot_Begin,
        FileSeekPivot_Current,
        FileSeekPivot_End
    };
    
    struct read_entire_file_result_32 {
        void *Contents;
        u32 Size;
        b32 IsEmpty;
    };
    
    struct read_file_chained_result_32 {
        s64 TotalSize;
        s64 TotalReadBytes;
        
        void *Buffer;
        u32 BufferCapacity;
        u32 BufferFilledSize;
        
        b32 IsEmpty;
        b32 ReachedEndOfFile;
    };

    b32 ReadEntireFile(char *Path, void **Memory, u64 *FileSize);
    u32 FileExtensionIndex(char *Path);
    
}

#endif