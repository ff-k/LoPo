#ifndef KDB_HEADER_ENTITY
#define KDB_HEADER_ENTITY

#include "kdb_component.h"

namespace kadabra {
    
    struct entity {
        component_transform  Transform;
        component_renderable *Renderable;
        
        b32 IsActive;
    };

}

#endif