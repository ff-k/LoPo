#ifndef KDB_HEADER_MOUSE_PICK
#define KDB_HEADER_MOUSE_PICK

#include "kdb_common.h"
#include "kdb_entity.h"

namespace kadabra {

    struct mouse_pick {
        entity *Entity;
        
        u32 FaceIndex;
        u32 ClosestVertIdx; // NOTE(furkan): Index within the face (0, 1 or 2)
        
        b32 IsEmpty;
    };

}

#endif