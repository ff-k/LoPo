#include "kdb_io.h"
#include "kdb_util.h"
#include "kdb_platform.h"

b32
kadabra::ReadEntireFile(char *Path, void **Memory, u64 *FileSize){
    // TODO(furkan): Currently, it does not support files larger than
    // 4GBs.

    b32 Success = false;

    // NOTE(furkan): Memory must not be allocated beforehand
    Assert(*Memory == 0);

    file_handle FileHandle;
    if(platform::OpenFile(&FileHandle, Path, 
                        FileAccessMode_Read,
                        FileShareMode_Read,
                        FileCreationAttrib_OpenIfExist)){
        if(platform::GetFileSize(FileHandle, FileSize)){
            if(platform::MemoryAllocate(Memory, *FileSize)){
                Assert(*FileSize < u32_Max)
                if(platform::ReadFile32(FileHandle, *Memory, (u32)*FileSize, 0)){
                    Success = true;
                } else {
                    if(!platform::MemoryFree(Memory)){
                        Warning("platform::MemoryFree in ReadEntireFile failed");
                    }
                }
            }
        }

        if(!platform::CloseFileHandle(FileHandle)){
            Warning("platform::CloseFileHandle in ReadEntireFile failed");
        }
    }

    return Success;
}

u32
kadabra::FileExtensionIndex(char *Path){
    u32 Result = 0xFFFFFFFF;

    u32 Idx=0;
    while(Path[Idx] != '\0'){
        if(Path[Idx] == '.' && Path[Idx+1] != '\0'){
            Result = Idx+1;
        }
        Idx++;
    }

    return Result;
}