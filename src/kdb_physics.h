#ifndef KDB_HEADER_PHYSICS
#define KDB_HEADER_PHYSICS

#include "kdb_common.h"
#include "kdb_math.h"

namespace kadabra {
    struct particle {
        vec3 Position;
        vec3 Velocity;
        vec3 Gravity;
        vec3 TotalForce;
        f32  Damping;
        f32  InverseMass;
        
        particle(){
            Position    = Vec3(0.0f, 0.0f, 0.0f);
            Velocity    = Vec3(0.0f, 0.0f, 0.0f);
            Gravity     = Vec3(0.0f, 0.0f, 0.0f);
            Damping     = 0.0f;
            InverseMass = 0.0f;
        }
        
        particle(vec3 Position, vec3 Velocity, vec3 Gravity, f32 Damping, f32 Mass){
            this->Position    = Position;
            this->Velocity    = Velocity;
            this->Gravity     = Gravity;
            this->TotalForce  = Vec3(0.0f, 0.0f, 0.0f);
            this->Damping     = Damping;
            this->InverseMass = (Mass != 0.0f) ? 1.0f/Mass : f32_Max;
        }
        
        void Integrate(f32 DeltaTime);
    };    
}

#endif

