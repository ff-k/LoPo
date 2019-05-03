#ifndef KDB_HEADER_RAY
#define KDB_HEADER_RAY

#include "kdb_common.h"
#include "kdb_math.h"

namespace kadabra {
    
    struct ray3 {
        vec3 Origin;
        vec3 Direction;
    };
    
    struct ray_hit {
        vec3 P;
        vec3 Normal;
        f32 t;
    
        b32 Hit;
    };

    b32 RayTriangleIntersection(ray_hit *Hit, ray3 Ray, 
                                vec3 P0, vec3 P1, vec3 P2);
    
    b32 RayTriangleIntersection(ray_hit *Hit, ray3 Ray, 
                                vec3 P0, vec3 P1, vec3 P2, 
                                vec3 N0, vec3 N1, vec3 N2);
    
}

#endif