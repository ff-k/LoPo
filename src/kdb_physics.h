#ifndef KDB_HEADER_PHYSICS
#define KDB_HEADER_PHYSICS

#include "kdb_common.h"
#include "kdb_math.h"

namespace kadabra {
    struct component_particle {
        vec3 Position;
        vec3 Velocity;
        vec3 Gravity;
        f32  Damping;
        
        component_particle(){
            Position    = Vec3(0.0f, 0.0f, 0.0f);
            Velocity    = Vec3(0.0f, 0.0f, 0.0f);
            Gravity     = Vec3(0.0f, 0.0f, 0.0f);
            Damping     = 0.0f;
        }
        
        component_particle(vec3 Position, vec3 Velocity, vec3 Gravity, f32 Damping){
            this->Position    = Position;
            this->Velocity    = Velocity;
            this->Gravity     = Gravity;
            this->Damping     = Damping;
        }
        
        void Integrate(f32 DeltaTime);
    };    
}

#endif

